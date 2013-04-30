#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 teknobow
#



if (($#ARGV != 2) || (!($ARGV[0] =~ /m680[03]0/)) || (! -d $ARGV[1]) || (! -d $ARGV[2]))
{
	printf STDERR "makeftbl.pl <m68000|m68030> <出力先ディレクトリ>"
			. " <フラグテーブルのディレクトリ>\n";

	exit 1;
}


$sw_mpu = "-".$ARGV[0];
$outdir = $ARGV[1];
$flagdir = $ARGV[2];


$Flag_Cc = 0x90;
$Flag_C  = 0x80;

sub read_flag_table
{
	local($name, *ary, $fname, $len, $packstr) = @_;
	local($flag);
	
	print $name;
	open(ffile, "$flagdir/$fname") || die " not found \'$flagdir/$fname\'.";
	binmode ffile;
	if (read(ffile, $flag, $len) != $len)
	{
		print " file size error.\n";
		exit 1;
	}
	@ary = unpack($packstr, $flag);
	close(ffile);
}

print "read flag tables.\n";
print "[";
&read_flag_table("INC",         *inc,     "inc.flg",      256, "C256");
&read_flag_table(", DEC",       *dec,     "dec.flg",      256, "C256");
&read_flag_table(", DAA",       *daa,     "daa.flg",     4096, "C4096");
&read_flag_table(", AND",       *and,     "and.flg",      256, "C256");
&read_flag_table(", OR/XOR",    *or,      "or.flg",       256, "C256");
&read_flag_table(", RLC",       *rlc,     "rlc.flg",      256, "C256");
&read_flag_table(", RRC",       *rrc,     "rrc.flg",      256, "C256");
&read_flag_table(", RL",        *rl,      "rl.flg",       512, "C512");
&read_flag_table(", RR",        *rr,      "rr.flg",       512, "C512");
&read_flag_table(", SRA",       *sra,     "sra.flg",      256, "C256");
&read_flag_table(", ADC16",     *adc16,   "adc16.flg",     32, "C32");
&read_flag_table(", SBC16",     *sbc16,   "sbc16.flg",     32, "C32");
&read_flag_table(", RLD/RRD",   *rrld,    "rrld.flg",     256, "C256");
&read_flag_table(", IFF",       *iff,     "iff.flg",      256, "C256");
print "]\n";


#
print "create $outdir/flagtbl.has.\n";
open (file, ">$outdir/flagtbl.has") || die "can't open '$outdir/flagtbl.has'.";

#
print file "*** This file was create by makeftbl.pl. ***\n\n";

print file "	.include z80emu.equ\n";
print file "	.include ../IO/x68k.equ\n";
print file "	.include ../IO/vdp.equ\n";
print file "	.include ../IO/slot.equ\n";

print file "	.xdef Ftbl,FtblinIO\n";
print file "	.xdef _SlotTablePage0\n";
print file "	.xdef _SlotTablePage1\n";
print file "	.xdef _SlotTablePage2\n";
print file "	.xdef _SlotTablePage3\n";

print file "	.xdef _PSG_ProcTable\n";

print file "	.xdef _workBC2,_workDE2,_workHL2\n";
print file "	.xdef _workIX,_workIY\n";
print file "	.xdef _workI,_Z80_DIEI\n";

print file "	.xref	TXT1_H_IOroutine,TXT1_L_IOroutine\n";
print file "	.xref	TXT2_H_IOroutine,TXT2_L_IOroutine\n";
print file "	.xref	MLT_H_IOroutine,MLT_L_IOroutine\n";
print file "	.xref	GRA1_H_IOroutine,GRA1_L_IOroutine\n";
print file "	.xref	GRA2_H_IOroutine,GRA2_L_IOroutine\n";
print file "	.xref	GRA3_H_IOroutine,GRA3_L_IOroutine\n";
print file "	.xref	GRA4_H_IOroutine,GRA4_L_IOroutine\n";
print file "	.xref	GRA5_H_IOroutine,GRA5_L_IOroutine\n";
print file "	.xref	GRA6_H_IOroutine,GRA6_L_IOroutine\n";
print file "	.xref	GRA7_H_IOroutine,GRA7_L_IOroutine\n";

#--
print file "\n";
print file "SET_OFFSET .macro lab\n";
print file "	lab: .equ _&lab-Ftbl\n";
print file "	.endm\n";
print file "\n";
print file "SET_OFFSET_IO .macro lab\n";
print file "	lab: .equ _&lab-FtblinIO\n";
print file "	.endm\n";
print file "\n";

#--
print file "	.text\n";

print file "	.quad\n";

print file "	.dcb.b	1024*16,0\n";

							$offsetFtblinIO = 0;
print file "FtblinIO:	.dc.l	0\n";			$offsetFtblinIO += 4;

print file "Ftbl:\n";
print file "_Z80intrFlag:	.equ *+1\n";

# opcode no.2 fetch work
print file "	.dc.w	0\n";				$offsetFtblinIO += 2;

# exx
print file "_regSave:\n";
print file "_workBC2:	.dc.w	0\n";			$offsetFtblinIO += 2;
print file "_workDE2:	.dc.w	0\n";			$offsetFtblinIO += 2;
print file "_workHL2:	.dc.w	0\n";			$offsetFtblinIO += 2;

# IX, IY, I register work
print file "_workIX:	.dc.w	0\n";			$offsetFtblinIO += 2;
print file "_workIY:	.dc.w	0\n";			$offsetFtblinIO += 2;
print file "_workI:	.dc.w	0\n";			$offsetFtblinIO += 2;

# interrupt work
print file "_Z80_DIEI:		.dc.w	0\n";		$offsetFtblinIO += 2;
print file "_Z80intrSuspend:	.dc.w	-1\n";		$offsetFtblinIO += 2;
print file "_Z80INTREQ:		.dc.b	\$04,0\n";	$offsetFtblinIO += 2;

# break point
print file "_w_breakPoint:	.dc.w	0,0\n";		$offsetFtblinIO += 4;

# emes.x 終了フラグ
print file "_terminateFlag:	.dc.w	0\n";		$offsetFtblinIO += 2;

# dummy routine
print file "_DummyPSGroutine:	st.b	d1\n";		$offsetFtblinIO += 2;
print file "_DummyRoutine:	rts\n";			$offsetFtblinIO += 2;

# PSG 用ワークエリア
print file "_PSGwork:		.dcb.b	32,0\n";	$offsetFtblinIO += 32;


# FtblinIO からのオフセットを 0x100 にあわせる
printf file "		.dcb.b	%d,0\n", 0x100 - $offsetFtblinIO;

# スロット管理テーブル
print file "_SlotTable:\n";
print file "_SlotTablePage0:	.dcb.b	16*4*4,0\n";	# 0x100 bytes
print file "_SlotTablePage1:	.dcb.b	16*4*4,0\n";	# 0x100 bytes
print file "_SlotTablePage2:	.dcb.b	16*4*4,0\n";	# 0x100 bytes
print file "_SlotTablePage3:	.dcb.b	16*4*4,0\n";	# 0x100 bytes
print file "_SlotTableMemType:	.dcb.b	16*4*4*4,0\n";	# 0x400 bytes

# PSG ルーチン
$dumPSG = "_DummyPSGroutine-Ftbl";
print file "_PSG_ProcTable:\n";
print file "_PSGread_0:		.dc.w	$dumPSG\n";
print file "_PSGread_1:		.dc.w	$dumPSG\n";
print file "_PSGread_2:		.dc.w	$dumPSG\n";
print file "_PSGread_3:		.dc.w	$dumPSG\n";
print file "_PSGread_4:		.dc.w	$dumPSG\n";
print file "_PSGread_5:		.dc.w	$dumPSG\n";
print file "_PSGread_6:		.dc.w	$dumPSG\n";
print file "_PSGread_7:		.dc.w	$dumPSG\n";
print file "_PSGread_8:		.dc.w	$dumPSG\n";
print file "_PSGread_9:		.dc.w	$dumPSG\n";
print file "_PSGread_10:	.dc.w	$dumPSG\n";
print file "_PSGread_11:	.dc.w	$dumPSG\n";
print file "_PSGread_12:	.dc.w	$dumPSG\n";
print file "_PSGread_13:	.dc.w	$dumPSG\n";
print file "_PSGread_14:	.dc.w	_procPSGread_14-Ftbl\n";
print file "_PSGread_15:	.dc.w	_procPSGread_15-Ftbl\n";

print file "_PSGwrite_0:	.dc.w	$dumPSG\n";
print file "_PSGwrite_1:	.dc.w	$dumPSG\n";
print file "_PSGwrite_2:	.dc.w	$dumPSG\n";
print file "_PSGwrite_3:	.dc.w	$dumPSG\n";
print file "_PSGwrite_4:	.dc.w	$dumPSG\n";
print file "_PSGwrite_5:	.dc.w	$dumPSG\n";
print file "_PSGwrite_6:	.dc.w	$dumPSG\n";
print file "_PSGwrite_7:	.dc.w	$dumPSG\n";
print file "_PSGwrite_8:	.dc.w	$dumPSG\n";
print file "_PSGwrite_9:	.dc.w	$dumPSG\n";
print file "_PSGwrite_10:	.dc.w	$dumPSG\n";
print file "_PSGwrite_11:	.dc.w	$dumPSG\n";
print file "_PSGwrite_12:	.dc.w	$dumPSG\n";
print file "_PSGwrite_13:	.dc.w	$dumPSG\n";
print file "_PSGwrite_14:	.dc.w	$dumPSG\n";
print file "_PSGwrite_15:	.dc.w	_procPSGwrite_15-Ftbl\n";

# SCC work
print file "_SCCwork:		.dcb.b	256,0\n";


# INC rl
for ($c = 1; $c >= 0; --$c)
{
	print file "_FtblINCrl:\n" if $c == 0;
	
	for ($i = 0; $i < 256; $i += 16)
	{
		print file "	.dc.b	";
		for ($j = 0; $j < 16; ++$j)
		{
			print file "," if $j != 0;
			printf file "\$%02x", $inc[$i + $j] | ($c ? $Flag_C : 0);
		}
		print file "\n";
	}
}

print file "\n";

# DEC rl
for ($c = 1; $c >= 0; --$c)
{
	print file "_FtblDECrl:\n" if $c == 0;
	
	for ($i = 0; $i < 256; $i += 16)
	{
		print file "	.dc.b	";
		for ($j = 0; $j < 16; ++$j)
		{
			print file "," if $j != 0;
			printf file "\$%02x", $dec[$i + $j] | ($c ? $Flag_C : 0);
		}
		print file "\n";
	}
}

print file "\n";

# DAA
print file "_FtblDAA:\n";
for ($i = 0; $i < 2048; $i += 8)
{
	print file "	.dc.b	";
	for ($j = 0; $j < 8; ++$j)
	{
		print file "," if $j != 0;
		printf file "%d,%d", $daa[$i + $j], $daa[2048 + $i + $j];
	}
	print file "\n";
}

print file "\n";

# AND
print file "_FtblANDr:\n";
for ($i = 0; $i < 256; $i += 16)
{
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		print file "," if $j != 0;
		printf file "\$%02x", $and[$i + $j];
	}
	print file "\n";
}

print file "\n";

# OR/XOR
print file "_FtblORr:\n";
print file "_FtblXORr:\n";
for ($i = 0; $i < 256; $i += 16)
{
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		print file "," if $j != 0;
		printf file "\$%02x", $or[$i + $j];
	}
	print file "\n";
}

