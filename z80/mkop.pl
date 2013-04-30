#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 teknobow
#


$nerr = 0;

print "	.xdef Xoptab,CBoptab,DDoptab,DDCBoptab,EDoptab,FDoptab,FDCBoptab,STRtab\n";
print "	.data\n";

while (<>) {
	next if /^#/;
	
	chop;
	split(/:/);
	$group = hex($_[0]) >> 8;
	$op = hex($_[0]) & 0xff;
	
	if ($group == 0)
	{
		if (defined $nimX[$op])
		{
			printf STDERR "%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimX[$op], $opX[$op], $sizeX[$op]) = ($_[1], $_[2], $_[3]);
	}
	elsif ($group == 0xCB)
	{
		if (defined $nimCB[$op])
		{
			printf STDERR "CB%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimCB[$op], $opCB[$op], $sizeCB[$op]) = ($_[1], $_[2], $_[3]);
	}
	elsif ($group == 0xDD)
	{
		if (defined $nimDD[$op])
		{
			printf STDERR "DD%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimDD[$op], $opDD[$op], $sizeDD[$op]) = ($_[1], $_[2], $_[3]);
	}
	elsif ($group == 0xDDCB)
	{
		if (defined $nimDDCB[$op])
		{
			printf STDERR "DDCB%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimDDCB[$op], $opDDCB[$op], $sizeDDCB[$op]) = ($_[1], $_[2], $_[3]);
	}
	elsif ($group == 0xED)
	{
		if (defined $nimED[$op])
		{
			printf STDERR "ED%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimED[$op], $opED[$op], $sizeED[$op]) = ($_[1], $_[2], $_[3]);
	}
	elsif ($group == 0xFD)
	{
		if (defined $nimFD[$op])
		{
			printf STDERR "FD%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimFD[$op], $opFD[$op], $sizeFD[$op]) = ($_[1], $_[2], $_[3]);
	}
	elsif ($group == 0xFDCB)
	{
		if (defined $nimFDCB[$op])
		{
			printf STDERR "FDCB%02X:redefined.\n", $op;
			++$nerr;
			next;
		}
		($nimFDCB[$op], $opFDCB[$op], $sizeFDCB[$op]) = ($_[1], $_[2], $_[3]);
	}
	else
	{
		printf STDERR "??? [%X:%s:%s:%d]\n", hex $_[0], $_[1], $_[2], $_[3];
	}
}


# check
for ($i = 0; $i < 255; ++$i)
{
	! defined($nimX[$i])
		&& (printf(STDERR "error %02X\n", $i),		++$nerr);
	
	! defined($nimCB[$i])
		&& (printf(STDERR "error CB%02X\n", $i),	++$nerr);
	
	! defined($nimDD[$i])
		&& (printf(STDERR "error DD%02X\n", $i),	++$nerr);
	
	! defined($nimDDCB[$i])
		&& (printf(STDERR "error DDCB%02X\n", $i),	++$nerr);
	
	! defined($nimED[$i])
		&& (printf(STDERR "error ED%02X\n", $i),	++$nerr);
	
	! defined($nimFD[$i])
		&& (printf(STDERR "error FD%02X\n", $i),	++$nerr);
	
	! defined($nimFDCB[$i])
		&& (printf(STDERR "error FDCB%02X\n", $i),     ++$nerr);
}
exit 1 if $nerr;


#
$num = 0;

sub strnum
{
	local($name) = @_;
	local($result) = $num;
	
	if (defined($strtable{$name}))
	{
		$result = $strtable{$name};
	}
	else
	{
		$nametable[$num] = $name;
		$strtable{$name} = $num++;
	}
	
	$result;
}


for ($i = 0; $i < 256; ++$i)
{
	&strnum($nimX[$i]);
	&strnum($nimCB[$i]);
	&strnum($nimDD[$i]);
	&strnum($nimED[$i]);
	&strnum($nimFD[$i]);
	&strnum($nimDDCB[$i]);
	&strnum($nimFDCB[$i]);
}


# make X
print "Xoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimX[$i]), &strnum($opX[$i]), $sizeX[$i], $nimX[$i], $opX[$i];
}


# make CB
print "CBoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimCB[$i]), &strnum($opCB[$i]), $sizeCB[$i], $nimCB[$i], $opCB[$i];
}

# make DD
print "DDoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimDD[$i]), &strnum($opDD[$i]),
		$sizeDD[$i],
		$nimDD[$i], $opDD[$i];
}

# make DDCB
print "DDCBoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimDDCB[$i]), &strnum($opDDCB[$i]),
		$sizeDDCB[$i],
		$nimDDCB[$i], $opDDCB[$i];
}

# make ED
print "EDoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimED[$i]), &strnum($opED[$i]),
		$sizeED[$i],
		$nimED[$i], $opED[$i];
}

# make FD
print "FDoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimFD[$i]), &strnum($opFD[$i]),
		$sizeFD[$i],
		$nimFD[$i], $opFD[$i];
}

# make FDCB
print "FDCBoptab:\n";
for ($i = 0; $i < 256; ++$i)
{
	printf "	.dc.w	\$%02x*512+\$%03x,%d	* %s %s\n",
		&strnum($nimFDCB[$i]), &strnum($opFDCB[$i]),
		$sizeFDCB[$i],
		$nimFDCB[$i], $opFDCB[$i];
}


# make str table
print "STRtab:\n";
for ($i = 0; $i < $num; ++$i)
{
	printf "	.dc.b	\"%-8s\"		* \$%04x\n", $nametable[$i], $i;
}
