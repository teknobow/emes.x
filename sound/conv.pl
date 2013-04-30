#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 nir
#


if (($#ARGV != 0) || (! -d $ARGV[0]))
{
	printf STDERR "conv.pl <出力先ディレクトリ>";

	exit 1;
}


$outdir = $ARGV[0];


#
$opmNote[0] = 0;
$opmNote[1] = 1;
$opmNote[2] = 2;
$opmNote[3] = 4;
$opmNote[4] = 5;
$opmNote[5] = 6;
$opmNote[6] = 8;
$opmNote[7] = 9;
$opmNote[8] = 0xa;
$opmNote[9] = 0xc;
$opmNote[10] = 0xd;
$opmNote[11] = 0xe;

sub note
{
	local($idx) = $_[0];
	
	$kf = $idx % 64;
	
	$idx = int($idx / 64);
	
	$n = $opmNote[$idx % 12];
	
	$idx = int($idx / 12);
	
	$oct = $idx;
}



$b = 4/3.579545;

for ($oct = 0; $oct < 11; ++$oct)
{
	for ($n = 0; $n < 12; ++$n)
	{
		for ($kf = 0; $kf < 64; ++$kf)
		{
###			printf STDERR "OCT=%d, N=%2d, KF=%2d\r", $oct, $n, $kf;
			
			$d1 = ($oct-4) + (($n + $kf/64)-8)/12;
			$dt1 = 440 * (2**$d1) * $b;
			
			$fmtable[($oct*12+$n)*64+$kf] = $dt1;
		}
	}
}


$i = 0;
for ($tp = 0xfff; $tp > 0; --$tp)
{
	$Hz = 111860.78125/$tp;
	
##	printf("%4d : %15.8f : ", $tp, $Hz);
	
	for ( ; $i < (11*12*64); ++$i)
	{
		if ($Hz <= $fmtable[$i])
		{
			$idx = $i;
			
			if (($i > 0) && ($i != 11*12*64-1))
			{
				$mae = $Hz - $fmtable[$i-1];
				$koko = $fmtable[$i] - $Hz;
				
				if ($mae < $koko) { --$idx; }
			}
			
##			printf("%15.8f : %5d : ", $fmtable[$idx], $idx);
			
			&note($idx);
##			printf("O%d,%2d,%2d", $oct, $n, $kf);
			
			$convTable[$tp] = $idx;
			
			last;
		}
	}
	
	if ($i == 11*12*64)
	{
		$convTable[$tp] = $i-1;
		
##		printf("%15.8f : %5d : ", $fmtable[$i-1], $i-1);
		&note($i-1);
##		printf("O%d,%2d,%2d", $oct, $n, $kf);
	}
	
##	print "\n";
}
$convTable[0] = 11*12*64-1;



open(file, ">$outdir/psg2opm.inc");

print file "tab_PSGtoOPM_pitch:\n";

for ($i = 0; $i <= 0xfff; ++$i)
{
	$dt = $convTable[$i];
	&note($dt);
	
	if ($oct <= 8)
	{
		printf file "	.dc.b	\$%02x,\$%02x	", ($oct<<4) | $n, $kf<<2;
	}
	else
	{
		printf file "	.dc.b	-1,-1	";
	}
	
	printf file "; %4d : %2d,%2d,%2d : %15.8f Hz\n", $i, $oct, $n, $kf, $fmtable[$dt];
}

close(file);