print file "\n";

# RLC
print file "_FtblRLCrl:\n";
for ($i = 0; $i < 256; $i += 16)
{
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		print file "," if $j != 0;
		printf file "\$%02x", $rlc[$i + $j];
	}
	print file "\n";
}

print file "\n";

# RRC
print file "_FtblRRCrl:\n";
for ($i = 0; $i < 256; $i += 16)
{
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		print file "," if $j != 0;
		printf file "\$%02x", $rrc[$i + $j];
	}
	print file "\n";
}

print file "\n";

# RL, SLA
for ($c = 1; $c >= 0; --$c)
{
	print file "_FtblRLrl:\n" if $c == 0;
	print file "_FtblSLArl:\n" if $c == 0;
	
	for ($i = 0; $i < 256; $i += 16)
	{
		print file "	.dc.b	";
		for ($j = 0; $j < 16; ++$j)
		{
			print file "," if $j != 0;
			printf file "\$%02x", $rl[$c * 256 + $i + $j];
		}
		print file "\n";
	}
}

print file "\n";

# RR, SRL
for ($c = 1; $c >= 0; --$c)
{
	print file "_FtblRRrl:\n" if $c == 0;
	print file "_FtblSRLrl:\n" if $c == 0;
	
	for ($i = 0; $i < 256; $i += 16)
	{
		print file "	.dc.b	";
		for ($j = 0; $j < 16; ++$j)
		{
			print file "," if $j != 0;
			printf file "\$%02x", $rr[$c * 256 + $i + $j];
		}
		print file "\n";
	}
}

