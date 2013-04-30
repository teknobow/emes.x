#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 teknobow
#


if (($#ARGV != 1) || (!($ARGV[0] =~ /m680[03]0/)) || (! -d $ARGV[1]))
{
	printf STDERR "io.pl <m68000|m68030> <出力先ディレクトリ>\n";

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



# 以下の値を変更した場合、io.equ も変更が必要
$SIZE_OUT   = 100;
$SIZE_SREG  = 28;
$SIZE_IN    = 34;
$SIZE_VDP   = 92;
$SIZE_TAIL  = 0;



print "pass 1\n";
$comline = "has -w $sw_mpu -s PASS=1 -o $outdir/io.o -x $outdir/io.lab -t . io.has";
print $comline, "\n";
$ret = system($comline);
exit 1 if $ret;


# ラベルファイルから各命令の開始アドレス、終了アドレスを取得
print "read address.\n";
system("cdc 0");
&read_adr("$outdir/io.lab");
system("cdc 1");



sub read_adr
{
	open (lfile, $_[0]) || die "can't open $_[0]";
	while (<lfile>)
	{
		if (/^TX1in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("TX1  IN  %02X\r", $mno);
			
			$startTX1in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX1in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX1in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX1in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX2in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("TX2 IN   %02X\r", $mno);
			
			$startTX2in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX2in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX2in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX2in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^MLTin_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("MLT IN   %02X\r", $mno);
			
			$startMLTin_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startMLTin_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endMLTin_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endMLTin_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR1in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR1 IN   %02X\r", $mno);
			
			$startGR1in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR1in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR1in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR1in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR2in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR2 IN   %02X\r", $mno);
			
			$startGR2in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR2in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR2in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR2in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR3in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR3 IN   %02X\r", $mno);
			
			$startGR3in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR3in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR3in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR3in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR4in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR4 IN   %02X\r", $mno);
			
			$startGR4in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR4in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR4in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR4in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR5in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR5 IN   %02X\r", $mno);
			
			$startGR5in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR5in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR5in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR5in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR6in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR6 IN   %02X\r", $mno);
			
			$startGR6in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR6in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR6in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR6in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR7in_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR7 IN   %02X\r", $mno);
			
			$startGR7in_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR7in_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR7in_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR7in_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX1out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("TX1 OUT  %02X\r", $mno);
			
			$startTX1out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX1out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX1out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX1out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX2out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("TX2 OUT  %02X\r", $mno);
			
			$startTX2out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX2out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX2out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX2out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^MLTout_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("MLT OUT  %02X\r", $mno);
			
			$startMLTout_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startMLTout_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endMLTout_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endMLTout_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR1out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR1 OUT  %02X\r", $mno);
			
			$startGR1out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR1out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR1out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR1out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR2out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR2 OUT  %02X\r", $mno);
			
			$startGR2out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR2out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR2out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR2out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR3out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR3 OUT  %02X\r", $mno);
			
			$startGR3out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR3out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR3out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR3out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR4out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR4 OUT  %02X\r", $mno);
			
			$startGR4out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR4out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR4out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR4out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR5out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR5 OUT  %02X\r", $mno);
			
			$startGR5out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR5out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR5out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR5out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR6out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR6 OUT  %02X\r", $mno);
			
			$startGR6out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR6out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR6out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR6out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR7out_([HL])([se])([0-9A-F]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = hex($3);
			local($adr) = hex($4);
			
			printf("GR7 OUT  %02X\r", $mno);
			
			$startGR7out_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR7out_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR7out_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR7out_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX1vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("TX1 VDP  %02X\r", $mno);
			
			$startTX1vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX1vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX1vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX1vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX2vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("TX2 VDP  %02X\r", $mno);
			
			$startTX2vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX2vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX2vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX2vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^MLTvdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("MLT VDP  %02X\r", $mno);
			
			$startMLTvdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startMLTvdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endMLTvdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endMLTvdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR1vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR1 VDP  %02X\r", $mno);
			
			$startGR1vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR1vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR1vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR1vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR2vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR2 VDP  %02X\r", $mno);
			
			$startGR2vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR2vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR2vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR2vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR3vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR3 VDP  %02X\r", $mno);
			
			$startGR3vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR3vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR3vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR3vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR4vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR4 VDP  %02X\r", $mno);
			
			$startGR4vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR4vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR4vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR4vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR5vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR5 VDP  %02X\r", $mno);
			
			$startGR5vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR5vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR5vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR5vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR6vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR6 VDP  %02X\r", $mno);
			
			$startGR6vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR6vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR6vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR6vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR7vdp_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR7 VDP  %02X\r", $mno);
			
			$startGR7vdp_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR7vdp_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR7vdp_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR7vdp_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX1sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("TX1 SREG %02X\r", $mno);
			
			$startTX1sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX1sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX1sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX1sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^TX2sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("TX2 SREG %02X\r", $mno);
			
			$startTX2sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startTX2sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endTX2sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endTX2sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^MLTsreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("MLT SREG %02X\r", $mno);
			
			$startMLTsreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startMLTsreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endMLTsreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endMLTsreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR1sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR1 SREG %02X\r", $mno);
			
			$startGR1sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR1sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR1sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR1sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR2sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR2 SREG %02X\r", $mno);
			
			$startGR2sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR2sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR2sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR2sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR3sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR3 SREG %02X\r", $mno);
			
			$startGR3sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR3sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR3sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR3sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR4sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR4 SREG %02X\r", $mno);
			
			$startGR4sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR4sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR4sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR4sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR5sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR5 SREG %02X\r", $mno);
			
			$startGR5sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR5sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR5sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR5sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR6sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR6 SREG %02X\r", $mno);
			
			$startGR6sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR6sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR6sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR6sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
		elsif (/^GR7sreg_([HL])([se])([0-9]+) .* ([0-9A-F]+)$/)
		{
			local($mno) = $3;
			local($adr) = hex($4);
			
			printf("GR7 SREG %02X\r", $mno);
			
			$startGR7sreg_H[$mno] = $adr if (($1 eq "H") && ($2 eq "s"));
			$startGR7sreg_L[$mno] = $adr if (($1 eq "L") && ($2 eq "s"));
			
			$endGR7sreg_H[$mno]   = $adr if (($1 eq "H") && ($2 eq "e"));
			$endGR7sreg_L[$mno]   = $adr if (($1 eq "L") && ($2 eq "e"));
		}
	}
	close(lfile);
}



#各命令のサイズを求める
print "check address.\n";
&check_address;

sub check_address
{
	local($nerr) = 0;
	local($i);
	
	$maxTX1in_H = $maxTX1in_L =
	$maxTX2in_H = $maxTX2in_L =
	$maxMLTin_H = $maxMLTin_L =
	$maxGR1in_H = $maxGR1in_L =
	$maxGR2in_H = $maxGR2in_L =
	$maxGR3in_H = $maxGR3in_L =
	$maxGR4in_H = $maxGR4in_L =
	$maxGR5in_H = $maxGR5in_L =
	$maxGR6in_H = $maxGR6in_L =
	$maxGR7in_H = $maxGR7in_L = 0;
	
	$maxTX1out_H = $maxTX1out_L =
	$maxTX2out_H = $maxTX2out_L =
	$maxMLTout_H = $maxMLTout_L =
	$maxGR1out_H = $maxGR1out_L =
	$maxGR2out_H = $maxGR2out_L =
	$maxGR3out_H = $maxGR3out_L =
	$maxGR4out_H = $maxGR4out_L =
	$maxGR5out_H = $maxGR5out_L =
	$maxGR6out_H = $maxGR6out_L =
	$maxGR7out_H = $maxGR7out_L = 0;
	
	$maxTX1vdp_H = $maxTX1vdp_L =
	$maxTX2vdp_H = $maxTX2vdp_L =
	$maxMLTvdp_H = $maxMLTvdp_L =
	$maxGR1vdp_H = $maxGR1vdp_L =
	$maxGR2vdp_H = $maxGR2vdp_L =
	$maxGR3vdp_H = $maxGR3vdp_L =
	$maxGR4vdp_H = $maxGR4vdp_L =
	$maxGR5vdp_H = $maxGR5vdp_L =
	$maxGR6vdp_H = $maxGR6vdp_L =
	$maxGR7vdp_H = $maxGR7vdp_L = 0;
	
	$maxTX1sreg_H = $maxTX1sreg_L =
	$maxTX2sreg_H = $maxTX2sreg_L =
	$maxMLTsreg_H = $maxMLTsreg_L =
	$maxGR1sreg_H = $maxGR1sreg_L =
	$maxGR2sreg_H = $maxGR2sreg_L =
	$maxGR3sreg_H = $maxGR3sreg_L =
	$maxGR4sreg_H = $maxGR4sreg_L =
	$maxGR5sreg_H = $maxGR5sreg_L =
	$maxGR6sreg_H = $maxGR6sreg_L =
	$maxGR7sreg_H = $maxGR7sreg_L = 0;
	
	
	for ($i = 0; $i < 256; ++$i) {
		#-- TX1
		! defined($startTX1in_H[$i])
			&& (printf("TX1_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1in_H[$i])
			&& (printf("TX1_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX1in_L[$i])
			&& (printf("TX1_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1in_L[$i])
			&& (printf("TX1_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startTX1out_H[$i])
			&& (printf("TX1_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1out_H[$i])
			&& (printf("TX1_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX1out_L[$i])
			&& (printf("TX1_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1out_L[$i])
			&& (printf("TX1_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- TX2
		! defined($startTX2in_H[$i])
			&& (printf("TX2_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2in_H[$i])
			&& (printf("TX2_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX2in_L[$i])
			&& (printf("TX2_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2in_L[$i])
			&& (printf("TX2_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startTX2out_H[$i])
			&& (printf("TX2_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2out_H[$i])
			&& (printf("TX2_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX2out_L[$i])
			&& (printf("TX2_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2out_L[$i])
			&& (printf("TX2_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- MLT
		! defined($startMLTin_H[$i])
			&& (printf("MLT_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTin_H[$i])
			&& (printf("MLT_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startMLTin_L[$i])
			&& (printf("MLT_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTin_L[$i])
			&& (printf("MLT_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startMLTout_H[$i])
			&& (printf("MLT_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTout_H[$i])
			&& (printf("MLT_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startMLTout_L[$i])
			&& (printf("MLT_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTout_L[$i])
			&& (printf("MLT_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR1
		! defined($startGR1in_H[$i])
			&& (printf("GR1_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1in_H[$i])
			&& (printf("GR1_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR1in_L[$i])
			&& (printf("GR1_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1in_L[$i])
			&& (printf("GR1_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR1out_H[$i])
			&& (printf("GR1_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1out_H[$i])
			&& (printf("GR1_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR1out_L[$i])
			&& (printf("GR1_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1out_L[$i])
			&& (printf("GR1_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR2
		! defined($startGR2in_H[$i])
			&& (printf("GR2_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2in_H[$i])
			&& (printf("GR2_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR2in_L[$i])
			&& (printf("GR2_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2in_L[$i])
			&& (printf("GR2_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR2out_H[$i])
			&& (printf("GR2_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2out_H[$i])
			&& (printf("GR2_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR2out_L[$i])
			&& (printf("GR2_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2out_L[$i])
			&& (printf("GR2_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR3
		! defined($startGR3in_H[$i])
			&& (printf("GR3_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3in_H[$i])
			&& (printf("GR3_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR3in_L[$i])
			&& (printf("GR3_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3in_L[$i])
			&& (printf("GR3_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR3out_H[$i])
			&& (printf("GR3_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3out_H[$i])
			&& (printf("GR3_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR3out_L[$i])
			&& (printf("GR3_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3out_L[$i])
			&& (printf("GR3_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR4
		! defined($startGR4in_H[$i])
			&& (printf("GR4_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4in_H[$i])
			&& (printf("GR4_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR4in_L[$i])
			&& (printf("GR4_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4in_L[$i])
			&& (printf("GR4_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR4out_H[$i])
			&& (printf("GR4_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4out_H[$i])
			&& (printf("GR4_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR4out_L[$i])
			&& (printf("GR4_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4out_L[$i])
			&& (printf("GR4_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR5
		! defined($startGR5in_H[$i])
			&& (printf("GR5_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5in_H[$i])
			&& (printf("GR5_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR5in_L[$i])
			&& (printf("GR5_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5in_L[$i])
			&& (printf("GR5_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR5out_H[$i])
			&& (printf("GR5_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5out_H[$i])
			&& (printf("GR5_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR5out_L[$i])
			&& (printf("GR5_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5out_L[$i])
			&& (printf("GR5_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR6
		! defined($startGR6in_H[$i])
			&& (printf("GR6_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6in_H[$i])
			&& (printf("GR6_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR6in_L[$i])
			&& (printf("GR6_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6in_L[$i])
			&& (printf("GR6_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR6out_H[$i])
			&& (printf("GR6_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6out_H[$i])
			&& (printf("GR6_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR6out_L[$i])
			&& (printf("GR6_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6out_L[$i])
			&& (printf("GR6_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		#-- GR7
		! defined($startGR7in_H[$i])
			&& (printf("GR7_H IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7in_H[$i])
			&& (printf("GR7_H IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR7in_L[$i])
			&& (printf("GR7_L IN  %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7in_L[$i])
			&& (printf("GR7_L IN  %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR7out_H[$i])
			&& (printf("GR7_H OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7out_H[$i])
			&& (printf("GR7_H OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR7out_L[$i])
			&& (printf("GR7_L OUT %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7out_L[$i])
			&& (printf("GR7_L OUT %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		
		$sizeTX1in_H[$i] = $endTX1in_H[$i] - $startTX1in_H[$i];
		$sizeTX1in_L[$i] = $endTX1in_L[$i] - $startTX1in_L[$i];
		
		$sizeTX2in_H[$i] = $endTX2in_H[$i] - $startTX2in_H[$i];
		$sizeTX2in_L[$i] = $endTX2in_L[$i] - $startTX2in_L[$i];
		
		$sizeMLTin_H[$i] = $endMLTin_H[$i] - $startMLTin_H[$i];
		$sizeMLTin_L[$i] = $endMLTin_L[$i] - $startMLTin_L[$i];
		
		$sizeGR1in_H[$i] = $endGR1in_H[$i] - $startGR1in_H[$i];
		$sizeGR1in_L[$i] = $endGR1in_L[$i] - $startGR1in_L[$i];
		
		$sizeGR2in_H[$i] = $endGR2in_H[$i] - $startGR2in_H[$i];
		$sizeGR2in_L[$i] = $endGR2in_L[$i] - $startGR2in_L[$i];
		
		$sizeGR3in_H[$i] = $endGR3in_H[$i] - $startGR3in_H[$i];
		$sizeGR3in_L[$i] = $endGR3in_L[$i] - $startGR3in_L[$i];
		
		$sizeGR4in_H[$i] = $endGR4in_H[$i] - $startGR4in_H[$i];
		$sizeGR4in_L[$i] = $endGR4in_L[$i] - $startGR4in_L[$i];
		
		$sizeGR5in_H[$i] = $endGR5in_H[$i] - $startGR5in_H[$i];
		$sizeGR5in_L[$i] = $endGR5in_L[$i] - $startGR5in_L[$i];
		
		$sizeGR6in_H[$i] = $endGR6in_H[$i] - $startGR6in_H[$i];
		$sizeGR6in_L[$i] = $endGR6in_L[$i] - $startGR6in_L[$i];
		
		$sizeGR7in_H[$i] = $endGR7in_H[$i] - $startGR7in_H[$i];
		$sizeGR7in_L[$i] = $endGR7in_L[$i] - $startGR7in_L[$i];
		
		
		$sizeTX1out_H[$i] = $endTX1out_H[$i] - $startTX1out_H[$i];
		$sizeTX1out_L[$i] = $endTX1out_L[$i] - $startTX1out_L[$i];
		
		$sizeTX2out_H[$i] = $endTX2out_H[$i] - $startTX2out_H[$i];
		$sizeTX2out_L[$i] = $endTX2out_L[$i] - $startTX2out_L[$i];
		
		$sizeMLTout_H[$i] = $endMLTout_H[$i] - $startMLTout_H[$i];
		$sizeMLTout_L[$i] = $endMLTout_L[$i] - $startMLTout_L[$i];
		
		$sizeGR1out_H[$i] = $endGR1out_H[$i] - $startGR1out_H[$i];
		$sizeGR1out_L[$i] = $endGR1out_L[$i] - $startGR1out_L[$i];
		
		$sizeGR2out_H[$i] = $endGR2out_H[$i] - $startGR2out_H[$i];
		$sizeGR2out_L[$i] = $endGR2out_L[$i] - $startGR2out_L[$i];
		
		$sizeGR3out_H[$i] = $endGR3out_H[$i] - $startGR3out_H[$i];
		$sizeGR3out_L[$i] = $endGR3out_L[$i] - $startGR3out_L[$i];
		
		$sizeGR4out_H[$i] = $endGR4out_H[$i] - $startGR4out_H[$i];
		$sizeGR4out_L[$i] = $endGR4out_L[$i] - $startGR4out_L[$i];
		
		$sizeGR5out_H[$i] = $endGR5out_H[$i] - $startGR5out_H[$i];
		$sizeGR5out_L[$i] = $endGR5out_L[$i] - $startGR5out_L[$i];
		
		$sizeGR6out_H[$i] = $endGR6out_H[$i] - $startGR6out_H[$i];
		$sizeGR6out_L[$i] = $endGR6out_L[$i] - $startGR6out_L[$i];
		
		$sizeGR7out_H[$i] = $endGR7out_H[$i] - $startGR7out_H[$i];
		$sizeGR7out_L[$i] = $endGR7out_L[$i] - $startGR7out_L[$i];
		
		
		if ($maxTX1in_H < $sizeTX1in_H[$i])
			{ $maxTX1in_H = $sizeTX1in_H[$i]; $maxopTX1in_H = $i; }
		
		if ($maxTX1in_L < $sizeTX1in_L[$i])
			{ $maxTX1in_L = $sizeTX1in_L[$i]; $maxopTX1in_L = $i; }
		
		
		if ($maxTX2in_H < $sizeTX2in_H[$i])
			{ $maxTX2in_H = $sizeTX2in_H[$i]; $maxopTX2in_H = $i; }
		
		if ($maxTX2in_L < $sizeTX2in_L[$i])
			{ $maxTX2in_L = $sizeTX2in_L[$i]; $maxopTX2in_L = $i; }
		
		
		if ($maxMLTin_H < $sizeMLTin_H[$i])
			{ $maxMLTin_H = $sizeMLTin_H[$i]; $maxopMLTin_H = $i; }
		
		if ($maxMLTin_L < $sizeMLTin_L[$i])
			{ $maxMLTin_L = $sizeMLTin_L[$i]; $maxopMLTin_L = $i; }
		
		
		if ($maxGR1in_H < $sizeGR1in_H[$i])
			{ $maxGR1in_H = $sizeGR1in_H[$i]; $maxopGR1in_H = $i; }
		
		if ($maxGR1in_L < $sizeGR1in_L[$i])
			{ $maxGR1in_L = $sizeGR1in_L[$i]; $maxopGR1in_L = $i; }
		
		
		if ($maxGR2in_H < $sizeGR2in_H[$i])
			{ $maxGR2in_H = $sizeGR2in_H[$i]; $maxopGR2in_H = $i; }
		
		if ($maxGR2in_L < $sizeGR2in_L[$i])
			{ $maxGR2in_L = $sizeGR2in_L[$i]; $maxopGR2in_L = $i; }
		
		
		if ($maxGR3in_H < $sizeGR3in_H[$i])
			{ $maxGR3in_H = $sizeGR3in_H[$i]; $maxopGR3in_H = $i; }
		
		if ($maxGR3in_L < $sizeGR3in_L[$i])
			{ $maxGR3in_L = $sizeGR3in_L[$i]; $maxopGR3in_L = $i; }
		
		
		if ($maxGR4in_H < $sizeGR4in_H[$i])
			{ $maxGR4in_H = $sizeGR4in_H[$i]; $maxopGR4in_H = $i; }
		
		if ($maxGR4in_L < $sizeGR4in_L[$i])
			{ $maxGR4in_L = $sizeGR4in_L[$i]; $maxopGR4in_L = $i; }
		
		
		if ($maxGR5in_H < $sizeGR5in_H[$i])
			{ $maxGR5in_H = $sizeGR5in_H[$i]; $maxopGR5in_H = $i; }
		
		if ($maxGR5in_L < $sizeGR5in_L[$i])
			{ $maxGR5in_L = $sizeGR5in_L[$i]; $maxopGR5in_L = $i; }
		
		
		if ($maxGR6in_H < $sizeGR6in_H[$i])
			{ $maxGR6in_H = $sizeGR6in_H[$i]; $maxopGR6in_H = $i; }
		
		if ($maxGR6in_L < $sizeGR6in_L[$i])
			{ $maxGR6in_L = $sizeGR6in_L[$i]; $maxopGR6in_L = $i; }
		
		
		if ($maxGR7in_H < $sizeGR7in_H[$i])
			{ $maxGR7in_H = $sizeGR7in_H[$i]; $maxopGR7in_H = $i; }
		
		if ($maxGR7in_L < $sizeGR7in_L[$i])
			{ $maxGR7in_L = $sizeGR7in_L[$i]; $maxopGR7in_L = $i; }
		
		
		
		if ($maxTX1out_H < $sizeTX1out_H[$i])
			{ $maxTX1out_H = $sizeTX1out_H[$i]; $maxopTX1out_H = $i; }
		
		if ($maxTX1out_L < $sizeTX1out_L[$i])
			{ $maxTX1out_L = $sizeTX1out_L[$i]; $maxopTX1out_L = $i; }
		
		
		if ($maxTX2out_H < $sizeTX2out_H[$i])
			{ $maxTX2out_H = $sizeTX2out_H[$i]; $maxopTX2out_H = $i; }
		
		if ($maxTX2out_L < $sizeTX2out_L[$i])
			{ $maxTX2out_L = $sizeTX2out_L[$i]; $maxopTX2out_L = $i; }
		
		
		if ($maxMLTout_H < $sizeMLTout_H[$i])
			{ $maxMLTout_H = $sizeMLTout_H[$i]; $maxopMLTout_H = $i; }
		
		if ($maxMLTout_L < $sizeMLTout_L[$i])
			{ $maxMLTout_L = $sizeMLTout_L[$i]; $maxopMLTout_L = $i; }
		
		
		if ($maxGR1out_H < $sizeGR1out_H[$i])
			{ $maxGR1out_H = $sizeGR1out_H[$i]; $maxopGR1out_H = $i; }
		
		if ($maxGR1out_L < $sizeGR1out_L[$i])
			{ $maxGR1out_L = $sizeGR1out_L[$i]; $maxopGR1out_L = $i; }
		
		
		if ($maxGR2out_H < $sizeGR2out_H[$i])
			{ $maxGR2out_H = $sizeGR2out_H[$i]; $maxopGR2out_H = $i; }
		
		if ($maxGR2out_L < $sizeGR2out_L[$i])
			{ $maxGR2out_L = $sizeGR2out_L[$i]; $maxopGR2out_L = $i; }
		
		
		if ($maxGR3out_H < $sizeGR3out_H[$i])
			{ $maxGR3out_H = $sizeGR3out_H[$i]; $maxopGR3out_H = $i; }
		
		if ($maxGR3out_L < $sizeGR3out_L[$i])
			{ $maxGR3out_L = $sizeGR3out_L[$i]; $maxopGR3out_L = $i; }
		
		
		if ($maxGR4out_H < $sizeGR4out_H[$i])
			{ $maxGR4out_H = $sizeGR4out_H[$i]; $maxopGR4out_H = $i; }
		
		if ($maxGR4out_L < $sizeGR4out_L[$i])
			{ $maxGR4out_L = $sizeGR4out_L[$i]; $maxopGR4out_L = $i; }
		
		
		if ($maxGR5out_H < $sizeGR5out_H[$i])
			{ $maxGR5out_H = $sizeGR5out_H[$i]; $maxopGR5out_H = $i; }
		
		if ($maxGR5out_L < $sizeGR5out_L[$i])
			{ $maxGR5out_L = $sizeGR5out_L[$i]; $maxopGR5out_L = $i; }
		
		
		if ($maxGR6out_H < $sizeGR6out_H[$i])
			{ $maxGR6out_H = $sizeGR6out_H[$i]; $maxopGR6out_H = $i; }
		
		if ($maxGR6out_L < $sizeGR6out_L[$i])
			{ $maxGR6out_L = $sizeGR6out_L[$i]; $maxopGR6out_L = $i; }
		
		
		if ($maxGR7out_H < $sizeGR7out_H[$i])
			{ $maxGR7out_H = $sizeGR7out_H[$i]; $maxopGR7out_H = $i; }
		
		if ($maxGR7out_L < $sizeGR7out_L[$i])
			{ $maxGR7out_L = $sizeGR7out_L[$i]; $maxopGR7out_L = $i; }
	}
	
	for ($i = 0; $i < 64; ++$i)
	{
		! defined($startTX1vdp_H[$i])
			&& (printf("TX1_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1vdp_H[$i])
			&& (printf("TX1_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX1vdp_L[$i])
			&& (printf("TX1_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1vdp_L[$i])
			&& (printf("TX1_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startTX2vdp_H[$i])
			&& (printf("TX2_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2vdp_H[$i])
			&& (printf("TX2_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX2vdp_L[$i])
			&& (printf("TX2_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2vdp_L[$i])
			&& (printf("TX2_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startMLTvdp_H[$i])
			&& (printf("MLT_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTvdp_H[$i])
			&& (printf("MLT_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startMLTvdp_L[$i])
			&& (printf("MLT_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTvdp_L[$i])
			&& (printf("MLT_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR1vdp_H[$i])
			&& (printf("GR1_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1vdp_H[$i])
			&& (printf("GR1_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR1vdp_L[$i])
			&& (printf("GR1_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1vdp_L[$i])
			&& (printf("GR1_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR2vdp_H[$i])
			&& (printf("GR2_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2vdp_H[$i])
			&& (printf("GR2_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR2vdp_L[$i])
			&& (printf("GR2_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2vdp_L[$i])
			&& (printf("GR2_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR3vdp_H[$i])
			&& (printf("GR3_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3vdp_H[$i])
			&& (printf("GR3_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR3vdp_L[$i])
			&& (printf("GR3_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3vdp_L[$i])
			&& (printf("GR3_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR4vdp_H[$i])
			&& (printf("GR4_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4vdp_H[$i])
			&& (printf("GR4_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR4vdp_L[$i])
			&& (printf("GR4_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4vdp_L[$i])
			&& (printf("GR4_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR5vdp_H[$i])
			&& (printf("GR5_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5vdp_H[$i])
			&& (printf("GR5_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR5vdp_L[$i])
			&& (printf("GR5_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5vdp_L[$i])
			&& (printf("GR5_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR6vdp_H[$i])
			&& (printf("GR6_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6vdp_H[$i])
			&& (printf("GR6_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR6vdp_L[$i])
			&& (printf("GR6_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6vdp_L[$i])
			&& (printf("GR6_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR7vdp_H[$i])
			&& (printf("GR7_H VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7vdp_H[$i])
			&& (printf("GR7_H VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR7vdp_L[$i])
			&& (printf("GR7_L VDP %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7vdp_L[$i])
			&& (printf("GR7_L VDP %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		$sizeTX1vdp_H[$i] = $endTX1vdp_H[$i] - $startTX1vdp_H[$i];
		$sizeTX1vdp_L[$i] = $endTX1vdp_L[$i] - $startTX1vdp_L[$i];
		
		$sizeTX2vdp_H[$i] = $endTX2vdp_H[$i] - $startTX2vdp_H[$i];
		$sizeTX2vdp_L[$i] = $endTX2vdp_L[$i] - $startTX2vdp_L[$i];
		
		$sizeMLTvdp_H[$i] = $endMLTvdp_H[$i] - $startMLTvdp_H[$i];
		$sizeMLTvdp_L[$i] = $endMLTvdp_L[$i] - $startMLTvdp_L[$i];
		
		$sizeGR1vdp_H[$i] = $endGR1vdp_H[$i] - $startGR1vdp_H[$i];
		$sizeGR1vdp_L[$i] = $endGR1vdp_L[$i] - $startGR1vdp_L[$i];
		
		$sizeGR2vdp_H[$i] = $endGR2vdp_H[$i] - $startGR2vdp_H[$i];
		$sizeGR2vdp_L[$i] = $endGR2vdp_L[$i] - $startGR2vdp_L[$i];
		
		$sizeGR3vdp_H[$i] = $endGR3vdp_H[$i] - $startGR3vdp_H[$i];
		$sizeGR3vdp_L[$i] = $endGR3vdp_L[$i] - $startGR3vdp_L[$i];
		
		$sizeGR4vdp_H[$i] = $endGR4vdp_H[$i] - $startGR4vdp_H[$i];
		$sizeGR4vdp_L[$i] = $endGR4vdp_L[$i] - $startGR4vdp_L[$i];
		
		$sizeGR5vdp_H[$i] = $endGR5vdp_H[$i] - $startGR5vdp_H[$i];
		$sizeGR5vdp_L[$i] = $endGR5vdp_L[$i] - $startGR5vdp_L[$i];
		
		$sizeGR6vdp_H[$i] = $endGR6vdp_H[$i] - $startGR6vdp_H[$i];
		$sizeGR6vdp_L[$i] = $endGR6vdp_L[$i] - $startGR6vdp_L[$i];
		
		$sizeGR7vdp_H[$i] = $endGR7vdp_H[$i] - $startGR7vdp_H[$i];
		$sizeGR7vdp_L[$i] = $endGR7vdp_L[$i] - $startGR7vdp_L[$i];
		
		
		if ($maxTX1vdp_H < $sizeTX1vdp_H[$i])
			{ $maxTX1vdp_H = $sizeTX1vdp_H[$i]; $maxopTX1vdp_H = $i; }
		
		if ($maxTX1vdp_L < $sizeTX1vdp_L[$i])
			{ $maxTX1vdp_L = $sizeTX1vdp_L[$i]; $maxopTX1vdp_L = $i; }
		
		
		if ($maxTX2vdp_H < $sizeTX2vdp_H[$i])
			{ $maxTX2vdp_H = $sizeTX2vdp_H[$i]; $maxopTX2vdp_H = $i; }
		
		if ($maxTX2vdp_L < $sizeTX2vdp_L[$i])
			{ $maxTX2vdp_L = $sizeTX2vdp_L[$i]; $maxopTX2vdp_L = $i; }
		
		
		if ($maxMLTvdp_H < $sizeMLTvdp_H[$i])
			{ $maxMLTvdp_H = $sizeMLTvdp_H[$i]; $maxopMLTvdp_H = $i; }
		
		if ($maxMLTvdp_L < $sizeMLTvdp_L[$i])
			{ $maxMLTvdp_L = $sizeMLTvdp_L[$i]; $maxopMLTvdp_L = $i; }
		
		
		if ($maxGR1vdp_H < $sizeGR1vdp_H[$i])
			{ $maxGR1vdp_H = $sizeGR1vdp_H[$i]; $maxopGR1vdp_H = $i; }
		
		if ($maxGR1vdp_L < $sizeGR1vdp_L[$i])
			{ $maxGR1vdp_L = $sizeGR1vdp_L[$i]; $maxopGR1vdp_L = $i; }
		
		
		if ($maxGR2vdp_H < $sizeGR2vdp_H[$i])
			{ $maxGR2vdp_H = $sizeGR2vdp_H[$i]; $maxopGR2vdp_H = $i; }
		
		if ($maxGR2vdp_L < $sizeGR2vdp_L[$i])
			{ $maxGR2vdp_L = $sizeGR2vdp_L[$i]; $maxopGR2vdp_L = $i; }
		
		
		if ($maxGR3vdp_H < $sizeGR3vdp_H[$i])
			{ $maxGR3vdp_H = $sizeGR3vdp_H[$i]; $maxopGR3vdp_H = $i; }
		
		if ($maxGR3vdp_L < $sizeGR3vdp_L[$i])
			{ $maxGR3vdp_L = $sizeGR3vdp_L[$i]; $maxopGR3vdp_L = $i; }
		
		
		if ($maxGR4vdp_H < $sizeGR4vdp_H[$i])
			{ $maxGR4vdp_H = $sizeGR4vdp_H[$i]; $maxopGR4vdp_H = $i; }
		
		if ($maxGR4vdp_L < $sizeGR4vdp_L[$i])
			{ $maxGR4vdp_L = $sizeGR4vdp_L[$i]; $maxopGR4vdp_L = $i; }
		
		
		if ($maxGR5vdp_H < $sizeGR5vdp_H[$i])
			{ $maxGR5vdp_H = $sizeGR5vdp_H[$i]; $maxopGR5vdp_H = $i; }
		
		if ($maxGR5vdp_L < $sizeGR5vdp_L[$i])
			{ $maxGR5vdp_L = $sizeGR5vdp_L[$i]; $maxopGR5vdp_L = $i; }
		
		
		if ($maxGR6vdp_H < $sizeGR6vdp_H[$i])
			{ $maxGR6vdp_H = $sizeGR6vdp_H[$i]; $maxopGR6vdp_H = $i; }
		
		if ($maxGR6vdp_L < $sizeGR6vdp_L[$i])
			{ $maxGR6vdp_L = $sizeGR6vdp_L[$i]; $maxopGR6vdp_L = $i; }
		
		
		if ($maxGR7vdp_H < $sizeGR7vdp_H[$i])
			{ $maxGR7vdp_H = $sizeGR7vdp_H[$i]; $maxopGR7vdp_H = $i; }
		
		if ($maxGR7vdp_L < $sizeGR7vdp_L[$i])
			{ $maxGR7vdp_L = $sizeGR7vdp_L[$i]; $maxopGR7vdp_L = $i; }
	}
	
	for ($i = 0; $i < 16; ++$i)
	{
		! defined($startTX1sreg_H[$i])
			&& (printf("TX1_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1sreg_H[$i])
			&& (printf("TX1_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX1sreg_L[$i])
			&& (printf("TX1_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX1sreg_L[$i])
			&& (printf("TX1_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startTX2sreg_H[$i])
			&& (printf("TX2_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2sreg_H[$i])
			&& (printf("TX2_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startTX2sreg_L[$i])
			&& (printf("TX2_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endTX2sreg_L[$i])
			&& (printf("TX2_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startMLTsreg_H[$i])
			&& (printf("MLT_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTsreg_H[$i])
			&& (printf("MLT_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startMLTsreg_L[$i])
			&& (printf("MLT_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endMLTsreg_L[$i])
			&& (printf("MLT_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR1sreg_H[$i])
			&& (printf("GR1_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1sreg_H[$i])
			&& (printf("GR1_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR1sreg_L[$i])
			&& (printf("GR1_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR1sreg_L[$i])
			&& (printf("GR1_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR2sreg_H[$i])
			&& (printf("GR2_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2sreg_H[$i])
			&& (printf("GR2_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR2sreg_L[$i])
			&& (printf("GR2_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR2sreg_L[$i])
			&& (printf("GR2_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR3sreg_H[$i])
			&& (printf("GR3_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3sreg_H[$i])
			&& (printf("GR3_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR3sreg_L[$i])
			&& (printf("GR3_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR3sreg_L[$i])
			&& (printf("GR3_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR4sreg_H[$i])
			&& (printf("GR4_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4sreg_H[$i])
			&& (printf("GR4_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR4sreg_L[$i])
			&& (printf("GR4_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR4sreg_L[$i])
			&& (printf("GR4_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR5sreg_H[$i])
			&& (printf("GR5_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5sreg_H[$i])
			&& (printf("GR5_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR5sreg_L[$i])
			&& (printf("GR5_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR5sreg_L[$i])
			&& (printf("GR5_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR6sreg_H[$i])
			&& (printf("GR6_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6sreg_H[$i])
			&& (printf("GR6_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR6sreg_L[$i])
			&& (printf("GR6_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR6sreg_L[$i])
			&& (printf("GR6_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		! defined($startGR7sreg_H[$i])
			&& (printf("GR7_H SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7sreg_H[$i])
			&& (printf("GR7_H SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		! defined($startGR7sreg_L[$i])
			&& (printf("GR7_L SREG %02X : start address nothing.\n", $i), ++$nerr);
		! defined($endGR7sreg_L[$i])
			&& (printf("GR7_L SREG %02X : end address nothing.\n", $i),   ++$nerr);
		
		
		
		$sizeTX1sreg_H[$i] = $endTX1sreg_H[$i] - $startTX1sreg_H[$i];
		$sizeTX1sreg_L[$i] = $endTX1sreg_L[$i] - $startTX1sreg_L[$i];
		
		$sizeTX2sreg_H[$i] = $endTX2sreg_H[$i] - $startTX2sreg_H[$i];
		$sizeTX2sreg_L[$i] = $endTX2sreg_L[$i] - $startTX2sreg_L[$i];
		
		$sizeMLTsreg_H[$i] = $endMLTsreg_H[$i] - $startMLTsreg_H[$i];
		$sizeMLTsreg_L[$i] = $endMLTsreg_L[$i] - $startMLTsreg_L[$i];
		
		$sizeGR1sreg_H[$i] = $endGR1sreg_H[$i] - $startGR1sreg_H[$i];
		$sizeGR1sreg_L[$i] = $endGR1sreg_L[$i] - $startGR1sreg_L[$i];
		
		$sizeGR2sreg_H[$i] = $endGR2sreg_H[$i] - $startGR2sreg_H[$i];
		$sizeGR2sreg_L[$i] = $endGR2sreg_L[$i] - $startGR2sreg_L[$i];
		
		$sizeGR3sreg_H[$i] = $endGR3sreg_H[$i] - $startGR3sreg_H[$i];
		$sizeGR3sreg_L[$i] = $endGR3sreg_L[$i] - $startGR3sreg_L[$i];
		
		$sizeGR4sreg_H[$i] = $endGR4sreg_H[$i] - $startGR4sreg_H[$i];
		$sizeGR4sreg_L[$i] = $endGR4sreg_L[$i] - $startGR4sreg_L[$i];
		
		$sizeGR5sreg_H[$i] = $endGR5sreg_H[$i] - $startGR5sreg_H[$i];
		$sizeGR5sreg_L[$i] = $endGR5sreg_L[$i] - $startGR5sreg_L[$i];
		
		$sizeGR6sreg_H[$i] = $endGR6sreg_H[$i] - $startGR6sreg_H[$i];
		$sizeGR6sreg_L[$i] = $endGR6sreg_L[$i] - $startGR6sreg_L[$i];
		
		$sizeGR7sreg_H[$i] = $endGR7sreg_H[$i] - $startGR7sreg_H[$i];
		$sizeGR7sreg_L[$i] = $endGR7sreg_L[$i] - $startGR7sreg_L[$i];
		
		
		
		if ($maxTX1sreg_H < $sizeTX1sreg_H[$i])
			{ $maxTX1sreg_H = $sizeTX1sreg_H[$i]; $maxopTX1sreg_H = $i; }
		
		if ($maxTX1sreg_L < $sizeTX1sreg_L[$i])
			{ $maxTX1sreg_L = $sizeTX1sreg_L[$i]; $maxopTX1sreg_L = $i; }
		
		
		if ($maxTX2sreg_H < $sizeTX2sreg_H[$i])
			{ $maxTX2sreg_H = $sizeTX2sreg_H[$i]; $maxopTX2sreg_H = $i; }
		
		if ($maxTX2sreg_L < $sizeTX2sreg_L[$i])
			{ $maxTX2sreg_L = $sizeTX2sreg_L[$i]; $maxopTX2sreg_L = $i; }
		
		
		if ($maxMLTsreg_H < $sizeMLTsreg_H[$i])
			{ $maxMLTsreg_H = $sizeMLTsreg_H[$i]; $maxopMLTsreg_H = $i; }
		
		if ($maxMLTsreg_L < $sizeMLTsreg_L[$i])
			{ $maxMLTsreg_L = $sizeMLTsreg_L[$i]; $maxopMLTsreg_L = $i; }
		
		
		if ($maxGR1sreg_H < $sizeGR1sreg_H[$i])
			{ $maxGR1sreg_H = $sizeGR1sreg_H[$i]; $maxopGR1sreg_H = $i; }
		
		if ($maxGR1sreg_L < $sizeGR1sreg_L[$i])
			{ $maxGR1sreg_L = $sizeGR1sreg_L[$i]; $maxopGR1sreg_L = $i; }
		
		
		if ($maxGR2sreg_H < $sizeGR2sreg_H[$i])
			{ $maxGR2sreg_H = $sizeGR2sreg_H[$i]; $maxopGR2sreg_H = $i; }
		
		if ($maxGR2sreg_L < $sizeGR2sreg_L[$i])
			{ $maxGR2sreg_L = $sizeGR2sreg_L[$i]; $maxopGR2sreg_L = $i; }
		
		
		if ($maxGR3sreg_H < $sizeGR3sreg_H[$i])
			{ $maxGR3sreg_H = $sizeGR3sreg_H[$i]; $maxopGR3sreg_H = $i; }
		
		if ($maxGR3sreg_L < $sizeGR3sreg_L[$i])
			{ $maxGR3sreg_L = $sizeGR3sreg_L[$i]; $maxopGR3sreg_L = $i; }
		
		
		if ($maxGR4sreg_H < $sizeGR4sreg_H[$i])
			{ $maxGR4sreg_H = $sizeGR4sreg_H[$i]; $maxopGR4sreg_H = $i; }
		
		if ($maxGR4sreg_L < $sizeGR4sreg_L[$i])
			{ $maxGR4sreg_L = $sizeGR4sreg_L[$i]; $maxopGR4sreg_L = $i; }
		
		
		if ($maxGR5sreg_H < $sizeGR5sreg_H[$i])
			{ $maxGR5sreg_H = $sizeGR5sreg_H[$i]; $maxopGR5sreg_H = $i; }
		
		if ($maxGR5sreg_L < $sizeGR5sreg_L[$i])
			{ $maxGR5sreg_L = $sizeGR5sreg_L[$i]; $maxopGR5sreg_L = $i; }
		
		
		if ($maxGR6sreg_H < $sizeGR6sreg_H[$i])
			{ $maxGR6sreg_H = $sizeGR6sreg_H[$i]; $maxopGR6sreg_H = $i; }
		
		if ($maxGR6sreg_L < $sizeGR6sreg_L[$i])
			{ $maxGR6sreg_L = $sizeGR6sreg_L[$i]; $maxopGR6sreg_L = $i; }
		
		
		if ($maxGR7sreg_H < $sizeGR7sreg_H[$i])
			{ $maxGR7sreg_H = $sizeGR7sreg_H[$i]; $maxopGR7sreg_H = $i; }
		
		if ($maxGR7sreg_L < $sizeGR7sreg_L[$i])
			{ $maxGR7sreg_L = $sizeGR7sreg_L[$i]; $maxopGR7sreg_L = $i; }
	}
	
	
	exit 1 if $nerr;
}


printf("TX1_H : IN %3d bytes [%02X], ", $maxTX1in_H,  $maxopTX1in_H);
printf("OUT %3d bytes [%02X], ", $maxTX1out_H, $maxopTX1out_H);
printf("VDP %3d bytes [%2d], ", $maxTX1vdp_H, $maxopTX1vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxTX1sreg_H, $maxopTX1sreg_H);

printf("TX1_L : IN %3d bytes [%02X], ", $maxTX1in_L,  $maxopTX1in_L);
printf("OUT %3d bytes [%02X], ", $maxTX1out_L, $maxopTX1out_L);
printf("VDP %3d bytes [%2d], ", $maxTX1vdp_L, $maxopTX1vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxTX1sreg_L, $maxopTX1sreg_L);


printf("TX2_H : IN %3d bytes [%02X], ", $maxTX2in_H,  $maxopTX2in_H);
printf("OUT %3d bytes [%02X], ", $maxTX2out_H, $maxopTX2out_H);
printf("VDP %3d bytes [%2d], ", $maxTX2vdp_H, $maxopTX2vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxTX2sreg_H, $maxopTX2sreg_H);

printf("TX2_L : IN %3d bytes [%02X], ", $maxTX2in_L,  $maxopTX2in_L);
printf("OUT %3d bytes [%02X], ", $maxTX2out_L, $maxopTX2out_L);
printf("VDP %3d bytes [%2d], ", $maxTX2vdp_L, $maxopTX2vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxTX2sreg_L, $maxopTX2sreg_L);


printf("MLT_H : IN %3d bytes [%02X], ", $maxMLTin_H,  $maxopMLTin_H);
printf("OUT %3d bytes [%02X], ", $maxMLTout_H, $maxopMLTout_H);
printf("VDP %3d bytes [%2d], ", $maxMLTvdp_H, $maxopMLTvdp_H);
printf("SREG %3d bytes [%2d]\n", $maxMLTsreg_H, $maxopMLTsreg_H);

printf("MLT_L : IN %3d bytes [%02X], ", $maxMLTin_L,  $maxopMLTin_L);
printf("OUT %3d bytes [%02X], ", $maxMLTout_L, $maxopMLTout_L);
printf("VDP %3d bytes [%2d], ", $maxMLTvdp_L, $maxopMLTvdp_L);
printf("SREG %3d bytes [%2d]\n", $maxMLTsreg_L, $maxopMLTsreg_L);


printf("GR1_H : IN %3d bytes [%02X], ", $maxGR1in_H,  $maxopGR1in_H);
printf("OUT %3d bytes [%02X], ", $maxGR1out_H, $maxopGR1out_H);
printf("VDP %3d bytes [%2d], ", $maxGR1vdp_H, $maxopGR1vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR1sreg_H, $maxopGR1sreg_H);

printf("GR1_L : IN %3d bytes [%02X], ", $maxGR1in_L,  $maxopGR1in_L);
printf("OUT %3d bytes [%02X], ", $maxGR1out_L, $maxopGR1out_L);
printf("VDP %3d bytes [%2d], ", $maxGR1vdp_L, $maxopGR1vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR1sreg_L, $maxopGR1sreg_L);


printf("GR2_H : IN %3d bytes [%02X], ", $maxGR2in_H,  $maxopGR2in_H);
printf("OUT %3d bytes [%02X], ", $maxGR2out_H, $maxopGR2out_H);
printf("VDP %3d bytes [%2d], ", $maxGR2vdp_H, $maxopGR2vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR2sreg_H, $maxopGR2sreg_H);

printf("GR2_L : IN %3d bytes [%02X], ", $maxGR2in_L,  $maxopGR2in_L);
printf("OUT %3d bytes [%02X], ", $maxGR2out_L, $maxopGR2out_L);
printf("VDP %3d bytes [%2d], ", $maxGR2vdp_L, $maxopGR2vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR2sreg_L, $maxopGR2sreg_L);


printf("GR3_H : IN %3d bytes [%02X], ", $maxGR3in_H,  $maxopGR3in_H);
printf("OUT %3d bytes [%02X], ", $maxGR3out_H, $maxopGR3out_H);
printf("VDP %3d bytes [%2d], ", $maxGR3vdp_H, $maxopGR3vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR3sreg_H, $maxopGR3sreg_H);

printf("GR3_L : IN %3d bytes [%02X], ", $maxGR3in_L,  $maxopGR3in_L);
printf("OUT %3d bytes [%02X], ", $maxGR3out_L, $maxopGR3out_L);
printf("VDP %3d bytes [%2d], ", $maxGR3vdp_L, $maxopGR3vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR3sreg_L, $maxopGR3sreg_L);


printf("GR4_H : IN %3d bytes [%02X], ", $maxGR4in_H,  $maxopGR4in_H);
printf("OUT %3d bytes [%02X], ", $maxGR4out_H, $maxopGR4out_H);
printf("VDP %3d bytes [%2d], ", $maxGR4vdp_H, $maxopGR4vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR4sreg_H, $maxopGR4sreg_H);

printf("GR4_L : IN %3d bytes [%02X], ", $maxGR4in_L,  $maxopGR4in_L);
printf("OUT %3d bytes [%02X], ", $maxGR4out_L, $maxopGR4out_L);
printf("VDP %3d bytes [%2d], ", $maxGR4vdp_L, $maxopGR4vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR4sreg_L, $maxopGR4sreg_L);


printf("GR5_H : IN %3d bytes [%02X], ", $maxGR5in_H,  $maxopGR5in_H);
printf("OUT %3d bytes [%02X], ", $maxGR5out_H, $maxopGR5out_H);
printf("VDP %3d bytes [%2d], ", $maxGR5vdp_H, $maxopGR5vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR5sreg_H, $maxopGR5sreg_H);

printf("GR5_L : IN %3d bytes [%02X], ", $maxGR5in_L,  $maxopGR5in_L);
printf("OUT %3d bytes [%02X], ", $maxGR5out_L, $maxopGR5out_L);
printf("VDP %3d bytes [%2d], ", $maxGR5vdp_L, $maxopGR5vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR5sreg_L, $maxopGR5sreg_L);


printf("GR6_H : IN %3d bytes [%02X], ", $maxGR6in_H,  $maxopGR6in_H);
printf("OUT %3d bytes [%02X], ", $maxGR6out_H, $maxopGR6out_H);
printf("VDP %3d bytes [%2d], ", $maxGR6vdp_H, $maxopGR6vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR6sreg_H, $maxopGR6sreg_H);

printf("GR6_L : IN %3d bytes [%02X], ", $maxGR6in_L,  $maxopGR6in_L);
printf("OUT %3d bytes [%02X], ", $maxGR6out_L, $maxopGR6out_L);
printf("VDP %3d bytes [%2d], ", $maxGR6vdp_L, $maxopGR6vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR6sreg_L, $maxopGR6sreg_L);


printf("GR7_H : IN %3d bytes [%02X], ", $maxGR7in_H,  $maxopGR7in_H);
printf("OUT %3d bytes [%02X], ", $maxGR7out_H, $maxopGR7out_H);
printf("VDP %3d bytes [%2d], ", $maxGR7vdp_H, $maxopGR7vdp_H);
printf("SREG %3d bytes [%2d]\n", $maxGR7sreg_H, $maxopGR7sreg_H);

printf("GR7_L : IN %3d bytes [%02X], ", $maxGR7in_L,  $maxopGR7in_L);
printf("OUT %3d bytes [%02X], ", $maxGR7out_L, $maxopGR7out_L);
printf("VDP %3d bytes [%2d], ", $maxGR7vdp_L, $maxopGR7vdp_L);
printf("SREG %3d bytes [%2d]\n", $maxGR7sreg_L, $maxopGR7sreg_L);


sub max
{
	(sort {-($a <=> $b); } @_)[0];
}

$maxIN   = &max($maxTX1in_H,   $maxTX2in_H,   $maxMLTin_H,   $maxGR1in_H,   $maxGR2in_H,
		$maxGR3in_H,   $maxGR4in_H,   $maxGR5in_H,   $maxGR6in_H,   $maxGR7in_H,
		$maxTX1in_L,   $maxTX2in_L,   $maxMLTin_L,   $maxGR1in_L,   $maxGR2in_L,
		$maxGR3in_L,   $maxGR4in_L,   $maxGR5in_L,   $maxGR6in_L,   $maxGR7in_L);

$maxOUT  = &max($maxTX1out_H,  $maxTX2out_H,  $maxMLTout_H,  $maxGR1out_H,  $maxGR2out_H,
		$maxGR3out_H,  $maxGR4out_H,  $maxGR5out_H,  $maxGR6out_H,  $maxGR7out_H,
		$maxTX1out_L,  $maxTX2out_L,  $maxMLTout_L,  $maxGR1out_L,  $maxGR2out_L,
		$maxGR3out_L,  $maxGR4out_L,  $maxGR5out_L,  $maxGR6out_L,  $maxGR7out_L);

$maxVDP  = &max($maxTX1vdp_H,  $maxTX2vdp_H,  $maxMLTvdp_H,  $maxGR1vdp_H,  $maxGR2vdp_H,
		$maxGR3vdp_H,  $maxGR4vdp_H,  $maxGR5vdp_H,  $maxGR6vdp_H,  $maxGR7vdp_H,
		$maxTX1vdp_L,  $maxTX2vdp_L,  $maxMLTvdp_L,  $maxGR1vdp_L,  $maxGR2vdp_L,
		$maxGR3vdp_L,  $maxGR4vdp_L,  $maxGR5vdp_L,  $maxGR6vdp_L,  $maxGR7vdp_L);

$maxSREG = &max($maxTX1sreg_H, $maxTX2sreg_H, $maxMLTsreg_H, $maxGR1sreg_H, $maxGR2sreg_H,
		$maxGR3sreg_H, $maxGR4sreg_H, $maxGR5sreg_H, $maxGR6sreg_H, $maxGR7sreg_H,
		$maxTX1sreg_L, $maxTX2sreg_L, $maxMLTsreg_L, $maxGR1sreg_L, $maxGR2sreg_L,
		$maxGR3sreg_L, $maxGR4sreg_L, $maxGR5sreg_L, $maxGR6sreg_L, $maxGR7sreg_L);


printf "MAX : IN = %3d bytes, OUT = %3d bytes, VDP = %3d bytes, SREG = %3d bytes\n",
		$maxIN, $maxOUT, $maxVDP, $maxSREG;

printf "total = %3d bytes.\n", $maxIN + $maxOUT + $maxVDP + $maxSREG;


if ($maxIN   > $SIZE_IN)   { printf "SIZE_IN error (> %d).\n",   $SIZE_IN;    exit 1; }
if ($maxOUT  > $SIZE_OUT)  { printf "SIZE_OUT error (> %d).\n",  $SIZE_OUT;   exit 1; }
if ($maxVDP  > $SIZE_VDP)  { printf "SIZE_VDP error (> %d).\n",  $SIZE_VDP;   exit 1; }
if ($maxSREG > $SIZE_SREG) { printf "SIZE_SREG error (> %d).\n", $SIZE_SREG;  exit 1; }

if ($maxIN   < $SIZE_IN)   { print "warning : maxIN ($maxIN) < SIZE_IN ($SIZE_IN).\n";        }
if ($maxOUT  < $SIZE_OUT)  { print "warning : maxOUT ($maxOUT) < SIZE_OUT ($SIZE_OUT).\n";    }
if ($maxVDP  < $SIZE_VDP)  { print "warning : maxVDP ($maxVDP) < SIZE_VDP ($SIZE_VDP).\n";    }
if ($maxSREG < $SIZE_SREG) { print "warning : maxSREG ($maxSREG) < SIZE_SREG ($SIZE_SREG).\n";}



#----------------------------------------------------------------
print "\npass 2\n";

print "read instructions.\n";
system("cdc 0");
open (mfile, "io.inc") || die "can't open 'io.inc'";
while (<mfile>)
{
	next if (/^[ \t]*\*/);
	
	if (/^[ \t]+PortIn[ \t]([0-9A-F]+)/)
	{
		$mno = hex($1);
		printf("IN   %02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			next if (/^[\t ]*$/);
			
			if (/^[ \t]+EndPortIn[ \t]([0-9A-F]+)/)
			{
				if ($mno != hex($1))
				{
					system("cdc 1");
					printf("error PortIn %02X\n", $mno);
					exit 1;
				}
				last;
			}
			
			$m = $m . $_;
		}
		$opcodeIN[$mno] = $m;
	}
	elsif (/^[ \t]+PortOut[ \t]([0-9A-F]+)/)
	{
		$mno = hex($1);
		printf("OUT  %02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			next if (/^[\t ]*$/);
			
			if (/^[ \t]+EndPortOut[ \t]([0-9A-F]+)/)
			{
				if ($mno != hex($1))
				{
					system("cdc 1");
					printf("error PortOut %02X\n", $mno);
					exit 1;
				}
				last;
			}
			
			$m = $m . $_;
		}
		$opcodeOUT[$mno] = $m;
	}
	elsif (/^[ \t]+SetVDPreg[ \t]([0-9]+)/)
	{
		$mno = $1;
		printf("VDP  %02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			next if (/^[\t ]*$/);
			
			if (/^[ \t]+EndSetVDPreg[ \t]([0-9]+)/)
			{
				if ($mno != $1)
				{
					system("cdc 1");
					printf("error SetVDPreg %02X\n", $mno);
					exit 1;
				}
				last;
			}
			
			$m = $m . $_;
		}
		$opcodeVDP[$mno] = $m;
	}
	elsif (/^[ \t]+StatusReg[ \t]([0-9]+)/)
	{
		$mno = $1;
		printf("SREG %02X\r", $mno);
		$m = "";
		while (<mfile>)
		{
			next if (/^[\t ]*$/);
			
			if (/^[ \t]+EndStatusReg[ \t]([0-9]+)/)
			{
				if ($mno != $1)
				{
					system("cdc 1");
					printf("error StatusReg %02X\n", $mno);
					exit 1;
				}
				last;
			}
			
			$m = $m . $_;
		}
		$opcodeSREG[$mno] = $m;
	}
}
close(mfile);
system("cdc 1");


#----------------------------------------------------------------------------------------------
print "build source.\n";

sub padding
{
	local($size,
	      $TX1_H, $TX1_L,
	      $TX2_H, $TX2_L,
	      $MLT_H, $MLT_L,
	      $GR1_H, $GR1_L,
	      $GR2_H, $GR2_L,
	      $GR3_H, $GR3_L,
	      $GR4_H, $GR4_L,
	      $GR5_H, $GR5_L,
	      $GR6_H, $GR6_L,
	      $GR7_H, $GR7_L) = @_;
	
	print mfile ".if TEXT1\n";
	if ($TX1_H != $TX1_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $TX1_L if ($size - $TX1_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $TX1_H if ($size - $TX1_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $TX1_H if ($size - $TX1_H);
	}
	
	print mfile ".elseif TEXT2\n";
	if ($TX2_H != $TX2_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $TX2_L if ($size - $TX2_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $TX2_H if ($size - $TX2_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $TX2_H if ($size - $TX2_H);
	}
	
	print mfile ".elseif MULTI_COLOR\n";
	if ($MLT_H != $MLT_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $MLT_L if ($size - $MLT_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $MLT_H if ($size - $MLT_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $MLT_H if ($size - $MLT_H);
	}
	
	print mfile ".elseif GRAPHIC1\n";
	if ($GR1_H != $GR1_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR1_L if ($size - $GR1_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR1_H if ($size - $GR1_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR1_H if ($size - $GR1_H);
	}
	
	print mfile ".elseif GRAPHIC2\n";
	if ($GR2_H != $GR2_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR2_L if ($size - $GR2_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR2_H if ($size - $GR2_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR2_H if ($size - $GR2_H);
	}
	
	print mfile ".elseif GRAPHIC3\n";
	if ($GR3_H != $GR3_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR3_L if ($size - $GR3_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR3_H if ($size - $GR3_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR3_H if ($size - $GR3_H);
	}
	
	print mfile ".elseif GRAPHIC4\n";
	if ($GR4_H != $GR4_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR4_L if ($size - $GR4_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR4_H if ($size - $GR4_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR4_H if ($size - $GR4_H);
	}
	
	print mfile ".elseif GRAPHIC5\n";
	if ($GR5_H != $GR5_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR5_L if ($size - $GR5_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR5_H if ($size - $GR5_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR5_H if ($size - $GR5_H);
	}
	
	print mfile ".elseif GRAPHIC6\n";
	if ($GR6_H != $GR6_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR6_L if ($size - $GR6_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR6_H if ($size - $GR6_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR6_H if ($size - $GR6_H);
	}
	
	print mfile ".elseif GRAPHIC7\n";
	if ($GR7_H != $GR7_L)
	{
		print mfile ".if VRAM_LOW\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR7_L if ($size - $GR7_L);
		print mfile ".else\n";
		printf mfile "	.dcb.b	%d,0\n", $size - $GR7_H if ($size - $GR7_H);
		print mfile ".endif\n";
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $size - $GR7_H if ($size - $GR7_H);
	}
	
	print mfile ".endif\n";
}




open (mfile, ">$outdir/io.tmp") || die "can't open '$outdir/io.tmp'";
system("cdc 0");
for ($i = 0x80; $i <= 0x17f; ++$i) {
	$mno = $i & 0xff;
	printf "%02X\r", $mno;
	
	#-- OUT
	printf mfile "	PortOut %02X\n" ,$mno;
	print mfile $opcodeOUT[$mno];
	
	&padding($SIZE_OUT,
		$sizeTX1out_H[$mno], $sizeTX1out_L[$mno],
		$sizeTX2out_H[$mno], $sizeTX2out_L[$mno],
		$sizeMLTout_H[$mno], $sizeMLTout_L[$mno],
		$sizeGR1out_H[$mno], $sizeGR1out_L[$mno],
		$sizeGR2out_H[$mno], $sizeGR2out_L[$mno],
		$sizeGR3out_H[$mno], $sizeGR3out_L[$mno],
		$sizeGR4out_H[$mno], $sizeGR4out_L[$mno],
		$sizeGR5out_H[$mno], $sizeGR5out_L[$mno],
		$sizeGR6out_H[$mno], $sizeGR6out_L[$mno],
		$sizeGR7out_H[$mno], $sizeGR7out_L[$mno]);
	
	printf mfile "	EndPortOut %02X\n", $mno;
	
	#-- SREG 0-15 (1111_0000)
	if (($mno & 0x0f) == 0)
	{
		$idx = $mno >> 4;
		
		printf mfile "	StatusReg %d\n" ,$idx;
		print mfile $opcodeSREG[$idx];
		
		&padding($SIZE_SREG,
			$sizeTX1sreg_H[$idx], $sizeTX1sreg_L[$idx],
			$sizeTX2sreg_H[$idx], $sizeTX2sreg_L[$idx],
			$sizeMLTsreg_H[$idx], $sizeMLTsreg_L[$idx],
			$sizeGR1sreg_H[$idx], $sizeGR1sreg_L[$idx],
			$sizeGR2sreg_H[$idx], $sizeGR2sreg_L[$idx],
			$sizeGR3sreg_H[$idx], $sizeGR3sreg_L[$idx],
			$sizeGR4sreg_H[$idx], $sizeGR4sreg_L[$idx],
			$sizeGR5sreg_H[$idx], $sizeGR5sreg_L[$idx],
			$sizeGR6sreg_H[$idx], $sizeGR6sreg_L[$idx],
			$sizeGR7sreg_H[$idx], $sizeGR7sreg_L[$idx]);
		
		printf mfile "	EndStatusReg %d\n", $idx;
	}
	else
	{
		printf mfile "	.dcb.b	%d,0\n", $SIZE_SREG;
	}
	
	#-- label
	if ($mno == 0)
	{
		print mfile ".if VRAM_LOW\n";
		
		print mfile ".if TEXT1\n";
		print mfile "TXT1_L_IOroutine:\n";
		print mfile ".elseif TEXT2\n";
		print mfile "TXT2_L_IOroutine:\n";
		print mfile ".elseif MULTI_COLOR\n";
		print mfile "MLT_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC1\n";
		print mfile "GRA1_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC2\n";
		print mfile "GRA2_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC3\n";
		print mfile "GRA3_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC4\n";
		print mfile "GRA4_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC5\n";
		print mfile "GRA5_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC6\n";
		print mfile "GRA6_L_IOroutine:\n";
		print mfile ".elseif GRAPHIC7\n";
		print mfile "GRA7_L_IOroutine:\n";
		print mfile ".endif\n";
		
		print mfile "	.dc.w	0\n";
		
		print mfile ".else\n";
		print mfile ".if TEXT1\n";
		print mfile "TXT1_H_IOroutine:\n";
		print mfile ".elseif TEXT2\n";
		print mfile "TXT2_H_IOroutine:\n";
		print mfile ".elseif MULTI_COLOR\n";
		print mfile "MLT_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC1\n";
		print mfile "GRA1_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC2\n";
		print mfile "GRA2_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC3\n";
		print mfile "GRA3_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC4\n";
		print mfile "GRA4_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC5\n";
		print mfile "GRA5_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC6\n";
		print mfile "GRA6_H_IOroutine:\n";
		print mfile ".elseif GRAPHIC7\n";
		print mfile "GRA7_H_IOroutine:\n";
		print mfile ".endif\n";
		
		print mfile "	.dc.w	2\n";
		
		print mfile ".endif\n";
	}
	else
	{
		print mfile "	.dc.w	0\n";
	}
	
	#-- IN
	printf mfile "	PortIn %02X\n" ,$mno;
	print mfile $opcodeIN[$mno];
	
	&padding($SIZE_IN,
		$sizeTX1in_H[$mno], $sizeTX1in_L[$mno],
		$sizeTX2in_H[$mno], $sizeTX2in_L[$mno],
		$sizeMLTin_H[$mno], $sizeMLTin_L[$mno],
		$sizeGR1in_H[$mno], $sizeGR1in_L[$mno],
		$sizeGR2in_H[$mno], $sizeGR2in_L[$mno],
		$sizeGR3in_H[$mno], $sizeGR3in_L[$mno],
		$sizeGR4in_H[$mno], $sizeGR4in_L[$mno],
		$sizeGR5in_H[$mno], $sizeGR5in_L[$mno],
		$sizeGR6in_H[$mno], $sizeGR6in_L[$mno],
		$sizeGR7in_H[$mno], $sizeGR7in_L[$mno]);
	
	printf mfile "	EndPortIn %02X\n", $mno;
	
	#-- VDP 0-63 (1111_1100)
	if (($mno & 3) == 0)
	{
		$idx = $mno >> 2;
		
		printf mfile "	SetVDPreg %d\n" ,$idx;
		print mfile $opcodeVDP[$idx];
		
		&padding($SIZE_VDP,
			$sizeTX1vdp_H[$idx], $sizeTX1vdp_L[$idx],
			$sizeTX2vdp_H[$idx], $sizeTX2vdp_L[$idx],
			$sizeMLTvdp_H[$idx], $sizeMLTvdp_L[$idx],
			$sizeGR1vdp_H[$idx], $sizeGR1vdp_L[$idx],
			$sizeGR2vdp_H[$idx], $sizeGR2vdp_L[$idx],
			$sizeGR3vdp_H[$idx], $sizeGR3vdp_L[$idx],
			$sizeGR4vdp_H[$idx], $sizeGR4vdp_L[$idx],
			$sizeGR5vdp_H[$idx], $sizeGR5vdp_L[$idx],
			$sizeGR6vdp_H[$idx], $sizeGR6vdp_L[$idx],
			$sizeGR7vdp_H[$idx], $sizeGR7vdp_L[$idx]);
		
		printf mfile "	EndSetVDPreg %d\n", $idx;
	}
	else
	{
		printf mfile "	.dcb.b %d,0\n", $SIZE_VDP;
	}
	
	
	#--
	printf mfile "	.dcb.b	%d,0\n", $SIZE_TAIL if $SIZE_TAIL;
}
system("cdc 1");
close(mfile);

$comline = "has -w $sw_mpu -s PASS=2 -o $outdir/io.o -x $outdir/io.lab -t . io.has";
print $comline, "\n";
$ret = system($comline);
if ($ret) {
	exit 1;
}


#---
print "read address.\n";

undef @startTX1in_H;
undef @startTX1in_L;
undef @startTX2in_H;
undef @startTX2in_L;
undef @startMLTin_H;
undef @startMLTin_L;
undef @startGR1in_H;
undef @startGR1in_L;
undef @startGR2in_H;
undef @startGR2in_L;
undef @startGR3in_H;
undef @startGR3in_L;
undef @startGR4in_H;
undef @startGR4in_L;
undef @startGR5in_H;
undef @startGR5in_L;
undef @startGR6in_H;
undef @startGR6in_L;
undef @startGR7in_H;
undef @startGR7in_L;

undef @startTX1out_H;
undef @startTX1out_L;
undef @startTX2out_H;
undef @startTX2out_L;
undef @startMLTout_H;
undef @startMLTout_L;
undef @startGR1out_H;
undef @startGR1out_L;
undef @startGR2out_H;
undef @startGR2out_L;
undef @startGR3out_H;
undef @startGR3out_L;
undef @startGR4out_H;
undef @startGR4out_L;
undef @startGR5out_H;
undef @startGR5out_L;
undef @startGR6out_H;
undef @startGR6out_L;
undef @startGR7out_H;
undef @startGR7out_L;

undef @startTX1vdp_H;
undef @startTX1vdp_L;
undef @startTX2vdp_H;
undef @startTX2vdp_L;
undef @startMLTvdp_H;
undef @startMLTvdp_L;
undef @startGR1vdp_H;
undef @startGR1vdp_L;
undef @startGR2vdp_H;
undef @startGR2vdp_L;
undef @startGR3vdp_H;
undef @startGR3vdp_L;
undef @startGR4vdp_H;
undef @startGR4vdp_L;
undef @startGR5vdp_H;
undef @startGR5vdp_L;
undef @startGR6vdp_H;
undef @startGR6vdp_L;
undef @startGR7vdp_H;
undef @startGR7vdp_L;

undef @startTX1sreg_H;
undef @startTX1sreg_L;
undef @startTX2sreg_H;
undef @startTX2sreg_L;
undef @startMLTsreg_H;
undef @startMLTsreg_L;
undef @startGR1sreg_H;
undef @startGR1sreg_L;
undef @startGR2sreg_H;
undef @startGR2sreg_L;
undef @startGR3sreg_H;
undef @startGR3sreg_L;
undef @startGR4sreg_H;
undef @startGR4sreg_L;
undef @startGR5sreg_H;
undef @startGR5sreg_L;
undef @startGR6sreg_H;
undef @startGR6sreg_L;
undef @startGR7sreg_H;
undef @startGR7sreg_L;

undef @endTX1in_H;
undef @endTX1in_L;
undef @endTX2in_H;
undef @endTX2in_L;
undef @endMLTin_H;
undef @endMLTin_L;
undef @endGR1in_H;
undef @endGR1in_L;
undef @endGR2in_H;
undef @endGR2in_L;
undef @endGR3in_H;
undef @endGR3in_L;
undef @endGR4in_H;
undef @endGR4in_L;
undef @endGR5in_H;
undef @endGR5in_L;
undef @endGR6in_H;
undef @endGR6in_L;
undef @endGR7in_H;
undef @endGR7in_L;

undef @endTX1out_H;
undef @endTX1out_L;
undef @endTX2out_H;
undef @endTX2out_L;
undef @endMLTout_H;
undef @endMLTout_L;
undef @endGR1out_H;
undef @endGR1out_L;
undef @endGR2out_H;
undef @endGR2out_L;
undef @endGR3out_H;
undef @endGR3out_L;
undef @endGR4out_H;
undef @endGR4out_L;
undef @endGR5out_H;
undef @endGR5out_L;
undef @endGR6out_H;
undef @endGR6out_L;
undef @endGR7out_H;
undef @endGR7out_L;

undef @endTX1vdp_H;
undef @endTX1vdp_L;
undef @endTX2vdp_H;
undef @endTX2vdp_L;
undef @endMLTvdp_H;
undef @endMLTvdp_L;
undef @endGR1vdp_H;
undef @endGR1vdp_L;
undef @endGR2vdp_H;
undef @endGR2vdp_L;
undef @endGR3vdp_H;
undef @endGR3vdp_L;
undef @endGR4vdp_H;
undef @endGR4vdp_L;
undef @endGR5vdp_H;
undef @endGR5vdp_L;
undef @endGR6vdp_H;
undef @endGR6vdp_L;
undef @endGR7vdp_H;
undef @endGR7vdp_L;

undef @endTX1sreg_H;
undef @endTX1sreg_L;
undef @endTX2sreg_H;
undef @endTX2sreg_L;
undef @endMLTsreg_H;
undef @endMLTsreg_L;
undef @endGR1sreg_H;
undef @endGR1sreg_L;
undef @endGR2sreg_H;
undef @endGR2sreg_L;
undef @endGR3sreg_H;
undef @endGR3sreg_L;
undef @endGR4sreg_H;
undef @endGR4sreg_L;
undef @endGR5sreg_H;
undef @endGR5sreg_L;
undef @endGR6sreg_H;
undef @endGR6sreg_L;
undef @endGR7sreg_H;
undef @endGR7sreg_L;

system("cdc 0");
&read_adr("$outdir/io.lab");
system("cdc 1");

print "check address.\n";
&check_address;


$nerr = 0;
for ($i = 0; $i < 256; ++$i) {
	if ($i != 0x7f)
	{
		if ($startTX1in_H[($i + 1) & 0xff] - $startTX1in_H[$i] != 256)
		{
			printf("error TX1_H %02X group, != 256 bytes. (%d)\n",
				$i, $startTX1in_H[($i + 1) & 0xff] - $startTX1in_H[$i]);
		}
		
		if ($startTX1in_L[($i + 1) & 0xff] - $startTX1in_L[$i] != 256)
		{
			printf("error TX1_L %02X group, != 256 bytes. (%d)\n",
				$i, $startTX1in_L[($i + 1) & 0xff] - $startTX1in_L[$i]);
		}
		
		
		if ($startTX2in_H[($i + 1) & 0xff] - $startTX2in_H[$i] != 256)
		{
			printf("error TX2_H %02X group, != 256 bytes. (%d)\n",
				$i, $startTX2in_H[($i + 1) & 0xff] - $startTX2in_H[$i]);
		}
		
		if ($startTX2in_L[($i + 1) & 0xff] - $startTX2in_L[$i] != 256)
		{
			printf("error TX2_L %02X group, != 256 bytes. (%d)\n",
				$i, $startTX2in_L[($i + 1) & 0xff] - $startTX2in_L[$i]);
		}
		
		
		if ($startMLTin_H[($i + 1) & 0xff] - $startMLTin_H[$i] != 256)
		{
			printf("error MLT_H %02X group, != 256 bytes. (%d)\n",
				$i, $startMLTin_H[($i + 1) & 0xff] - $startMLTin_H[$i]);
		}
		
		if ($startMLTin_L[($i + 1) & 0xff] - $startMLTin_L[$i] != 256)
		{
			printf("error MLT_L %02X group, != 256 bytes. (%d)\n",
				$i, $startMLTin_L[($i + 1) & 0xff] - $startMLTin_L[$i]);
		}
		
		
		if ($startGR1in_H[($i + 1) & 0xff] - $startGR1in_H[$i] != 256)
		{
			printf("error GR1_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR1in_H[($i + 1) & 0xff] - $startGR1in_H[$i]);
		}
		
		if ($startGR1in_L[($i + 1) & 0xff] - $startGR1in_L[$i] != 256)
		{
			printf("error GR1_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR1in_L[($i + 1) & 0xff] - $startGR1in_L[$i]);
		}
		
		
		if ($startGR2in_H[($i + 1) & 0xff] - $startGR2in_H[$i] != 256)
		{
			printf("error GR2_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR2in_H[($i + 1) & 0xff] - $startGR2in_H[$i]);
		}
		
		if ($startGR2in_L[($i + 1) & 0xff] - $startGR2in_L[$i] != 256)
		{
			printf("error GR2_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR2in_L[($i + 1) & 0xff] - $startGR2in_L[$i]);
		}
		
		
		if ($startGR3in_H[($i + 1) & 0xff] - $startGR3in_H[$i] != 256)
		{
			printf("error GR3_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR3in_H[($i + 1) & 0xff] - $startGR3in_H[$i]);
		}
		
		if ($startGR3in_L[($i + 1) & 0xff] - $startGR3in_L[$i] != 256)
		{
			printf("error GR3_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR3in_L[($i + 1) & 0xff] - $startGR3in_L[$i]);
		}
		
		
		if ($startGR4in_H[($i + 1) & 0xff] - $startGR4in_H[$i] != 256)
		{
			printf("error GR4_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR4in_H[($i + 1) & 0xff] - $startGR4in_H[$i]);
		}
		
		if ($startGR4in_L[($i + 1) & 0xff] - $startGR4in_L[$i] != 256)
		{
			printf("error GR4_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR4in_L[($i + 1) & 0xff] - $startGR4in_L[$i]);
		}
		
		
		if ($startGR5in_H[($i + 1) & 0xff] - $startGR5in_H[$i] != 256)
		{
			printf("error GR5_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR5in_H[($i + 1) & 0xff] - $startGR5in_H[$i]);
		}
		
		if ($startGR5in_L[($i + 1) & 0xff] - $startGR5in_L[$i] != 256)
		{
			printf("error GR5_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR5in_L[($i + 1) & 0xff] - $startGR5in_L[$i]);
		}
		
		
		if ($startGR6in_H[($i + 1) & 0xff] - $startGR6in_H[$i] != 256)
		{
			printf("error GR6_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR6in_H[($i + 1) & 0xff] - $startGR6in_H[$i]);
		}
		
		if ($startGR6in_L[($i + 1) & 0xff] - $startGR6in_L[$i] != 256)
		{
			printf("error GR6_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR6in_L[($i + 1) & 0xff] - $startGR6in_L[$i]);
		}
		
		
		if ($startGR7in_H[($i + 1) & 0xff] - $startGR7in_H[$i] != 256)
		{
			printf("error GR7_H %02X group, != 256 bytes. (%d)\n",
				$i, $startGR7in_H[($i + 1) & 0xff] - $startGR7in_H[$i]);
		}
		
		if ($startGR7in_L[($i + 1) & 0xff] - $startGR7in_L[$i] != 256)
		{
			printf("error GR7_L %02X group, != 256 bytes. (%d)\n",
				$i, $startGR7in_L[($i + 1) & 0xff] - $startGR7in_L[$i]);
		}
	}
	
	$sizeTX1in_H[$i] != $SIZE_IN
		&& printf("error TX1in_H %02X, %d bytes.\n", $i, $sizeTX1in_H[$i]);
	$sizeTX1in_L[$i] != $SIZE_IN
		&& printf("error TX1in_L %02X, %d bytes.\n", $i, $sizeTX1in_L[$i]);
	
	$sizeTX2in_H[$i] != $SIZE_IN
		&& printf("error TX2in_H %02X, %d bytes.\n", $i, $sizeTX2in_H[$i]);
	$sizeTX2in_H[$i] != $SIZE_IN
		&& printf("error TX2in_L %02X, %d bytes.\n", $i, $sizeTX2in_L[$i]);
	
	$sizeMLTin_H[$i] != $SIZE_IN
		&& printf("error MLTin_H %02X, %d bytes.\n", $i, $sizeMLTin_H[$i]);
	$sizeMLTin_L[$i] != $SIZE_IN
		&& printf("error MLTin_L %02X, %d bytes.\n", $i, $sizeMLTin_L[$i]);
	
	$sizeGR1in_H[$i] != $SIZE_IN
		&& printf("error GR1in_H %02X, %d bytes.\n", $i, $sizeGR1in_H[$i]);
	$sizeGR1in_L[$i] != $SIZE_IN
		&& printf("error GR1in_L %02X, %d bytes.\n", $i, $sizeGR1in_L[$i]);
	
	$sizeGR2in_H[$i] != $SIZE_IN
		&& printf("error GR2in_H %02X, %d bytes.\n", $i, $sizeGR2in_H[$i]);
	$sizeGR2in_L[$i] != $SIZE_IN
		&& printf("error GR2in_L %02X, %d bytes.\n", $i, $sizeGR2in_L[$i]);
	
	$sizeGR3in_H[$i] != $SIZE_IN
		&& printf("error GR3in_H %02X, %d bytes.\n", $i, $sizeGR3in_H[$i]);
	$sizeGR3in_L[$i] != $SIZE_IN
		&& printf("error GR3in_L %02X, %d bytes.\n", $i, $sizeGR3in_L[$i]);
	
	$sizeGR4in_H[$i] != $SIZE_IN
		&& printf("error GR4in_H %02X, %d bytes.\n", $i, $sizeGR4in_H[$i]);
	$sizeGR4in_L[$i] != $SIZE_IN
		&& printf("error GR4in_L %02X, %d bytes.\n", $i, $sizeGR4in_L[$i]);
	
	$sizeGR5in_H[$i] != $SIZE_IN
		&& printf("error GR5in_H %02X, %d bytes.\n", $i, $sizeGR5in_H[$i]);
	$sizeGR5in_L[$i] != $SIZE_IN
		&& printf("error GR5in_L %02X, %d bytes.\n", $i, $sizeGR5in_L[$i]);
	
	$sizeGR6in_H[$i] != $SIZE_IN
		&& printf("error GR6in_H %02X, %d bytes.\n", $i, $sizeGR6in_H[$i]);
	$sizeGR6in_L[$i] != $SIZE_IN
		&& printf("error GR6in_L %02X, %d bytes.\n", $i, $sizeGR6in_L[$i]);
	
	$sizeGR7in_H[$i] != $SIZE_IN
		&& printf("error GR7in_H %02X, %d bytes.\n", $i, $sizeGR7in_H[$i]);
	$sizeGR7in_L[$i] != $SIZE_IN
		&& printf("error GR7in_L %02X, %d bytes.\n", $i, $sizeGR7in_L[$i]);
	
	
	$sizeTX1out_H[$i] != $SIZE_OUT
		&& printf("error TX1out_H %02X, %d bytes.\n", $i, $sizeTX1out_H[$i]);
	$sizeTX1out_L[$i] != $SIZE_OUT
		&& printf("error TX1out_L %02X, %d bytes.\n", $i, $sizeTX1out_L[$i]);
	
	$sizeTX2out_H[$i] != $SIZE_OUT
		&& printf("error TX2out_H %02X, %d bytes.\n", $i, $sizeTX2out_H[$i]);
	$sizeTX2out_L[$i] != $SIZE_OUT
		&& printf("error TX2out_L %02X, %d bytes.\n", $i, $sizeTX2out_L[$i]);
	
	$sizeMLTout_H[$i] != $SIZE_OUT
		&& printf("error MLTout_H %02X, %d bytes.\n", $i, $sizeMLTout_H[$i]);
	$sizeMLTout_L[$i] != $SIZE_OUT
		&& printf("error MLTout_L %02X, %d bytes.\n", $i, $sizeMLTout_L[$i]);
	
	$sizeGR1out_H[$i] != $SIZE_OUT
		&& printf("error GR1out_H %02X, %d bytes.\n", $i, $sizeGR1out_H[$i]);
	$sizeGR1out_L[$i] != $SIZE_OUT
		&& printf("error GR1out_L %02X, %d bytes.\n", $i, $sizeGR1out_L[$i]);
	
	$sizeGR2out_H[$i] != $SIZE_OUT
		&& printf("error GR2out_H %02X, %d bytes.\n", $i, $sizeGR2out_H[$i]);
	$sizeGR2out_L[$i] != $SIZE_OUT
		&& printf("error GR2out_L %02X, %d bytes.\n", $i, $sizeGR2out_L[$i]);
	
	$sizeGR3out_H[$i] != $SIZE_OUT
		&& printf("error GR3out_H %02X, %d bytes.\n", $i, $sizeGR3out_H[$i]);
	$sizeGR3out_L[$i] != $SIZE_OUT
		&& printf("error GR3out_L %02X, %d bytes.\n", $i, $sizeGR3out_L[$i]);
	
	$sizeGR4out_H[$i] != $SIZE_OUT
		&& printf("error GR4out_H %02X, %d bytes.\n", $i, $sizeGR4out_H[$i]);
	$sizeGR4out_L[$i] != $SIZE_OUT
		&& printf("error GR4out_L %02X, %d bytes.\n", $i, $sizeGR4out_L[$i]);
	
	$sizeGR5out_H[$i] != $SIZE_OUT
		&& printf("error GR5out_H %02X, %d bytes.\n", $i, $sizeGR5out_H[$i]);
	$sizeGR5out_L[$i] != $SIZE_OUT
		&& printf("error GR5out_L %02X, %d bytes.\n", $i, $sizeGR5out_L[$i]);
	
	$sizeGR6out_H[$i] != $SIZE_OUT
		&& printf("error GR6out_H %02X, %d bytes.\n", $i, $sizeGR6out_H[$i]);
	$sizeGR6out_L[$i] != $SIZE_OUT
		&& printf("error GR6out_L %02X, %d bytes.\n", $i, $sizeGR6out_L[$i]);
	
	$sizeGR7out_H[$i] != $SIZE_OUT
		&& printf("error GR7out_H %02X, %d bytes.\n", $i, $sizeGR7out_H[$i]);
	$sizeGR7out_L[$i] != $SIZE_OUT
		&& printf("error GR7out_L %02X, %d bytes.\n", $i, $sizeGR7out_L[$i]);
	
	if ($i < 64)
	{
		$sizeTX1vdp_H[$i] != $SIZE_VDP
			&& printf("error TX1vdp_H %02X, %d bytes.\n", $i, $sizeTX1vdp_H[$i]);
		$sizeTX1vdp_L[$i] != $SIZE_VDP
			&& printf("error TX1vdp_L %02X, %d bytes.\n", $i, $sizeTX1vdp_L[$i]);
		
		$sizeTX2vdp_H[$i] != $SIZE_VDP
			&& printf("error TX2vdp_H %02X, %d bytes.\n", $i, $sizeTX2vdp_H[$i]);
		$sizeTX2vdp_L[$i] != $SIZE_VDP
			&& printf("error TX2vdp_L %02X, %d bytes.\n", $i, $sizeTX2vdp_L[$i]);
		
		$sizeMLTvdp_H[$i] != $SIZE_VDP
			&& printf("error MLTvdp_H %02X, %d bytes.\n", $i, $sizeMLTvdp_H[$i]);
		$sizeMLTvdp_L[$i] != $SIZE_VDP
			&& printf("error MLTvdp_L %02X, %d bytes.\n", $i, $sizeMLTvdp_L[$i]);
		
		$sizeGR1vdp_H[$i] != $SIZE_VDP
			&& printf("error GR1vdp_H %02X, %d bytes.\n", $i, $sizeGR1vdp_H[$i]);
		$sizeGR1vdp_L[$i] != $SIZE_VDP
			&& printf("error GR1vdp_L %02X, %d bytes.\n", $i, $sizeGR1vdp_L[$i]);
		
		$sizeGR2vdp_H[$i] != $SIZE_VDP
			&& printf("error GR2vdp_H %02X, %d bytes.\n", $i, $sizeGR2vdp_H[$i]);
		$sizeGR2vdp_L[$i] != $SIZE_VDP
			&& printf("error GR2vdp_L %02X, %d bytes.\n", $i, $sizeGR2vdp_L[$i]);
		
		$sizeGR3vdp_H[$i] != $SIZE_VDP
			&& printf("error GR3vdp_H %02X, %d bytes.\n", $i, $sizeGR3vdp_H[$i]);
		$sizeGR3vdp_L[$i] != $SIZE_VDP
			&& printf("error GR3vdp_L %02X, %d bytes.\n", $i, $sizeGR3vdp_L[$i]);
		
		$sizeGR4vdp_H[$i] != $SIZE_VDP
			&& printf("error GR4vdp_H %02X, %d bytes.\n", $i, $sizeGR4vdp_H[$i]);
		$sizeGR4vdp_L[$i] != $SIZE_VDP
			&& printf("error GR4vdp_L %02X, %d bytes.\n", $i, $sizeGR4vdp_L[$i]);
		
		$sizeGR5vdp_H[$i] != $SIZE_VDP
			&& printf("error GR5vdp_H %02X, %d bytes.\n", $i, $sizeGR5vdp_H[$i]);
		$sizeGR5vdp_L[$i] != $SIZE_VDP
			&& printf("error GR5vdp_L %02X, %d bytes.\n", $i, $sizeGR5vdp_L[$i]);
		
		$sizeGR6vdp_H[$i] != $SIZE_VDP
			&& printf("error GR6vdp_H %02X, %d bytes.\n", $i, $sizeGR6vdp_H[$i]);
		$sizeGR6vdp_L[$i] != $SIZE_VDP
			&& printf("error GR6vdp_L %02X, %d bytes.\n", $i, $sizeGR6vdp_L[$i]);
		
		$sizeGR7vdp_H[$i] != $SIZE_VDP
			&& printf("error GR7vdp_H %02X, %d bytes.\n", $i, $sizeGR7vdp_H[$i]);
		$sizeGR7vdp_L[$i] != $SIZE_VDP
			&& printf("error GR7vdp_L %02X, %d bytes.\n", $i, $sizeGR7vdp_L[$i]);
	}
	
	if ($i < 16)
	{
		$sizeTX1sreg_H[$i] != $SIZE_SREG
			&& printf("error TX1sreg_H %02X, %d bytes.\n", $i, $sizeTX1sreg_H[$i]);
		$sizeTX1sreg_L[$i] != $SIZE_SREG
			&& printf("error TX1sreg_L %02X, %d bytes.\n", $i, $sizeTX1sreg_L[$i]);
		
		$sizeTX2sreg_H[$i] != $SIZE_SREG
			&& printf("error TX2sreg_H %02X, %d bytes.\n", $i, $sizeTX2sreg_H[$i]);
		$sizeTX2sreg_L[$i] != $SIZE_SREG
			&& printf("error TX2sreg_L %02X, %d bytes.\n", $i, $sizeTX2sreg_L[$i]);
		
		$sizeMLTsreg_H[$i] != $SIZE_SREG
			&& printf("error MLTsreg_H %02X, %d bytes.\n", $i, $sizeMLTsreg_H[$i]);
		$sizeMLTsreg_L[$i] != $SIZE_SREG
			&& printf("error MLTsreg_L %02X, %d bytes.\n", $i, $sizeMLTsreg_L[$i]);
		
		$sizeGR1sreg_H[$i] != $SIZE_SREG
			&& printf("error GR1sreg_H %02X, %d bytes.\n", $i, $sizeGR1sreg_H[$i]);
		$sizeGR1sreg_L[$i] != $SIZE_SREG
			&& printf("error GR1sreg_L %02X, %d bytes.\n", $i, $sizeGR1sreg_L[$i]);
		
		$sizeGR2sreg_H[$i] != $SIZE_SREG
			&& printf("error GR2sreg_H %02X, %d bytes.\n", $i, $sizeGR2sreg_H[$i]);
		$sizeGR2sreg_L[$i] != $SIZE_SREG
			&& printf("error GR2sreg_L %02X, %d bytes.\n", $i, $sizeGR2sreg_L[$i]);
		
		$sizeGR3sreg_H[$i] != $SIZE_SREG
			&& printf("error GR3sreg_H %02X, %d bytes.\n", $i, $sizeGR3sreg_H[$i]);
		$sizeGR3sreg_L[$i] != $SIZE_SREG
			&& printf("error GR3sreg_L %02X, %d bytes.\n", $i, $sizeGR3sreg_L[$i]);
		
		$sizeGR4sreg_H[$i] != $SIZE_SREG
			&& printf("error GR4sreg_H %02X, %d bytes.\n", $i, $sizeGR4sreg_H[$i]);
		$sizeGR4sreg_L[$i] != $SIZE_SREG
			&& printf("error GR4sreg_L %02X, %d bytes.\n", $i, $sizeGR4sreg_L[$i]);
		
		$sizeGR5sreg_H[$i] != $SIZE_SREG
			&& printf("error GR5sreg_H %02X, %d bytes.\n", $i, $sizeGR5sreg_H[$i]);
		$sizeGR5sreg_L[$i] != $SIZE_SREG
			&& printf("error GR5sreg_L %02X, %d bytes.\n", $i, $sizeGR5sreg_L[$i]);
		
		$sizeGR6sreg_H[$i] != $SIZE_SREG
			&& printf("error GR6sreg_H %02X, %d bytes.\n", $i, $sizeGR6sreg_H[$i]);
		$sizeGR6sreg_L[$i] != $SIZE_SREG
			&& printf("error GR6sreg_L %02X, %d bytes.\n", $i, $sizeGR6sreg_L[$i]);
		
		$sizeGR7sreg_H[$i] != $SIZE_SREG
			&& printf("error GR7sreg_H %02X, %d bytes.\n", $i, $sizeGR7sreg_H[$i]);
		$sizeGR7sreg_L[$i] != $SIZE_SREG
			&& printf("error GR7sreg_L %02X, %d bytes.\n", $i, $sizeGR7sreg_L[$i]);
	}
}
if ($nerr) {
	exit 1;
}


print "\ncomplete.\n\n";
