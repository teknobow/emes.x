//---------------------------------------------------------------------------------------------
// MSX Emulator for X680x0 - emes.x
//
//    Copyright 1997-1998 nir
//


#include <stdio.h>
#include "flag.h"



unsigned char Table_PEPO[256];


void create_table_PEPO(void)
{
	short val;
	
	for (val = 0; val < 256; ++val)
	{
		short i, pv, d;
		
		pv = 0;
		d = val;
		for (i = 0; i < 8; ++i)
		{
			if (d & 1) ++pv;
			d >>= 1;
		}
		
		if (pv & 1) Table_PEPO[val] = 0;
		else Table_PEPO[val] = Flag_P;
	}
}


/*
 * INC r
 *
 * 演算前の値を使用
 */
int inc_r(void)
{
	unsigned short r;
	unsigned char table[256];
	
	
	fprintf(stderr, "create INC flag table.\n");
	
	for (r = 0; r <= 0xff; ++r)
	{
		unsigned short r2 = r + 1;
		unsigned char f = 0;
		
		if (r2 & 0x80) f |= Flag_S;
		
		if ((r2 & 0xff) == 0) f |= Flag_Z;
		
		if ((r & 0x0f) == 15) f |= Flag_H;
		
		if (r == 0x7f) f |= Flag_V;
		
		if (r2 > 0xff) f |= Flag_c;
		
		table[r] = f;
	}
	
	fwrite(table, 1, 256, stdout);
	
	return 0;
}


/*
 * DEC r
 *
 * 演算前の値を使用
 */
int dec_r(void)
{
	unsigned short r;
	unsigned char table[256];
	
	
	fprintf(stderr, "create DEC flag table.\n");
	
	for (r = 0; r <= 0xff; ++r)
	{
		unsigned short r2 = r - 1;
		unsigned char f = 0;
		
		if (r2 & 0x80) f |= Flag_S;
		
		if ((r2 & 0xff) == 0) f |= Flag_Z;
		
		if ((r & 0x0f) == 0) f |= Flag_H;
		
		if (r == 0x80) f |= Flag_V;
		
		f |= Flag_N;
		
		if (r2 > 0xff) f |= Flag_c;
		
		table[r] = f;
	}
	
	fwrite(table, 1, 256, stdout);
	
	return 0;
}


/* DAA */
int daa(void)
{
	unsigned char val_table[2][2][2][256];	/* C:H:N:val */
	unsigned char flag_table[2][2][2][256];	/* C:H:N:flag */
	unsigned short c, h, n, idx;
	
	
	fprintf(stderr, "create DAA flag table.\n");
	
	
	/* ADD の補正 */
	n = 0;
	for (c = 0; c <= 1; ++c)
	{
		for (h = 0; h <= 1; ++h)
		{
			for (idx = 0; idx <= 0xff; ++idx)
			{
				unsigned short half_h = (idx & 0xf0) >> 4;
				unsigned short half_l = (idx & 0x0f);
				unsigned short val_hosei, carry;
				unsigned short val, flag;
				
				if (c == 0)
				{
					if (h == 0)
					{
						if (half_l < 10)
						{
							if (half_h < 10)
							{
								val_hosei = 0;
								carry = 0;
							}
							else
							{
								val_hosei = 0x60;
								carry = Flag_Cc;
							}
						}
						else
						{
							if (half_h < 9)
							{
								val_hosei = 0x06;
								carry = 0;
							}
							else
							{
								val_hosei = 0x66;
								carry = Flag_Cc;
							}
						}
					}
					else
					{
						if (half_h < 10)
						{
							val_hosei = 0x06;
							carry = 0;
						}
						else
						{
							val_hosei = 0x66;
							carry = Flag_Cc;
						}
					}
				}
				else
				{
					if (h == 0)
					{
						if (half_l < 10)
						{
							val_hosei = 0x60;
							carry = Flag_Cc;
						}
						else
						{
							val_hosei = 0x66;
							carry = Flag_Cc;
						}
					}
					else
					{
						val_hosei = 0x66;
						carry = Flag_Cc;
					}
				}
				
				val = (idx + val_hosei) & 0xff;
				
				flag = 0;
				if (val & 0x80) flag |= Flag_S;
				if (val == 0) flag |= Flag_Z;
				flag |= Table_PEPO[val];
				flag |= carry;
				
				val_table[c][h][n][idx] = val;
				flag_table[c][h][n][idx] = flag;
			}
		}
	}
	
	
	/* SUB の補正 */
	n = 1;
	for (c = 0; c <= 1; ++c)
	{
		for (h = 0; h <= 1; ++h)
		{
			for (idx = 0; idx <= 0xff; ++idx)
			{
				unsigned short val_hosei, carry;
				unsigned short val, flag;
				
				if (c == 0)
				{
					if (h == 0)
					{
						val_hosei = 0;
						carry = 0;
					}
					else
					{
						val_hosei = 0xfa;
						carry = 0;
					}
				}
				else
				{
					if (h == 0)
					{
						val_hosei = 0xa0;
						carry = Flag_Cc;
					}
					else
					{
						val_hosei = 0x9a;
						carry = Flag_Cc;
					}
				}
				
				val = (idx + val_hosei) & 0xff;
				
				flag = 0;
				if (val & 0x80) flag |= Flag_S;
				if (val == 0) flag |= Flag_Z;
				flag |= Flag_N;
				flag |= Table_PEPO[val];
				flag |= carry;
				
				val_table[c][h][n][idx] = val;
				flag_table[c][h][n][idx] = flag;
			}
		}
	}
	
	fwrite(val_table,  1, sizeof val_table,  stdout);
	fwrite(flag_table, 1, sizeof flag_table, stdout);
	
	return 0;
}