print file "\n";

# SRA
print file "_FtblSRArl:\n";
for ($i = 0; $i < 256; $i += 16)
{
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		print file "," if $j != 0;
		printf file "\$%02x", $sra[$i + $j];
	}
	print file "\n";
}

print file "\n";


# ADC16
for ($i = 0x80; $i < 0x180; $i += 16)
{
	print file "_FtblADC16:\n" if $i == 0x100;
	
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		$idx = ($i + $j) & 0xff;
		
		print file "," if $j != 0;
		printf file "\$%02x", $adc16[$idx & 0x1f];
	}
	print file "\n";
}

print file "\n";

# SBC16
for ($i = 0x80; $i < 0x180; $i += 16)
{
	print file "_FtblSBC16:\n" if $i == 0x100;
	
	print file "	.dc.b	";
	for ($j = 0; $j < 16; ++$j)
	{
		$idx = ($i + $j) & 0xff;
		
		print file "," if $j != 0;
		printf file "\$%02x", $sbc16[$idx & 0x1f];
	}
	print file "\n";
}

print file "\n";

# RLD/RRD/IN
for ($c = 1; $c >= 0; --$c)
{
	print file "_FtblRRLD:\n" if $c == 0;
	print file "_FtblIN:\n" if $c == 0;
	
	for ($i = 0; $i < 256; $i += 16)
	{
		print file "	.dc.b	";
		for ($j = 0; $j < 16; ++$j)
		{
			print file "," if $j != 0;
			printf file "\$%02x", $rrld[$i + $j] | ($c ? $Flag_Cc : 0);
		}
		print file "\n";
	}
}

