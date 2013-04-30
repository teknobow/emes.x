//---------------------------------------------------------------------------------------------
// MSX Emulator for X680x0 - emes.x
//
//    Copyright 1997-1998 teknobow
//


#include <stdio.h>


unsigned long conv(unsigned long base, unsigned short col)
{
	unsigned long val = 0;
	
	if (col & 0b1000) val |= base << 3;
	if (col & 0b0100) val |= base << 2;
	if (col & 0b0010) val |= base << 1;
	if (col & 0b0001) val |= base;
	
	
	return val;
}


int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned short idx;
	char fname[1024];
	
	
	if (argc != 2)
	{
		fprintf(stderr, "mkpcgtbl.x <outdir>\n");
		
		return 1;
	}
	
	
	strcpy(fname, argv[1]);
	strcat(fname, "/pcgtbl.has");
	
	
	fp = fopen(fname, "w");
	if (fp == NULL)
	{
		fprintf(stderr, "cannot create '%s'.\n", fname);
		
		return 1;
	}
	
	fprintf(fp, "	.xdef PCGdatatbl\n");
//	fprintf(fp, "	.xdef table_PCGtoGRAPH\n");
//	fprintf(fp, "	.xdef table_PCGtoXPCG");
	fprintf(fp, "	.data\n");
	fprintf(fp, "	.quad\n");
	
	for (idx = 0x80; idx <= 0x17f; ++idx)
	{
		unsigned short pat = idx & 0xff;
		unsigned long base1;
		unsigned long base0;
		unsigned short col;
		
		
		if (idx == 0x100) fprintf(fp, "PCGdatatbl:\n");
		
		/* ‚P‚Ì•”•ª‚ÌF */
		base1 = 0;
		if (pat & 0b1000_0000) base1 |= 0x10000000;
		if (pat & 0b0100_0000) base1 |= 0x01000000;
		if (pat & 0b0010_0000) base1 |= 0x00100000;
		if (pat & 0b0001_0000) base1 |= 0x00010000;
		if (pat & 0b0000_1000) base1 |= 0x00001000;
		if (pat & 0b0000_0100) base1 |= 0x00000100;
		if (pat & 0b0000_0010) base1 |= 0x00000010;
		if (pat & 0b0000_0001) base1 |= 0x00000001;
		
		
		/* ‚O‚Ì•”•ª‚ÌF */
		base0 = 0;
		if (!(pat & 0b1000_0000)) base0 |= 0x10000000;
		if (!(pat & 0b0100_0000)) base0 |= 0x01000000;
		if (!(pat & 0b0010_0000)) base0 |= 0x00100000;
		if (!(pat & 0b0001_0000)) base0 |= 0x00010000;
		if (!(pat & 0b0000_1000)) base0 |= 0x00001000;
		if (!(pat & 0b0000_0100)) base0 |= 0x00000100;
		if (!(pat & 0b0000_0010)) base0 |= 0x00000010;
		if (!(pat & 0b0000_0001)) base0 |= 0x00000001;
		
		for (col= 0; col < 16; ++col)
		{
			fprintf(fp, "	.dc.l	$%08x,$%08x\n",
						conv(base1, col), conv(base0, col));
			fprintf(fp, "	.dc.l	0,0\n");
		}
	}
	



	goto skip;



	
	/*----------------------------------------------------------------------*/
	fprintf(fp, "table_PCGtoGRAPH:\n");
	
	for (idx = 0; idx <= 0xff; ++idx)
	{
		unsigned short fcol, bcol;
		unsigned short pat;
		
		fprintf(stderr, "%02X\r", idx);
		
		//pat = ((idx & 0x0f) << 4) | ((idx & 0xf0) >> 4);
		pat = idx;
		
		for (fcol = 0; fcol < 16; ++fcol)
		{
			fprintf(fp, "	.dc.l	");
			
			for (bcol = 0; bcol < 16; ++bcol)
			{
				unsigned long base1 = 0;
				unsigned long base0 = 0;
				
				
				/* 75316420 */
				
				/* ‚P‚Ì•”•ª‚ÌF */
				if (pat & 0b1000_0000) base1 |= 0x10000000;
				if (pat & 0b0100_0000) base1 |= 0x00001000;
				if (pat & 0b0010_0000) base1 |= 0x01000000;
				if (pat & 0b0001_0000) base1 |= 0x00000100;
				if (pat & 0b0000_1000) base1 |= 0x00100000;
				if (pat & 0b0000_0100) base1 |= 0x00000010;
				if (pat & 0b0000_0010) base1 |= 0x00010000;
				if (pat & 0b0000_0001) base1 |= 0x00000001;
				
				
				/* ‚O‚Ì•”•ª‚ÌF */
				if (!(pat & 0b1000_0000)) base0 |= 0x10000000;
				if (!(pat & 0b0100_0000)) base0 |= 0x00001000;
				if (!(pat & 0b0010_0000)) base0 |= 0x01000000;
				if (!(pat & 0b0001_0000)) base0 |= 0x00000100;
				if (!(pat & 0b0000_1000)) base0 |= 0x00100000;
				if (!(pat & 0b0000_0100)) base0 |= 0x00000010;
				if (!(pat & 0b0000_0010)) base0 |= 0x00010000;
				if (!(pat & 0b0000_0001)) base0 |= 0x00000001;
				
				fprintf(fp, "$%08x", conv(base1, fcol) | conv(base0, bcol));
				if (bcol != 15) fprintf(fp, ",");
			}
			
			fprintf(fp, "\n");
		}
	}
	
	/*----------------------------------------------------------------------*/
	fprintf(fp, "table_PCGtoXPCG:\n");
	
	for (idx = 0; idx <= 0xff; ++idx)
	{
		unsigned short fcol, bcol;
		unsigned short pat;
		
		fprintf(stderr, "%02X\r", idx);
		
		//pat = ((idx & 0x0f) << 4) | ((idx & 0xf0) >> 4);
		pat = idx;
		
		for (fcol = 0; fcol < 16; ++fcol)
		{
			fprintf(fp, "	.dc.l	");
			
			for (bcol = 0; bcol < 16; ++bcol)
			{
				unsigned long base1 = 0;
				unsigned long base0 = 0;
				
				
				/* 75316420 */
				
				/* ‚P‚Ì•”•ª‚ÌF */
				if (pat & 0b1000_0000) base1 |= 0x10000000;
				if (pat & 0b0100_0000) base1 |= 0x01000000;
				if (pat & 0b0010_0000) base1 |= 0x00100000;
				if (pat & 0b0001_0000) base1 |= 0x00010000;
				if (pat & 0b0000_1000) base1 |= 0x00001000;
				if (pat & 0b0000_0100) base1 |= 0x00000100;
				if (pat & 0b0000_0010) base1 |= 0x00000010;
				if (pat & 0b0000_0001) base1 |= 0x00000001;
				
				
				/* ‚O‚Ì•”•ª‚ÌF */
				if (!(pat & 0b1000_0000)) base0 |= 0x10000000;
				if (!(pat & 0b0100_0000)) base0 |= 0x01000000;
				if (!(pat & 0b0010_0000)) base0 |= 0x00100000;
				if (!(pat & 0b0001_0000)) base0 |= 0x00010000;
				if (!(pat & 0b0000_1000)) base0 |= 0x00001000;
				if (!(pat & 0b0000_0100)) base0 |= 0x00000100;
				if (!(pat & 0b0000_0010)) base0 |= 0x00000010;
				if (!(pat & 0b0000_0001)) base0 |= 0x00000001;
				
				fprintf(fp, "$%08x", conv(base1, fcol) | conv(base0, bcol));
				if (bcol != 15) fprintf(fp, ",");
			}
			
			fprintf(fp, "\n");
		}
	}
	
	


skip:


	
	fclose(fp);
	
	
	return 0;
}


