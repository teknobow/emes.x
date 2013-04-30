#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 teknobow
#


if (($#ARGV != 2) || (!($ARGV[0] =~ /m680[03]0/)) || (! -d $ARGV[1]) || (! -d $ARGV[2]))
{
	printf STDERR "z80emu.pl <m68000|m68030> <出力先ディレクトリ>"
			. " <フラグテーブルのディレクトリ>\n";

	exit 1;
}


$sw_mpu = "-".$ARGV[0];
if ($ARGV[0] eq "m68000")
{
	$sw_mpu .= " -s MPU=68000";
}
else
{
	$sw_mpu .= " -s MPU=68030";
}
$outdir = $ARGV[1];
$flagdir = $ARGV[2];



$SIZE_X    = 54;
$SIZE_CB   = 28;
$SIZE_DD   = 48;
$SIZE_DDCB = 32;
$SIZE_ED   = 56;
$SIZE_TAIL = 0;

$SIZE_VW   = 114;
$SIZE_VS   =  96;

$REQUIRE_NOP_SIZE    = 16;
$REQUIRE_DDCB00_SIZE = 6;

$Flag_Cc = 0x90;
$Flag_C  = 0x80;



print "pass 1\n";
$comline = "has -w $sw_mpu -s PASS=1 -o $outdir/z80emu.o -x $outdir/z80emu.lab z80emu.has";
print $comline, "\n";
$ret = system($comline);
exit 1 if $ret;


# ラベルファイルから各Z80命令の開始アドレス、終了アドレスを取得
print "read opcode address.\n";
system("cdc 0");
&read_opcode_adr("$outdir/z80emu.lab");
system("cdc 1");



sub read_opcode_adr
{
	#-- 拡張スロット選択ルーチンへのオフセット値
	$cnt_rel_extslot = 0;
	
	open (lfile, $_[0]) || die "can't open '$_[0]'.";
	while (<lfile>)
	{
		if (/^_beginOpcode .* ([0-9A-F]+)$/)
		{
			$beginOpcode = hex($1);
		}
		elsif (/^_Zjmp .* ([0-9A-F]+)$/)
		{
			$Zjmp = hex($1);
		}
		elsif (/^_endOpcode .* ([0-9A-F]+)$/)
		{
			$endOpcode = hex($1);
		}
		elsif (/^X([se])_([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($2);
			local($adr) = hex($3);
			local($op)  = $mno >> 8;
			printf("%6.02X\r", $mno);
			$mno = $mno % 256;
			if ($1 eq "s")
			{
				$op == 0xDDCB && ($startDDCB[$mno] = $adr, next);
				$op ==   0xED && ($startED[$mno]   = $adr, next);
				$op ==   0xDD && ($startDD[$mno]   = $adr, next);
				$op ==   0xCB && ($startCB[$mno]   = $adr, next);
				$startX[$mno] = $adr;
			}
			else
			{
				$op == 0xDDCB && ($endDDCB[$mno] = $adr, next);
				$op ==   0xED && ($endED[$mno]   = $adr, next);
				$op ==   0xDD && ($endDD[$mno]   = $adr, next);
				$op ==   0xCB && ($endCB[$mno]   = $adr, next);
				$endX[$mno] = $adr;
			}
		}
		elsif (/^VW([se])_([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($2);
			local($adr) = hex($3);
			printf("  VW%02X\r", $mno);
			$startVW[$mno] = $adr if ($1 eq "s");
			$endVW[$mno]   = $adr if ($1 eq "e");
		}
		elsif (/^VS([se])_([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($2);
			local($adr) = hex($3);
			printf("  VS%02X\r", $mno);
			$startVS[$mno] = $adr if ($1 eq "s");
			$endVS[$mno]   = $adr if ($1 eq "e");
		}
		elsif (/^RELADR_EXTSLOT_([0-9]+) .* ([0-9A-F]+)$/)
		{
			$reladr_extslot[$cnt_rel_extslot++] = hex($2);
		}
	}
	close(lfile);
}




#各Z80命令のサイズを求める
print "check opcode address.\n";
&check_address;

sub check_address
{
	local($nerr) = 0;
	local($i);
	
	$maxX = $maxCB = $maxDD = $maxDDCB = $maxED = $maxVW = $maxVS = 0;
	
	for ($i = 0; $i < 256; ++$i)
	{
		! defined($startX[$i])
			&& (printf("opcode %02X の開始アドレスがない\n", $i),     ++$nerr);
		! defined($startCB[$i])
			&& (printf("opcode CB%02X の開始アドレスがない\n", $i),   ++$nerr);
		! defined($startDD[$i])
			&& (printf("opcode DD%02X の開始アドレスがない\n", $i),   ++$nerr);
		! defined($startDDCB[$i])
			&& (printf("opcode DDCB%02X の開始アドレスがない\n", $i), ++$nerr);
		! defined($startED[$i])
			&& (printf("opcode ED%02X の開始アドレスがない\n", $i),   ++$nerr);
		
		! defined($endX[$i])
			&& (printf("opcode %02X の終了アドレスがない\n", $i),     ++$nerr);
		! defined($endCB[$i])
			&& (printf("opcode CB%02X の終了アドレスがない\n", $i),   ++$nerr);
		! defined($endDD[$i])
			&& (printf("opcode DD%02X の終了アドレスがない\n", $i),   ++$nerr);
		! defined($endDDCB[$i])
			&& (printf("opcode DDCB%02X の終了アドレスがない\n", $i), ++$nerr);
		! defined($endED[$i])
			&& (printf("opcode ED%02X の終了アドレスがない\n", $i),   ++$nerr);
		
		$sizeX[$i]    = $endX[$i]    - $startX[$i];
		$sizeCB[$i]   = $endCB[$i]   - $startCB[$i];
		$sizeDD[$i]   = $endDD[$i]   - $startDD[$i];
		$sizeDDCB[$i] = $endDDCB[$i] - $startDDCB[$i];
		$sizeED[$i]   = $endED[$i]   - $startED[$i];
		
		($maxX    = $sizeX[$i],    $maxopX    = $i) if $maxX    < $sizeX[$i];
		($maxCB   = $sizeCB[$i],   $maxopCB   = $i) if $maxCB   < $sizeCB[$i];
		($maxDD   = $sizeDD[$i],   $maxopDD   = $i) if $maxDD   < $sizeDD[$i];
		($maxDDCB = $sizeDDCB[$i], $maxopDDCB = $i) if $maxDDCB < $sizeDDCB[$i];
		($maxED   = $sizeED[$i],   $maxopED   = $i) if $maxED   < $sizeED[$i];
	}
	
	
	! defined($startVW[0])
		&& (printf("VRAMwrite 00 の開始アドレスがない\n"), ++$nerr);
	! defined($startVS[0])
		&& (printf("VRAMsetAdr 00 の開始アドレスがない\n"), ++$nerr);
	
	! defined($endVW[0])
		&& (printf("VRAMwrite 00 の終了アドレスがない\n"), ++$nerr);
	! defined($endVS[0])
		&& (printf("VRAMsetAdr 00 の終了アドレスがない\n"), ++$nerr);
	
	$sizeVW[0] = $endVW[0] - $startVW[0];
	$sizeVS[0] = $endVS[0] - $startVS[0];
	
	($maxVW = $sizeVW[0], $maxopVW = $i) if $maxVW < $sizeVW[0];
	
	for ($i = 0x80; $i <= 0xff; ++$i)
	{
		! defined($startVW[$i])
			&& (printf("VRAMwrite %02X の開始アドレスがない\n", $i), ++$nerr);
		! defined($startVS[$i])
			&& (printf("VRAMsetAdr %02X の開始アドレスがない\n", $i), ++$nerr);
		
		! defined($endVW[$i])
			&& (printf("VRAMwrite %02X の終了アドレスがない\n", $i), ++$nerr);
		! defined($endVS[$i])
			&& (printf("VRAMsetAdr %02X の終了アドレスがない\n", $i), ++$nerr);
		
		$sizeVW[$i] = $endVW[$i] - $startVW[$i];
		$sizeVS[$i] = $endVS[$i] - $startVS[$i];
		
		($maxVW = $sizeVW[$i], $maxopVW = $i) if $maxVW < $sizeVW[$i];
		($maxVS = $sizeVS[$i], $maxopVS = $i) if $maxVS < $sizeVS[$i];
	}
	
	exit 1 if $nerr;
}


printf("xx     : max = %3d bytes, opcode %02X\n", $maxX, $maxopX);
printf("CBxx   : max = %3d bytes, opcode CB%02X\n", $maxCB, $maxopCB);
printf("DDxx   : max = %3d bytes, opcode DD%02X\n", $maxDD, $maxopDD);
printf("DDCBxx : max = %3d bytes, opcode DDCB%02X\n", $maxDDCB, $maxopDDCB);
printf("EDxx   : max = %3d bytes, opcode ED%02X\n", $maxED, $maxopED);
printf("total = %3d bytes.\n", $maxX + $maxCB + $maxDD + $maxDDCB + $maxED);
printf("----------\n");
printf("VWxx   : max = %3d bytes, VRAMwrite %02X\n", $maxVW, $maxopVW);
printf("VSxx   : max = %3d bytes, VRAMsetAdr %02X\n", $maxVS, $maxopVS);
printf("NOP    = %3d bytes.\n", $sizeX[0]);
printf("DDCB00 = %3d bytes.\n", $sizeDDCB[0]);

$nerr = 0;
if ($maxX    > $SIZE_X)    { printf "SIZE_X error (> %d).\n",    $SIZE_X;    ++$nerr; }
if ($maxCB   > $SIZE_CB)   { printf "SIZE_CB error (> %d).\n",   $SIZE_CB;   ++$nerr; }
if ($maxDD   > $SIZE_DD)   { printf "SIZE_DD error (> %d).\n",   $SIZE_DD;   ++$nerr; }
if ($maxDDCB > $SIZE_DDCB) { printf "SIZE_DDCB error (> %d).\n", $SIZE_DDCB; ++$nerr; }
if ($maxED   > $SIZE_ED)   { printf "SIZE_ED error (> %d).\n",   $SIZE_ED;   ++$nerr; }
if ($maxVW   > $SIZE_VW)   { printf "SIZE_VW error (> %d).\n",   $SIZE_VW;   ++$nerr; }
if ($maxVS   > $SIZE_VS)   { printf "SIZE_VS error (> %d).\n",   $SIZE_VS;   ++$nerr; }


if ($maxX    < $SIZE_X)    { print "warning : maxX ($maxX) < SIZE_X ($SIZE_X).\n"; }
if ($maxCB   < $SIZE_CB)   { print "warning : maxCB ($maxCB) < SIZE_CB ($SIZE_CB).\n"; }
if ($maxDD   < $SIZE_DD)   { print "warning : maxDD ($maxDD) < SIZE_DD ($SIZE_DD).\n"; }
if ($maxDDCB < $SIZE_DDCB) { print "warning : maxDDCB ($maxDDCB) < SIZE_DDCB ($SIZE_DDCB).\n"; }
if ($maxED   < $SIZE_ED)   { print "warning : maxED ($maxED) < SIZE_ED ($SIZE_ED).\n"; }
if ($maxVW   < $SIZE_VW)   { print "warning : maxVW ($maxVW) < SIZE_VW ($SIZE_VW).\n"; }
if ($maxVS   < $SIZE_VS)   { print "warning : maxVS ($maxVS) < SIZE_VS ($SIZE_VS).\n"; }

exit 1 if $nerr;

if ($sizeX[0] != $REQUIRE_NOP_SIZE)
{
	print "NOP size error (!= $REQUIRE_NOP_SIZE).\n";
	exit 1;
}
if ($sizeDDCB[0] != $REQUIRE_DDCB00_SIZE)
{
	print "DDCB00 size error (!= $REQUIRE_DDCB00_SIZE).\n";
	exit 1;
}


#------
print "\npass 2\n";

print "read opcodes.\n";
system("cdc 0");
open (mfile, "z80emu.inc") || die "can't open 'z80emu.inc'.";
while (<mfile>)
{
	if (/[ \t]*opcode[ \t]([0-9A-F]+)/)
	{
		$mno = hex($1);
		$op = $mno >> 8;
		printf("%6.02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			if (/endopcode[ \t]([0-9A-F]+)/)
			{
				if ($mno != hex($1))
				{
					printf("error opcode %02X\n", $mno);
					exit 1;
				}
				last;
			}
			next if (/^[\t ]+$/);
			$m = $m . $_;
		}
		$op == 0xDDCB && ($opcodeDDCB[$mno & 0xff] = $m, next);
		$op ==   0xED && ($opcodeED[$mno & 0xff]   = $m, next);
		$op ==   0xDD && ($opcodeDD[$mno & 0xff]   = $m, next);
		$op ==   0xCB && ($opcodeCB[$mno & 0xff]   = $m, next);
		$opcodeX[$mno] = $m;
	}
}
close(mfile);

open (mfile, "../IO/vramwrite.inc") || die "can't open '../IO/vramwrite.inc'.";
while (<mfile>)
{
	if (/[ \t]*VRAMwrite[ \t]([0-9A-F]+)/)
	{
		$mno = hex($1);
		printf("  VW%02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			if (/EndVRAMwrite[ \t]([0-9A-F]+)/)
			{
				if ($mno != hex($1))
				{
					printf("error VRAMwrite %02X\n", $mno);
					exit 1;
				}
				last;
			}
			next if (/^[\t ]+$/);
			$m = $m . $_;
		}
		$opcodeVW[$mno] = $m;
	}
	elsif (/[ \t]*VRAMsetAdr[ \t]([0-9A-F]+)/)
	{
		$mno = hex($1);
		printf("  VS%02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			if (/EndVRAMsetAdr[ \t]([0-9A-F]+)/)
			{
				if ($mno != hex($1))
				{
					printf("error VRAMsetAdr %02X\n", $mno);
					exit 1;
				}
				last;
			}
			next if (/^[\t ]+$/);
			$m = $m . $_;
		}
		$opcodeVS[$mno] = $m;
	}
}
close(mfile);


system("cdc 1");


#
print "read flag tables. [";

sub read_flag_table
{
	local($name, *ary, $fname, $len, $packstr) = @_;
	local($flag);
	
	print $name;
	open(ffile, "$flagdir/$fname") || die " not found '$flagdir/$fname'.";
	binmode ffile;
	if (read(ffile, $flag, $len) != $len)
	{
		print " file size error.\n";
		exit 1;
	}
	@ary = unpack($packstr, $flag);
	close(ffile);
}

&read_flag_table("INC", *inc, "inc.flg", 256, "C256");
&read_flag_table(", DEC", *dec, "dec.flg", 256, "C256");
&read_flag_table(", RLC", *rlc, "rlc.flg", 256, "C256");
&read_flag_table(", RRC", *rrc, "rrc.flg", 256, "C256");
&read_flag_table(", RL",  *rl,  "rl.flg",  512, "C512");
&read_flag_table(", RR",  *rr,  "rr.flg",  512, "C512");
### &read_flag_table(", SLA", *sla, "sla.flg", 256, "C256");
&read_flag_table(", SRA", *sra, "sra.flg", 256, "C256");
### &read_flag_table(", SRL", *srl, "srl.flg", 256, "C256");
#&read_flag_table(", SLL", *sll, "sll.flg", 256, "C256");
print "]\n";


#--- Special instruction
print "read special instruction data.\n";
open(spfile, "$outdir/special_adr.dat")	|| die " not found '$outdir/special_adr.dat'.";
binmode spfile;
if (read(spfile, $dt, 256*2) != 256*2)
{
	print " file size error.\n";
	exit 1;
}
@special_adr = unpack("S256", $dt);
close(spfile);



#-------------------------------------
print "build source.\n";
open (mfile, ">$outdir/z80emu.tmp") || die "can't open '$outdir/z80emu.tmp'.";
system("cdc 0");

#----- begin address
print mfile "_beginOpcode:\n";

for ($i = 0x80; $i <= 0x17f; ++$i)
{
	$mno = $i & 0xff;
	printf "%02X\r", $mno;
	
	
	#----- DDCB
	printf mfile "	opcode DDCB%02X\n", $mno;
	print mfile $opcodeDDCB[$mno];
	if ($mno == 0)
	{
		print mfile "	VRAMsetAdr 00\n";
		print mfile $opcodeVS[0];
		print mfile "	EndVRAMsetAdr 00\n";
		
		printf mfile "	.dcb.b	%d,0\n",
			$SIZE_DDCB - ($sizeDDCB[$mno] + $sizeVS[0]);
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $SIZE_DDCB - $sizeDDCB[$mno];
	}
	printf mfile "	endopcode DDCB%02X\n", $mno;
	
	
	#----- ED
	printf mfile "	opcode ED%02X\n", $mno;
	print mfile $opcodeED[$mno];
	printf mfile "	.dcb.b	%d,0\n", $SIZE_ED - $sizeED[$mno];
	printf mfile "	endopcode ED%02X\n", $mno;
	
	
	#----- FLAG etc.
	printf mfile "	.dcb.b	10,0\n";
	
	#-- VRAM Low/High
	printf mfile "	.dc.w	0\n";
	printf mfile "	.dc.w	0\n";
	
	#-- for Special
	printf mfile "	.dc.w	-1\n";				#-- check address
	printf mfile "	.dc.w	\$%04X\n", $special_adr[$mno];	#-- proc address
	
	#-- 空き
	printf mfile "	.dc.b	0\n";
	
	#-- for SRA rh
	printf mfile "	.dc.b	%d\n", $sra[$mno];
	
	#-- for RR rh, SRL rh
	printf mfile "	.dc.b	%d,%d\n", $rr[(($mno - 1) & 0xff) + 256], $rr[$mno];
	
	#-- for RL rh, SLA rh
	printf mfile "	.dc.b	%d,%d\n", $rl[(($mno - 1) & 0xff) + 256], $rl[$mno];
	
	#-- for RRC rh
	printf mfile "	.dc.b	%d,%d\n", $rrc[($mno - 1) & 0xff], $rrc[$mno];
	
	#-- for RLC rh
	printf mfile "	.dc.b	%d,%d\n", $rlc[($mno - 1) & 0xff], $rlc[$mno];
	
	#-- for ADC rh, SBC rh
	printf mfile "	.dc.b	%d,%d\n", $mno, $mno;
	
	#-- for DEC rh
	printf mfile "	.dc.b	%d,%d\n", $dec[($mno - 1) & 0xff] | $Flag_C, $dec[$mno];
	
	#-- for INC rh
	printf mfile "	.dc.b	%d,%d\n", $inc[($mno - 1) & 0xff] | $Flag_C, $inc[$mno];
	
	#-- for RAM/ROM check
	printf mfile "	.dc.w	0\n";
	
	#-- for opcode fetch, interrupt, debugger and so on.
	print mfile "_Zjmp:\n"   if $mno == 0;
	
	printf mfile "	.dcb.w	1,0\n";
	
	
	#----- XX
	printf mfile "	opcode %02X\n", $mno;
	print mfile $opcodeX[$mno];
	if ($mno == 0)
	{
		print mfile "	VRAMwrite 00\n";
		print mfile $opcodeVW[0];
		print mfile "	EndVRAMwrite 00\n";
		
		printf mfile "	.dcb.b	%d,0\n",
			$SIZE_X - ($sizeX[$mno] + $sizeVW[0]);
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $SIZE_X - $sizeX[$mno];
	}
	printf mfile "	endopcode %02X\n", $mno;
	
	
	#----- CB
	printf mfile "	opcode CB%02X\n", $mno;
	print mfile $opcodeCB[$mno];
	printf mfile "	.dcb.b	%d,0\n", $SIZE_CB - $sizeCB[$mno];
	printf mfile "	endopcode CB%02X\n", $mno;
	
	
	#----- DD
	printf mfile "	opcode DD%02X\n", $mno;
	print mfile $opcodeDD[$mno];
	printf mfile "	.dcb.b	%d,0\n", $SIZE_DD - $sizeDD[$mno];
	printf mfile "	endopcode DD%02X\n", $mno;
	
	printf mfile "	.dcb.b	%d,0\n", $SIZE_TAIL;
}
system("cdc 1");
close(mfile);


open (mfile, ">$outdir/vramwrite.tmp") || die "can't open '$outdir/vramwrite.tmp'.";
system("cdc 0");
$sizeNOP = $sizeX[0];
for ($i = 0x80; $i <= 0xff; ++$i)
{
	$mno = $i & 0xff;
	printf "%02X\r", $mno;
	
	printf mfile "	.dcb.b	%d,0\n", $REQUIRE_DDCB00_SIZE;
	
	printf mfile "	VRAMsetAdr %02X\n", $mno;
	print mfile $opcodeVS[$mno];
	printf mfile "	.dcb.b	%d,0\n", $SIZE_VS - $sizeVS[$mno];
	printf mfile "	EndVRAMsetAdr %02X\n", $mno;
	
	#----- FLAG etc.
	printf mfile "	.dcb.b	0,0\n";
	
	#-- for Special (unuse)
	printf mfile "	.dc.w	0,0\n";
	
	#-- for RAM/ROM check (unuse)
	printf mfile "	.dc.b	0\n";
	
	#-- for SRA rh (unuse)
	printf mfile "	.dc.b	0\n";
	
	#-- for RR rh, SRL rh
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $rr[(($mno - 1) & 0xff) + 256];
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	#-- for RL rh, SLA rh
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $rl[(($mno - 1) & 0xff) + 256];
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	#-- for RRC rh
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $rrc[($mno - 1) & 0xff];
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	#-- for RLC rh
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $rlc[($mno - 1) & 0xff];
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	#-- for ADC rh, SBC rh (unuse)
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $mno;
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	#-- for DEC rh
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $dec[($mno - 1) & 0xff] | $Flag_C;
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	#-- for INC rh
	if ($mno == 0x80)
	{
		printf mfile "	.dc.b	%d,0\n", $inc[($mno - 1) & 0xff] | $Flag_C;
	}
	else
	{
		printf mfile "	.dc.b	0,0\n";
	}
	
	# VRAM_BLOCK
	print mfile "	.dc.w	0\n";
	# FETCH
	print mfile "	.dc.w	0\n";
	
	printf mfile "	.dcb.b	%d,0\n", $REQUIRE_NOP_SIZE;
	
	printf mfile "	VRAMwrite %02X\n", $mno;
	print mfile $opcodeVW[$mno];
	printf mfile "	.dcb.b	%d,0\n", $SIZE_VW - $sizeVW[$mno];
	printf mfile "	EndVRAMwrite %02X\n", $mno;
}

close(mfile);
system("cdc 1");


#---
$comline =
    "has -w $sw_mpu -s PASS=2 -o $outdir/z80emu.o -x $outdir/z80emu.lab z80emu.has";
print $comline, "\n";
$ret = system($comline);
if ($ret) {
	exit 1;
}


print "read opcode address.\n";
undef @startX;
undef @startCB;
undef @startDD;
undef @startDDCB;
undef @startED;
undef @startFD;
undef @startFDCB;
undef @startVW;
undef @startVS;

undef @endX;
undef @endCB;
undef @endDD;
undef @endDDCB;
undef @endED;
undef @endFD;
undef @endFDCB;
undef @endVW;
undef @endVS;

system("cdc 0");
&read_opcode_adr("$outdir/z80emu.lab");
system("cdc 1");

print "check opcode address.\n";
&check_address;

#---
printf "Z80emu size : %d + %d = %d bytes.\n",
			$Zjmp - $beginOpcode,
			$endOpcode - $Zjmp,
			$endOpcode - $beginOpcode;


#---
print "create $outdir/z80emusize.equ.\n";
open (mfile, ">$outdir/z80emusize.equ") || die "can't open '$outdir/z80emusize.equ'.";
printf mfile "*** This file was create by z80emu.pl. ***\n\n";
printf mfile "SIZE_Z80emu: .equ %d\n", $endOpcode - $beginOpcode;
printf mfile "SIZE_Zjmp: .equ %d\n", $Zjmp - $beginOpcode;
close(mfile);


#---
$nerr = 0;
for ($i = 0; $i < 256; ++$i) {
	if ($i != 0x7f)
	{
		if ($startDDCB[($i + 1) & 0xff] - $startDDCB[$i] != 256)
		{
			printf("error opcode %02X group, != 256 bytes. (%d)\n",
				$i, $startDDCB[($i + 1) & 0xff] - $startDDCB[$i]);
			++$nerr;
		}
	}
	
	$sizeX[$i]    != $SIZE_X
		&& (printf("error opcode %02X, %d bytes.\n", $i, $sizeX[$i]), ++$nerr);
	
	$sizeCB[$i]   != $SIZE_CB
		&& (printf("error opcode CB%02X, %d bytes.\n", $i, $sizeCB[$i]), ++$nerr);
	
	$sizeDD[$i]   != $SIZE_DD
		&& (printf("error opcode DD%02X, %d bytes.\n", $i, $sizeDD[$i]), ++$nerr);
	
	$sizeDDCB[$i] != $SIZE_DDCB
		&& (printf("error opcode DDCB%02X, %d bytes.\n", $i, $sizeDDCB[$i]), ++$nerr);
	
	$sizeED[$i] != $SIZE_ED
		&& (printf("error opcode ED%02X, %d bytes.\n", $i, $sizeED[$i]), ++$nerr);
	
	if ($i == 0x7f)
	{
		if ($startVS[0x80] - $startDDCB[0x7f] - $REQUIRE_DDCB00_SIZE != 256)
		{
			printf("error VRAMsetAdr group not start 256\n");
			++$nerr;
		}
		
		if ($startVW[0x80] - $startX[0x7f] - $REQUIRE_NOP_SIZE != 256)
		{
			printf("error VRAMwrite group not start 256\n");
			++$nerr;
		}
	}
	
	if ($i >= 0x80 && $i < 0xff)
	{
		if ($startVW[($i + 1) & 0xff] - $startVW[$i] != 256)
		{
			printf("error VRAMwrite %02X group, != 256 bytes. (%d)\n",
				$i, $startVW[($i + 1) & 0xff] - $startVW[$i]);
			++$nerr;
		}
	}
	
	if ($i >= 0x80)
	{
		$sizeVW[$i] != $SIZE_VW
			&& (printf("error VRAMwrite %02X, %d bytes.\n", $i, $sizeVW[$i]),
				++$nerr);
		
		$sizeVS[$i] != $SIZE_VS
			&& (printf("error VRAMsetAdr %02X, %d bytes.\n", $i, $sizeVS[$i]),
				++$nerr);
	}
}
if ($nerr) {
	exit 1;
}


#----------------
open(file, ">$outdir/relextslot.has") || die "can't open '$outdir/relextslot.has'.";
printf file "*** This file was create by z80emu.pl. ***\n\n";
print file "	.xdef RelAdr_ExtSlot\n";
print file "	.data\n";
print file "RelAdr_ExtSlot:\n";
for ($i = 0; $i < $cnt_rel_extslot; ++$i)
{
	printf file "	.dc.w	\$%04x\n", ($reladr_extslot[$i] - $Zjmp) & 0xffff;
}
print file "	.dc.w	0\n";
close(file);

$comline = "has $outdir/relextslot.has -o $outdir/relextslot.o";
print $comline, "\n";
$ret = system($comline);
if ($ret) {
	exit 1;
}


print "\ncomplete.\n\n";