print file "\n";


# IFF
for ($c = 1; $c >= 0; --$c)
{
	print file "_FtblIFF:\n" if $c == 0;
	
	for ($i = 0; $i < 256; $i += 16)
	{
		print file "	.dc.b	";
		for ($j = 0; $j < 16; ++$j)
		{
			print file "," if $j != 0;
			printf file "\$%02x", $iff[$i + $j] | ($c ? $Flag_Cc : 0);
		}
		print file "\n";
	}
}
print file "\n";



# MSX -> X68k パレット変換テーブル　Red Blue 成分
for ($i = 0x80; $i <= 0x17f; $i += 16)
{
	$idx = $i & 0xff;
	
	print file "_paletteConvTable_RB:\n" if $idx == 0;
	
	print file "	.dc.w	";
	for ($j = 0; $j < 8; ++$j)
	{
		$idx = ($i + $j*2) & 0xff;
		
		#-- Blue GGGG GRRR RRBB BBBI
		$blue = ($idx & 0x0e) >> 1;
		$pal = (($blue << 2) | ($blue >> 1)) << 1;
		
		#-- Red GGGG GRRR RRBB BBBI
		$red = ($idx & 0xe0) >> (4+1);
		$pal |= (($red << 2) | ($red >> 1)) << 6;
		
		print file "," if $j != 0;
		printf file "\$%04X", $pal | 1;
	}
	print file "\n";
}
print file "\n";


# MSX -> X68k パレット変換テーブル　Green 成分
for ($i = 0x80; $i <= 0x17f; $i += 16)
{
	$idx = $i & 0xff;
	
	print file "_paletteConvTable_G:\n" if $idx == 0;
	
	print file "	.dc.w	";
	for ($j = 0; $j < 8; ++$j)
	{
		$idx = ($i + $j*2) & 0xff;
		
		#-- Green GGGG GRRR RRBB BBBI
		$green = ($idx & 0x0e) >> 1;
		$pal = (($green << 2) | ($green >> 1)) << 11;
		
		print file "," if $j != 0;
		printf file "\$%04X", $pal | 1;
	}
	print file "\n";
}
print file "\n";


