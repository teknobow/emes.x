#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 nir
#


if ($#ARGV != 0)
{
	printf STDERR "出力先のディレクトリの指定が必要です\n";

	exit 1;
}


$outdir = $ARGV[0];



if (! -d $outdir)
{
	printf STDERR "出力先がディレクトリではありません\n";
	
	exit 1;
}


sub read_flag_table
{
	local($name, $flag, $fname, $len) = @_;
	
	print $name;
	open(ffile, $outdir."/".$fname) || die " not found '$outdir/$fname'.";
	binmode ffile;
	if (read(ffile, $_[1], $len) != $len)
	{
		print " file size error.\n";
		exit 1;
	}
	close(ffile);
}


print "read flag tables.\n";
print "[";
&read_flag_table("ADD8",   $add8, "add8.flg", 131072);
&read_flag_table(", SUB8", $sub8, "sub8.flg", 131072);
print "]\n";


#
print "create $outdir/ftbl8.has.\n";
open (file, ">$outdir/ftbl8.has") || die "can't open '$outdir/ftbl8.has'.";

print file "	.xdef	FtblADD8,FtblSUB8\n";
print file "	.data\n";

system("cdc 0");

# ADD8
for ($i = 1; $i >= 0; --$i)
{
	print file "FtblADD8:\n" if $i == 0;
	
	for ($j = 0; $j < 65536; $j += 16)
	{
		printf("ADD:%d:%02X\r", $i, $j >> 8) if ($j & 0xff) == 0;
		
#		print file "	.dc.b	";
#		for ($k = 0; $k < 16; ++$k)
#		{
#			print file "," if $k != 0;
#			print file ord substr($add8, $i * 65536 + $j + $k, 1);
#		}
#		print file "\n";

		$line = "	.dc.b	";
		for ($k = 0; $k < 16; ++$k)
		{
			$line .= "," if $k != 0;
			$line .= ord substr($add8, $i * 65536 + $j + $k, 1);
		}
		print file $line;
		print file "\n";
	}
}
print file "\n";

# SUB8
for ($i = 1; $i >= 0; --$i)
{
	print file "FtblSUB8:\n" if $i == 0;
	
	for ($j = 0; $j < 65536; $j += 16)
	{
		printf("SUB:%d:%02X\r", $i, $j >> 8) if ($j & 0xff) == 0;
		print file "	.dc.b	";
		for ($k = 0; $k < 16; ++$k)
		{
			print file "," if $k != 0;
			print file ord substr($sub8, $i * 65536 + $j + $k, 1);
		}
		print file "\n";
	}
}

system("cdc 1");

close file;

#---
$comline = "has -w -m68000 -o $outdir/ftbl8.o $outdir/ftbl8.has";
print $comline, "\n";
$ret = system($comline);
exit 1 if $ret;