int daa_test(void)
{
	unsigned char val_table[2][2][2][256];	/* C:H:N:val */
	unsigned char c_table[2][2][2][256];	/* C:H:N:val */
	signed short x, y, c;
	
	memset(val_table, 0xffff, sizeof val_table);
	memset(c_table, 0xffff, sizeof c_table);
	
	for (x = 0; x < 100; ++x)
	{
		for (y = 0; y < 100; ++y)
		{
			for (c = 0; c <= 1; ++c)
			{
				short hex_x = (x/10)*16 + (x % 10);
				short hex_y = (y/10)*16 + (y % 10);
				short half_x = x % 10;
				short half_y = y % 10;
				short dec_add = x + y + c;
				short dec_sub = x - y - c;
				short hex_add = hex_x + hex_y + c;
				short hex_sub = hex_x - hex_y - c;
				short carry, h_carry, n, idx, val, c_val;
				
				
				fprintf(stderr, "\r%2d : %2d : %d", (int)x, (int)y, (int)c);
				
				
				/* add : carry */
				if (hex_add > 0xff) carry = 1;
				else carry = 0;
				
				/* add : h_carry */
				if (half_x + half_y + c > 15) h_carry = 1;
				else h_carry = 0;
				
				/* add : N */
				n = 0;
				
				/* add : check */
				idx = hex_add & 0xff;
				val = ((dec_add % 100)/10)*16 + (dec_add % 10);
				if (dec_add > 99) c_val = 1;
				else c_val = 0;
				
				if (val_table[carry][h_carry][n][idx] == 0xffff)
				{
					val_table[carry][h_carry][n][idx] = val;
					c_table[carry][h_carry][n][idx] = c_val;
				}
				else
				{
					if ((val_table[carry][h_carry][n][idx] != val)
					 || (c_table[carry][h_carry][n][idx] != c_val))
					{
						fprintf(stderr, "C=%d, H=%d, N=%d, idx=%d\n",
							(int)carry,
							(int)h_carry,
							(int)n,
							(int)idx);
					}
				}
				
				/**/
				/* sub : carry */
				if (hex_sub < 0) carry = 1;
				else carry = 0;
				
				/* sub : h_carry */
				if (half_x - half_y - c < 0) h_carry = 1;
				else h_carry = 0;
				
				/* sub : N */
				n = 1;
				
				/* sub : check */
				idx = hex_sub & 0xff;
				if (dec_sub < 0)
				{
					c_val = 1;
					dec_sub = - dec_sub;
				}
				else
				{
					c_val = 0;
				}
				val = (dec_sub % 100)/10*16 + (dec_sub % 10);
				
				if (val_table[carry][h_carry][n][idx] == 0xffff)
				{
					val_table[carry][h_carry][n][idx] = val;
					c_table[carry][h_carry][n][idx] = c_val;
				}
				else
				{
					if ((val_table[carry][h_carry][n][idx] != val)
					 || (c_table[carry][h_carry][n][idx] != c_val))
					{
						fprintf(stderr, "C=%d, H=%d, N=%d, idx=%d\n",
							(int)carry,
							(int)h_carry,
							(int)n,
							(int)idx);
					}
				}
			}
		}
	}
	
	fprintf(stderr, "\n");
	
	{
		short c, h, n, idx;
		
		for (c = 0; c <= 1; ++c)
		{
			for (h = 0; h <= 1; ++h)
			{
				for (n = 0; n <= 1; ++n)
				{
					for (idx = 0; idx < 256; ++idx)
					{
						if (val_table[c][h][n][idx] == 0xffff)
						{
							fprintf(stderr, "%d:%d:%d:%3d",
								(int)c, (int)h,
								(int)n, (int)idx);
						
							fprintf(stderr, " not use\n");
						}
					}
				}
			}
		}
	}
	
	
	return 0;
}