#-- Joystick
for ($i = 0; $i <= 0xff; $i += 16)
{
	print file "_JoyConvTable:\n" if $i == 0;
	print file "	.dc.b	";
	
	for ($j = 0; $j < 16; ++$j)
	{
		$idx = ($i + $j) & 0xff;
		
		$dt = 0x00;	# JIS 配列キーボード (後で反転するため)
		
		$dt |= 0x01 if !($idx & 0x01);
		$dt |= 0x02 if !($idx & 0x02);
		$dt |= 0x04 if !($idx & 0x04);
		$dt |= 0x08 if !($idx & 0x08);
		$dt |= 0x10 if !($idx & 0x20);
		$dt |= 0x20 if !($idx & 0x40);
		
		print file "," if $j != 0;
		printf file "\$%02X", 255-$dt;
	}
	print file "\n";
}
print file "\n";


#-- パターンネームテーブル座標からPCG座標への変換テーブル
for ($i = 0x80; $i <= 0x17f; $i += 16)
{
	print file "_convXY_Block1:\n" if $i == 0x100;
	print file "	.dc.w	";
	
	for ($j = 0; $j < 16; ++$j)
	{
		$idx = ($i + $j) & 0xff;
		
		$x = $idx & 0x1f;
		$y = ($idx & 0xe0) >> 5;
		
		print file "," if $j != 0;
		printf file "\$%02X", ($y * 64 + $x) * 2 + 0x4000;
	}
	print file "\n";
}
print file "\n";



#----------
print file "	.include ../IO/slot.inc\n";
print file "	.include ./romwrite.inc\n";

print file ".if USE_DEBUGGER.eq.1\n";
###print file "	.include debugger.inc\n";
print file "	.include except.inc\n";
print file ".endif\n";

print file "	.include ../IO/clockIC.inc\n";

print file "	.include ../IO/keyboard.inc\n";

print file "	.include ../IO/setvram.inc\n";

print file "	.include ../IO/vdp.inc\n";

print file "	.include ../IO/joystick.inc\n";

print file "	.include special.inc\n";

print file "tail_unsigned:\n";

print file ".if PASS.eq.2\n";
print file "	.include $outdir/drvspace.inc\n";
print file ".endif\n";

close(file);


#-----------
$comline =
  "has -s PASS=1 -w $sw_mpu -o $outdir/flagtbl.o -x $outdir/flagtbl.lab $outdir/flagtbl.has";
print $comline, "\n";
$ret = system($comline);
exit 1 if $ret;

#
open (lfile, "$outdir/flagtbl.lab") || die "can't open '$outdir/flagtbl.lab'.";
while (<lfile>)
{
	if (/^([0-9A-Za-z_]+) .* ([0-9A-F]+)$/)
	{
		$label{$1} = hex($2);
	}
}
close(lfile);

if ((!defined $label{"Ftbl"}) || (!defined $label{"FtblinIO"}))
{
	print "error.\n";
	exit 1;
}


$freeSigned   = 32768 - ($label{"tail_signed"}   - $label{"FtblinIO"});

$freeUnsigned = 65536 - ($label{"tail_unsigned"} - $label{"FtblinIO"});

printf "free - signed   : %d bytes.\n", $freeSigned;

printf "     - unsigned : %d bytes.\n", $freeUnsigned;



#---
open(file, ">$outdir/drvspace.inc") || die "can't open '$outdir/drvspace.inc'.";
print file "*** This file was create by makeftbl.pl. ***\n\n";
print file "	.xdef _driverSpace\n";
print file "_driverSpace:\n";
print file "	.dcb.b	$freeUnsigned,0\n";
close(file);

$comline =
  "has -s PASS=2 -w $sw_mpu -o $outdir/flagtbl.o -x $outdir/flagtbl.lab $outdir/flagtbl.has";
print $comline, "\n";
$ret = system($comline);
exit 1 if $ret;



#---
print "create $outdir/flagtbl.equ.\n";
open(outfile, ">$outdir/flagtbl.equ") || die "can't open '$outdir/flagtbl.equ'.";

print outfile "*** This file was create by makeftbl.pl. ***\n\n";

$Ftbl = $label{"Ftbl"};
$FtblinIO = $label{"FtblinIO"};
printf "Ftbl=0x%X, FtblinIO=0x%X\n", $Ftbl, $FtblinIO;

# print outfile "\t.xref Ftbl,FtblinIO\n";

