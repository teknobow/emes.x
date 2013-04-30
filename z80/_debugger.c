//---------------------------------------------------------------------------------------------
// MSX Emulator for X680x0 - emes.x
//
//    Copyright 1997-1998 teknobow
//


#define __DOS_INLINE__
#include <sys/dos.h>


extern void _changeCRTmode(short mode);
extern short CRTmode;

/* BC' : workBC2
 * DE' : workDE2
 * HL' : workHL2
 * IX  : workIX
 * IY  : workIY
 * I   : workI
 * IFF : Z80_DIEI
 */

extern unsigned short workBC2, workDE2, workHL2, workIX, workIY, workI;
extern unsigned char Z80_DIEI;



int Z80debugger(	int regA,	/* d3 */
			int regF,	/* d4 */
			int regBC,	/* d5 */
			int regDE,	/* d6 */
			int regHL,	/* d7 */
			int regVadr,	/* a2 */
			int retIO,	/* a3 */
			int regZjmp,	/* a4 */
			int regPC,	/* a5 */
			int regSP	/* a6 */	)
{
	char *z80mem = (char *)(regHL & 0xffff0000);
	short saveCRTmode = CRTmode;
	short d;
	char *hextab = "0123456789ABCDEF";
	
	/* ‰æ–ÊÝ’è */
//	_changeCRTmode(5);
	
	_dos_putchar(hextab[(regPC & 0xf000) >> 12]);
	_dos_putchar(hextab[(regPC & 0x0f00) >>  8]);
	_dos_putchar(hextab[(regPC & 0x00f0) >>  4]);
	_dos_putchar(hextab[(regPC & 0x000f)]);
	_dos_putchar(':');
	_dos_putchar(hextab[(*(char *)regPC & 0xf0) >> 4]);
	_dos_putchar(hextab[(*(char *)regPC & 0x0f)]);
	_dos_putchar(' ');
	
//	_dos_kflushgc();
	
//	_changeCRTmode(saveCRTmode);
	
	return 0;
}