unsigned char add_table[2][256*256];
int add_r_r(void)
{
	short src, dst, c, val;
	
	
	fprintf(stderr, "create ADD flag table.\n");
	
	for (src = 0; src < 256; ++src)
	{
		for (dst = 0; dst < 256; ++dst)
		{
			for (c = 0; c <= 1; ++c)
			{
				unsigned short flag = 0;
				
				val = dst + src + c;
				
				if (val & 0x80) flag |= Flag_S;
				
				if ((val & 0xff) == 0) flag |= Flag_Z;
				
				if (((dst & 0x0f) + (src & 0x0f) + c) > 15) flag |= Flag_H;
				
				{
					short x = (char)dst;
					short y = (char)src;
					
					if (((x + y + c) < -128)
					 || ((x + y + c) > 127))
						flag |= Flag_V;
				}
				
				if (val > 0xff) flag |= Flag_Cc;
				
				add_table[c][src * 256 + dst] = flag;
			}
		}
	}
	
	fwrite(add_table[0], 1, 256*256, stdout);
	fwrite(add_table[1], 1, 256*256, stdout);
	
	return 0;
}


unsigned char sub_table[2][256*256];
int sub_r(void)
{
	short src, dst, c, val;
	
	
	fprintf(stderr, "create SUB flag table.\n");
	
	for (src = 0; src < 256; ++src)
	{
		for (dst = 0; dst < 256; ++dst)
		{
			for (c = 0; c <= 1; ++c)
			{
				unsigned short flag = 0;
				
				val = dst - src - c;
				
				if (val & 0x80) flag |= Flag_S;
				
				if ((val & 0xff) == 0) flag |= Flag_Z;
				
				if (((dst & 0x0f) - (src & 0x0f) - c) < 0) flag |= Flag_H;
				
				{
					short x = (char)dst;
					short y = (char)src;
					
					if (((x - y - c) < -128)
					 || ((x - y - c) > 127))
						flag |= Flag_V;
				}
				
				flag |= Flag_N;
				
				if (val < 0) flag |= Flag_Cc;
				
				sub_table[c][src * 256 + dst] = flag;
			}
		}
	}
	
	fwrite(sub_table[0], 1, 256*256, stdout);
	fwrite(sub_table[1], 1, 256*256, stdout);
	
	
	return 0;
}


/* 演算後の値 */
int and_r(void)
{
	unsigned char and_table[256];
	short val;
	
	
	fprintf(stderr, "create AND flag table.\n");
	
	for (val = 0; val < 256; ++val)
	{
		unsigned short flag = 0;
		
		if (val & 0x80) flag |= Flag_S;
		
		if (val == 0) flag |= Flag_Z;
		
		flag |= Flag_H;
		
		flag |= Table_PEPO[val];
		
		and_table[val] = flag;
	}
	
	fwrite(and_table, 1, 256, stdout);
	
	return 0;
}


/* 演算後の値 */
int or_r(void)
{
	unsigned char or_table[256];
	short val;
	
	
	fprintf(stderr, "create OR/XOR flag table.\n");
	
	for (val = 0; val < 256; ++val)
	{
		unsigned short flag = 0;
		
		if (val & 0x80) flag |= Flag_S;
		
		if (val == 0) flag |= Flag_Z;
		
		flag |= Table_PEPO[val];
		
		or_table[val] = flag;
	}
	
	fwrite(or_table, 1, 256, stdout);
	
	return 0;
}


int pop_af(void)
{
	unsigned char pop_table[256];
	short idx;
	
	
	fprintf(stderr, "create POP AF table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		
		if (idx & 0b1000_0000) flag |= Flag_S;
		if (idx & 0b0100_0000) flag |= Flag_Z;
		if (idx & 0b0001_0000) flag |= Flag_H;
		if (idx & 0b0000_0100) flag |= Flag_P;
		if (idx & 0b0000_0010) flag |= Flag_N;
		if (idx & 0b0000_0001) flag |= Flag_Cc;
		
		pop_table[idx] = flag;
	}
	
	fwrite(pop_table, 1, 256, stdout);
	
	return 0;
}