sub offset
{
	local($l, $ll) = ($_[0], "_" . $_[0]);
	
	if (! defined $label{$ll})
	{
		printf "Label '%s' : not defined.\n", $ll;
		exit 1;
	}
	
	if ($label{$ll} - $Ftbl > 32767)
	{
		printf "Label '%s' : illegal relative. (0x%X)\n", $ll, $label{$ll};
		exit 1;
	}
	
	
	printf outfile "%-32s .equ \$%04x\n", $l, $label{$ll} - $Ftbl;
}

sub offsetIO
{
	local($l, $ll) = ($_[0], "_" . $_[0]);
	
	if (! defined $label{$ll})
	{
		printf "Label '%s' : not defined.\n", $ll;
		exit 1;
	}
	
	if ($label{$ll} - $FtblinIO > 32767)
	{
		printf "Label '%s' : illegal relative. (0x%X)\n", $ll, $label{$ll};
		exit 1;
	}
	
	printf outfile "%-32s .equ \$%04x\n", $l, $label{$ll} - $FtblinIO;
}


if (! defined $label{"_SlotTable"})
{
	print "Label '_SlotTable' : not defined.\n";
	exit 1;
}
if ($label{"_SlotTable"} - $FtblinIO != 0x100)
{
	printf "_SlotTableのオフセットが不正です。(%d)\n", $label{"_SlotTable"} - $FtblinIO;
	exit 1;
}


&offset("regSave");
&offset("workBC2");
&offset("workDE2");
&offset("workHL2");
&offset("workIX");
&offset("workIY");
&offset("workI");
&offset("Z80intrFlag");
&offset("Z80_DIEI");
&offset("Z80intrSuspend");
&offset("Z80INTREQ");

#-- Break Point
&offset("breakPoint");
&offset("breakPoint_1");
&offset("breakPoint_2");
&offset("w_breakPoint");

&offset("DummyRoutine");

#-- SLOT
&offsetIO("SlotTable");
&offsetIO("SlotTablePage0");
&offsetIO("SlotTablePage1");
&offsetIO("SlotTablePage2");
&offsetIO("SlotTablePage3");

#-- PSG
&offsetIO("PSGwork");
&offsetIO("PSG_ProcTable");
&offsetIO("PSGread_0");
&offsetIO("PSGread_1");
&offsetIO("PSGread_2");
&offsetIO("PSGread_3");
&offsetIO("PSGread_4");
&offsetIO("PSGread_5");
&offsetIO("PSGread_6");
&offsetIO("PSGread_7");
&offsetIO("PSGread_8");
&offsetIO("PSGread_9");
&offsetIO("PSGread_10");
&offsetIO("PSGread_11");
&offsetIO("PSGread_12");
&offsetIO("PSGread_13");
&offsetIO("PSGread_14");
&offsetIO("PSGread_15");
&offsetIO("PSGwrite_0");
&offsetIO("PSGwrite_1");
&offsetIO("PSGwrite_2");
&offsetIO("PSGwrite_3");
&offsetIO("PSGwrite_4");
&offsetIO("PSGwrite_5");
&offsetIO("PSGwrite_6");
&offsetIO("PSGwrite_7");
&offsetIO("PSGwrite_8");
&offsetIO("PSGwrite_9");
&offsetIO("PSGwrite_10");
&offsetIO("PSGwrite_11");
&offsetIO("PSGwrite_12");
&offsetIO("PSGwrite_13");
&offsetIO("PSGwrite_14");
&offsetIO("PSGwrite_15");

#-- SCC
&offsetIO("SCCwork");

#--
&offset("FtblINCrl");
&offset("FtblDECrl");
&offset("FtblDAA");
&offset("FtblANDr");
&offset("FtblORr");
&offset("FtblXORr");
&offset("FtblADC16");
&offset("FtblSBC16");
&offset("FtblRLCrl");
&offset("FtblRRCrl");
&offset("FtblRLrl");
&offset("FtblRRrl");
&offset("FtblSLArl");
&offset("FtblSRArl");
&offset("FtblSRLrl");
&offset("FtblRRLD");
&offset("FtblIN");
&offset("FtblIFF");

&offsetIO("paletteConvTable_RB");
&offsetIO("paletteConvTable_G");

&offsetIO("JoyConvTable");

&offsetIO("convXY_Block1");

#--
###&offset("debugger");
###&offset("debugger2");
&offset("exception");
&offset("enterDebugger");

&offset("interrupt_inDebugger");
###&offset("traceFlag");

#-- Slot
&offset("RomWrite_BC_A");	# not IO
&offset("RomWrite_DE_A");	# not IO
&offset("RomWrite_mn_HL");	# not IO
&offset("RomWrite_mn_A");	# not IO
&offset("RomWrite_HL_n");	# not IO
&offset("RomWrite_HL_B");	# not IO
&offset("RomWrite_HL_C");	# not IO
&offset("RomWrite_HL_D");	# not IO
&offset("RomWrite_HL_E");	# not IO
&offset("RomWrite_HL_H");	# not IO
&offset("RomWrite_HL_L");	# not IO
&offset("RomWrite_HL_A");	# not IO
&offset("RomWrite_mn_Ixy");	# not IO
&offset("RomWrite_Ixy_n");	# not IO
&offset("RomWrite_Ixy_B");	# not IO
&offset("RomWrite_Ixy_C");	# not IO
&offset("RomWrite_Ixy_D");	# not IO
&offset("RomWrite_Ixy_E");	# not IO
&offset("RomWrite_Ixy_H");	# not IO
&offset("RomWrite_Ixy_L");	# not IO
&offset("RomWrite_Ixy_A");	# not IO
&offset("RomWrite_mn_BC");	# not IO
&offset("RomWrite_mn_DE");	# not IO
&offset("RomWrite_mn_SP");	# not IO

&offsetIO("SegmentsMemMap");
&offsetIO("MemoryMapperTbl");
&offsetIO("IndexMemoryMapper");

&offsetIO("readClockIC");
&offsetIO("ClockIC_memory");

#-- KeyBoard
&offsetIO("MSXKeyTable");
&offsetIO("Debug_KeySense");
&offsetIO("noKeyInt");
&offsetIO("keyboard_LED");
&offsetIO("now_keyboard_LED");


&offsetIO("SetVRAM_512");
&offsetIO("SetVRAM_256");
&offsetIO("SetVRAM_128");
&offsetIO("SetVRAM_32");
&offsetIO("SetVRAM_16");
&offsetIO("SetVRAM_8");
&offsetIO("SetVRAM_4");
&offsetIO("SetVRAM_3");

&offset("initVDP"); # not IO
&offsetIO("TXT1_SetTable");
&offsetIO("TXT2_SetTable");
&offsetIO("MLT_SetTable");
&offsetIO("GRA1_SetTable_withColor");
&offsetIO("GRA1_SetTable");
&offsetIO("GRA2_SetTable");
&offsetIO("GRA3_SetTable");

&offsetIO("TXT1_ScreenInitTbl");
&offsetIO("TXT2_ScreenInitTbl");
&offsetIO("MLT_ScreenInitTbl");
&offsetIO("GRA1_ScreenInitTbl");
&offsetIO("GRA2_ScreenInitTbl");
&offsetIO("GRA3_ScreenInitTbl");
&offsetIO("GRA4_ScreenInitTbl");
&offsetIO("GRA5_ScreenInitTbl");
&offsetIO("GRA6_ScreenInitTbl");
&offsetIO("GRA7_ScreenInitTbl");

&offsetIO("VRAM");

&offsetIO("VDPport1_data");
&offsetIO("VDPport1_dataset");

&offsetIO("VDPpalette_data");
&offsetIO("VDPpalette_dataset");

&offsetIO("CountVDISP");
&offsetIO("cnt_CountVDISP");

&offsetIO("CountUpdate");
&offsetIO("cnt_CountUpdate");

&offsetIO("GRAMadr");

&offsetIO("GRA2_backDropColor");

&offsetIO("VDPreg_0");
&offsetIO("VDPreg_1");
&offsetIO("VDPreg_2");
&offsetIO("VDPreg_3");
&offsetIO("VDPreg_4");
&offsetIO("VDPreg_5");
&offsetIO("VDPreg_6");
&offsetIO("VDPreg_7");
&offsetIO("VDPreg_8");
&offsetIO("VDPreg_9");
&offsetIO("VDPreg_10");
&offsetIO("VDPreg_11");
&offsetIO("VDPreg_12");
&offsetIO("VDPreg_13");
&offsetIO("VDPreg_14");
&offsetIO("VDPreg_15");
&offsetIO("VDPreg_16");
&offsetIO("VDPreg_17");
&offsetIO("VDPreg_18");
&offsetIO("VDPreg_19");
&offsetIO("VDPreg_20");
&offsetIO("VDPreg_21");
&offsetIO("VDPreg_22");
&offsetIO("VDPreg_23");