int push_af(void)
{
	unsigned char push_table[256];
	short idx;
	
	
	fprintf(stderr, "create PUSH AF table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		
		if (idx & Flag_S) flag |= 0b1000_0000;
		if (idx & Flag_Z) flag |= 0b0100_0000;
		if (idx & Flag_H) flag |= 0b0001_0000;
		if (idx & Flag_P) flag |= 0b0000_0100;
		if (idx & Flag_N) flag |= 0b0000_0010;
		if (idx & Flag_C) flag |= 0b0000_0001;
		
		push_table[idx] = flag;
	}
	
	fwrite(push_table, 1, 256, stdout);
	
	return 0;
}


/* 演算前の値 */
int rlc_r(void)
{
	unsigned char rlc_table[256];
	short idx;
	
	
	fprintf(stderr, "create RLC flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		unsigned short val;
		
		val = (idx << 1) | ((idx & 0x80) ? 1 : 0);
		val &= 0xff;
		
		if (val & 0x80) flag |= Flag_S;
		if (val == 0) flag |= Flag_Z;
		flag |= Table_PEPO[val];
		if (idx & 0x80) flag |= Flag_Cc;
		
		rlc_table[idx] = flag;
	}
	
	fwrite(rlc_table, 1, 256, stdout);
	
	return 0;
}


/* 演算前の値 */
int rrc_r(void)
{
	unsigned char rrc_table[256];
	short idx;
	
	
	fprintf(stderr, "create RRC flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		unsigned short val;
		
		val = (idx >> 1) | ((idx & 0x01) ? 0x80 : 0);
		val &= 0xff;
		
		if (val & 0x80) flag |= Flag_S;
		if (val == 0) flag |= Flag_Z;
		flag |= Table_PEPO[val];
		if (idx & 0x01) flag |= Flag_Cc;
		
		rrc_table[idx] = flag;
	}
	
	fwrite(rrc_table, 1, 256, stdout);
	
	return 0;
}


/* 演算前の値 */
int rl_r(void)
{
	unsigned char rl_table[2][256];
	short idx, c;
	
	
	fprintf(stderr, "create RL flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		for (c = 0; c <= 1; ++c)
		{
			unsigned short flag = 0;
			unsigned short val;
			
			val = (idx << 1) | (c ? 1 : 0);
			val &= 0xff;
			
			if (val & 0x80) flag |= Flag_S;
			if (val == 0) flag |= Flag_Z;
			flag |= Table_PEPO[val];
			if (idx & 0x80) flag |= Flag_Cc;
			
			rl_table[c][idx] = flag;
		}
	}
	
	fwrite(rl_table[0], 1, 256, stdout);
	fwrite(rl_table[1], 1, 256, stdout);
	
	return 0;
}


int rr_r(void)
{
	unsigned char rr_table[2][256];
	short idx, c;
	
	
	fprintf(stderr, "create RR flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		for (c = 0; c <= 1; ++c)
		{
			unsigned short flag = 0;
			unsigned short val;
			
			val = (idx >> 1) | (c ? 0x80 : 0);
			val &= 0xff;
			
			if (val & 0x80) flag |= Flag_S;
			if (val == 0) flag |= Flag_Z;
			flag |= Table_PEPO[val];
			if (idx & 0x01) flag |= Flag_Cc;
			
			rr_table[c][idx] = flag;
		}
	}
	
	fwrite(rr_table[0], 1, 256, stdout);
	fwrite(rr_table[1], 1, 256, stdout);
	
	return 0;
}


int sla_r(void)
{
	unsigned char sla_table[256];
	short idx, c;
	
	
	fprintf(stderr, "create SLA flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		unsigned short val;
		
		val = (idx << 1);
		val &= 0xff;
		
		if (val & 0x80) flag |= Flag_S;
		if (val == 0) flag |= Flag_Z;
		flag |= Table_PEPO[val];
		if (idx & 0x80) flag |= Flag_Cc;
		
		sla_table[idx] = flag;
	}
	
	fwrite(sla_table, 1, 256, stdout);
	
	return 0;
}


int sra_r(void)
{
	unsigned char sra_table[256];
	short idx, c;
	
	
	fprintf(stderr, "create SRA flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		unsigned short val;
		
		val = (idx >> 1) | (idx & 0x80);
		val &= 0xff;
		
		if (val & 0x80) flag |= Flag_S;
		if (val == 0) flag |= Flag_Z;
		flag |= Table_PEPO[val];
		if (idx & 0x01) flag |= Flag_Cc;
		
		sra_table[idx] = flag;
	}
	
	fwrite(sra_table, 1, 256, stdout);
	
	return 0;
}


int srl_r(void)
{
	unsigned char srl_table[256];
	short idx, c;
	
	
	fprintf(stderr, "create SRL flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		unsigned short val;
		
		val = (idx >> 1);
		val &= 0xff;
		
		if (val & 0x80) flag |= Flag_S;
		if (val == 0) flag |= Flag_Z;
		flag |= Table_PEPO[val];
		if (idx & 0x01) flag |= Flag_Cc;
		
		srl_table[idx] = flag;
	}
	
	fwrite(srl_table, 1, 256, stdout);
	
	return 0;
}


int adc16(void)
{
	unsigned char adc16_table[32];
	short idx;
	
	fprintf(stderr, "create ADC16 flag table.\n");
	
	for (idx = 0; idx < 32; ++idx)
	{
		unsigned short flag = 0;
		
		// Negative
		if (idx & 0b01000) flag |= Flag_S;
		
		// Zero
		if (idx & 0b00100) flag |= Flag_Z;
		
		// Overflow
		if (idx & 0b00010) flag |= Flag_V;
		
		// Carry
		if (idx & 0b00001) flag |= Flag_Cc;
		
		adc16_table[idx] = flag;
	}
	
	fwrite(adc16_table, 1, 32, stdout);
	
	return 0;
}


int sbc16(void)
{
	unsigned char sbc16_table[32];
	short idx;
	
	fprintf(stderr, "create SBC16 flag table.\n");
	
	for (idx = 0; idx < 32; ++idx)
	{
		unsigned short flag = Flag_N;
		
		// Negative
		if (idx & 0b01000) flag |= Flag_S;
		
		// Zero
		if (idx & 0b00100) flag |= Flag_Z;
		
		// Overflow
		if (idx & 0b00010) flag |= Flag_V;
		
		// Carry
		if (idx & 0b00001) flag |= Flag_Cc;
		
		sbc16_table[idx] = flag;
	}
	
	fwrite(sbc16_table, 1, 32, stdout);
	
	return 0;
}


int rrld(void)
{
	unsigned char rrld_table[256];
	short idx;
	
	
	fprintf(stderr, "create RRD/RLD flag table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		
		if (idx & 0x80) flag |= Flag_S;
		if (idx == 0) flag |= Flag_Z;
		flag |= Table_PEPO[idx];
		
		rrld_table[idx] = flag;
	}
	
	fwrite(rrld_table, 1, 256, stdout);
	
	return 0;
}


int iff(void)
{
	unsigned char iff_table[256];
	short idx;
	
	fprintf(stderr, "create IFF table.\n");
	
	for (idx = 0; idx < 256; ++idx)
	{
		unsigned short flag = 0;
		
		if (idx & 0x80) flag |= Flag_S;
		if (idx == 0) flag |= Flag_Z;
		
		iff_table[idx] = flag;
	}
	
	fwrite(iff_table, 1, 256, stdout);
	
	return 0;
}



int main(int argc, char *argv[])
{
	if (argc < 2) return 1;
	
	fmode(stdout, _IOBINARY);
	
	create_table_PEPO();
	
	if (strcmp(argv[1], "INC") == 0) inc_r();
	else if (strcmp(argv[1], "DEC") == 0) dec_r();
	else if (strcmp(argv[1], "DAA") == 0) daa();
	else if (strcmp(argv[1], "ADD") == 0) add_r_r();
	else if (strcmp(argv[1], "SUB") == 0) sub_r();
	else if (strcmp(argv[1], "AND") == 0) and_r();
	else if (strcmp(argv[1], "OR")  == 0) or_r();
	else if (strcmp(argv[1], "POP_AF") == 0) pop_af();
	else if (strcmp(argv[1], "PUSH_AF") == 0) push_af();
	else if (strcmp(argv[1], "RLC") == 0) rlc_r();
	else if (strcmp(argv[1], "RRC") == 0) rrc_r();
	else if (strcmp(argv[1], "RL") == 0) rl_r();
	else if (strcmp(argv[1], "RR") == 0) rr_r();
	else if (strcmp(argv[1], "SLA") == 0) sla_r();
	else if (strcmp(argv[1], "SRA") == 0) sra_r();
	else if (strcmp(argv[1], "SRL") == 0) srl_r();
	else if (strcmp(argv[1], "ADC16") == 0) adc16();
	else if (strcmp(argv[1], "SBC16") == 0) sbc16();
	else if (strcmp(argv[1], "RRD") == 0) rrld();
	else if (strcmp(argv[1], "RLD") == 0) rrld();
	else if (strcmp(argv[1], "IFF") == 0) iff();
	else return 1;
	
	return 0;
}