&offsetIO("VDPreg_32");
&offsetIO("VDPreg_33");
&offsetIO("VDPreg_34");
&offsetIO("VDPreg_35");
&offsetIO("VDPreg_36");
&offsetIO("VDPreg_37");
&offsetIO("VDPreg_38");
&offsetIO("VDPreg_39");
&offsetIO("VDPreg_40");
&offsetIO("VDPreg_41");
&offsetIO("VDPreg_42");
&offsetIO("VDPreg_43");
&offsetIO("VDPreg_44");
&offsetIO("VDPreg_45");
&offsetIO("VDPreg_46");

&offsetIO("VDPcom_SX");
&offsetIO("VDPcom_SY");
&offsetIO("VDPcom_DX");
&offsetIO("VDPcom_DY");
&offsetIO("VDPcom_NX");
&offsetIO("VDPcom_NY");
&offsetIO("VDPcom_CLR");
&offsetIO("VDPcom_ARG");
&offsetIO("VDPcom_CMR");

&offsetIO("StatusReg_0");
&offsetIO("StatusReg_1");
&offsetIO("StatusReg_2");
&offsetIO("StatusReg_3");
&offsetIO("StatusReg_4");
&offsetIO("StatusReg_5");
&offsetIO("StatusReg_6");
&offsetIO("StatusReg_7");
&offsetIO("StatusReg_8");
&offsetIO("StatusReg_9");


&offsetIO("ScreenRefresh");
&offsetIO("PatNameTblUpdate");
&offsetIO("PatGenTblUpdate");
&offsetIO("ColorTblUpdate");

&offsetIO("SprPatGenTblUpdate");
&offsetIO("SprAtrTblUpdate");

&offsetIO("inScrUpdate");

&offsetIO("PatGenTbl");
&offsetIO("PatGenTbl_HL");

&offsetIO("PatNameTbl");
&offsetIO("PatNameTbl_HL");

&offsetIO("ColorTbl");
&offsetIO("ColorTbl_HL");

&offsetIO("SprPatGenTbl");
&offsetIO("SprPatGenTbl_HL");

&offsetIO("SprAtrTbl");
&offsetIO("SprAtrTbl_HL");

&offsetIO("paletteTbl");

# &offsetIO("PatNameUpdateTbl");
&offsetIO("SprPatGenUpdateTbl");
&offsetIO("SprAtrUpdateTbl");
&offsetIO("PatGenUpdateTbl");


close outfile;


#------------------------
# Special Instruction

for ($i = 0; $i < 256; ++$i)
{
	$special_adr[$i] = 0;
}


# setSpecial( num, label )
sub setSpecial
{
	local($num, $l) = ($_[0], "_".$_[1]);
	
	if ($num < 0 || $num > 255)
	{
		printf "setSpecial : illegal number. (%d, %s)\n", $num, $l;
		exit 1;
	}
	
	if (! defined $label{$l})
	{
		printf "Label '%s' : not defined.\n", $l;
		exit 1;
	}
	
	if ($label{$l} - $Ftbl > 65535)
	{
		printf "Label '%s' : illegal relative. (0x%X)\n", $l, $label{$l};
		exit 1;
	}
	
	if ($special_adr[$num] != 0)
	{
		printf "Label '%s' : redefinition. (0x%X)\n", $l, $num;
		exit 1;
	}
	
	$special_adr[$num] = $label{$l} - $Ftbl;
}


#--
&setSpecial(0x00, "Special_RDSLT");

for ($i = 1; $i < 256; ++$i)
{
	&setSpecial($i, "enterDebugger");
}

#--
print "create $outdir/special_adr.dat.\n";
open(spfile, ">$outdir/special_adr.dat")
		|| die "can not create '$outdir/special_adr.dat'.";
binmode spfile;

$dt = pack("S256", @special_adr);

print spfile $dt;

#if (write(spfile, $dt, 256*2) != 256*2)
#{
#	print "write size error.\n";
#	exit 1;
#}
close(spfile);



print "complete.\n";
