;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;



*----------------------------------------------------------------------------------------------

*		Z80 state  (HD64180 state)		* 68000 state

*--- NOP		 4 ( 3)			SZ H VNC	CSZ H VN
*						･･ ･ ･･･	･･･ ･ ･･
	opcode 00
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* 
	endopcode 00



*--- LD BC,mn		10 ( 9)			SZ H VNC
*						･･ ･ ･･･
	opcode 01
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regBC				* 16
	move.b	(regPC),regBC				*  8
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode 01



*--- LD (BC),A		 7 ( 7)			SZ H VNC
*						･･ ･ ･･･
	opcode 02
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regBC,ExtSlotSrc_A,1		* 
	
	CHECK_ROM	regBC,d0			* 
	
	movea.l	regBC,a0				*  4
	move.b	regA,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_BC_A(regFtbl)			* 
	endopcode 02



*--- INC BC		 6 ( 4)			SZ H VNC
*						･･ ･ ･･･
	opcode 03
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	addq.w	#1,regBC				*  4
	
	FETCH						* 
	endopcode 03



*--- INC B		 4 ( 4)			SZ H VNC
*						** * V0･
	opcode 04
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,d0				*  4
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblINCrh(regZjmp,d0.w),regF		* 14
	
*	addi.w	#$0100,regBC				*  8
	add.w	regZjmp,regBC				*  4
	
	FETCH						* 
	endopcode 04



*--- DEC B		4 (4)			SZ H VNC
*						** * V1･
	opcode 05
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,d0				*  4
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblDECrh(regZjmp,d0.w),regF		* 14
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	
	FETCH						* 
	endopcode 05



*--- LD B,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 06
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regPC)+,(regSwap)			* 12
	move.w	(regSwap)+,regBC			*  8 = 28
	
	FETCH						* 
	endopcode 06



*--- RLCA		4 (3)			SZ H VNC
*						･･ 0 ･0*
	opcode 07
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	___H__NCc				*  8
	rol.b	#1,regA					*  8
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 07



*--- EX AF,AF'		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 08
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	swap.w	regF					*  4
	swap.w	regA					*  4
	
	FETCH						* =  8
	endopcode 08



*--- ADD HL,BC		11 (7)			SZ H VNC
*						･･ x ･0*
	opcode 09
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	______NCc				*  8
	
	add.w	regBC,regHL				*  4
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 09



*--- LD A,(BC)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 0A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regBC,a0				*  4
	move.b	(a0),regA				*  8
	
	FETCH						* = 12
	endopcode 0A



*--- DEC BC		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 0B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	subq.w	#1,regBC				*  4
	
	FETCH						* 
	endopcode 0B



*--- INC C		4 (4)			SZ H VNC
*						** * V0･
	opcode 0C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblINCrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regBC,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	addq.b	#1,regBC				*  4
	
	FETCH						* 
	endopcode 0C



*--- DEC C		4 (4)			SZ H VNC
*						** * V1･
	opcode 0D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblDECrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regBC,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	subq.b	#1,regBC				*  4
	
	FETCH						* 
	endopcode 0D



*--- LD C,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 0E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regBC				*  8
	
	FETCH						* 
	endopcode 0E



*--- RRCA		4 (3)			SZ H VNC
*						･･ 0 ･0*
	opcode 0F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	___H__NCc				*  8
	ror.b	#1,regA					*  8
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 0F



*--- DJNZ e		13 (9) / 18 (7)		SZ H VNC
*			(B != 0)/(B==0)		･･ ･ ･･･
	opcode 10
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bls.s	@f					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcs.s	@f					* 10/ 8 (T/F)
**		move.b	(regPC)+,d0			*     8
**		ext.w	d0				*     4
**		adda.w	d0,regPC			*     8
		
		move.b	(regPC)+,regAdr			*     8
		ext.w	regAdr				*     4
		adda.w	regAdr,regPC			*     8
		; Z80メモリの範囲を越えた場合は HALT で補正される
		
		FETCH					* 
	@@:
		addq.w	#1,regPC			*  8
		FETCH					* 
	endopcode 10



*--- LD DE,mn		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode 11
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regDE				* 16
	move.b	(regPC),regDE				*  8
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode 11



*--- LD (DE),A		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 12
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regDE,ExtSlotSrc_A,1		* 
	
	CHECK_ROM	regDE,d0			* 
	
	movea.l	regDE,a0				*  4
	move.b	regA,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_DE_A(regFtbl)			* 
	endopcode 12



*--- INC DE		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 13
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	addq.w	#1,regDE				*  4
	
	FETCH						* 
	endopcode 13



*--- INC D		4 (4)			SZ H VNC
*						** * V0･
	opcode 14
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d0				*  4
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblINCrh(regZjmp,d0.w),regF		* 14
	
*	addi.w	#$0100,regDE				*  8
	add.w	regZjmp,regDE				*  4
	
	FETCH						* 
	endopcode 14



*--- DEC D		4 (4)			SZ H VNC
*						** * V1･
	opcode 15
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d0				*  4
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblDECrh(regZjmp,d0.w),regF		* 14
	
*	subi.w	#$0100,regDE				*  8
	sub.w	regZjmp,regDE				*  4
	
	FETCH						* 
	endopcode 15



*--- LD D,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 16
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regPC)+,(regSwap)			* 12
	move.w	(regSwap)+,regDE			*  8
	
	FETCH						* = 28
	endopcode 16



*--- RLA		4 (3)			SZ H VNC	CSZcH VN
*						･･ 0 ･0*	*･･*0 ･0
	opcode 17
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	
	Fclr	___H__NCc				*  8
	
	add.b	d0,d0					*  4
	roxl.b	#1,regA					*  8
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 17



*--- JR e		12 (8)			SZ H VNC
*						･･ ･ ･･･
	opcode 18
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
**	-- d0が符号拡張されるためNG
**	move.b	(regPC)+,d0				*  8
**	ext.w	d0					*  4
**	adda.w	d0,regPC				*  8
	
	
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	adda.w	regAdr,regPC				*  8
	; メモリの範囲を越えた場合は HALT で補正される
	
	
	FETCH						* 
	endopcode 18



*--- ADD HL,DE		11 (7)			SZ H VNC
*						･･ x ･0*
	opcode 19
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	______NCc				*  8
	
	add.w	regDE,regHL				*  4
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 19



*--- LD A,(DE)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 1A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regDE,a0				*  4
	move.b	(a0),regA				*  8
	
	FETCH						* = 12
	endopcode 1A



*--- DEC DE		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 1B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	subq.w	#1,regDE				*  4
	
	FETCH						* =  4
	endopcode 1B



*--- INC E		4 (4)			SZ H VNC
*						** * V0･
	opcode 1C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblINCrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regDE,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	addq.b	#1,regDE				*  4
	
	FETCH						* 
	endopcode 1C



*--- DEC E		4 (4)			SZ H VNC
*						** * V1･
	opcode 1D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblDECrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regDE,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	subq.b	#1,regDE				*  4
	
	FETCH						* 
	endopcode 1D



*--- LD E,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 1E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regDE				*  8
	
	FETCH						* =  8
	endopcode 1E



*--- RRA		4 (3)			SZ H VNC
*						･･ 0 ･0*
	opcode 1F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	
	Fclr	___H__NCc				*  8
	
	add.b	d0,d0					*  4
	roxr.b	#1,regA					*  8
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 1F



*--- JR NZ,e		12 (8) / 7 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode 20
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regAdr				*  8
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
		ext.w	regAdr				*     4
		adda.w	regAdr,regPC			*     8
		; メモリの範囲を越えた場合は HALT で補正される
@@:
	FETCH						* 
	endopcode 20



*--- LD HL,mn		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode 21
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regHL				* 16
	move.b	(regPC),regHL				*  8
	addq.w	#2,regPC				*  8
	
	
**	move.b	(regPC)+,-(sp)				* 12
**	move.b	(regPC)+,(sp)				* 12
**	move.w	(sp)+,regHL				*  8
	
	
	FETCH						*  
	endopcode 21



*--- LD (mn),HL		16 (16)			SZ H VNC
*						･･ ･ ･･･
*	!! ページの境界をまたいだ書き込みのチェックはしていない
*	!! 上位バイトの書き込みのみチェック
	opcode 22
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	
	CHECK_ROM regAdr,d0				* 
	
	movea.l	regAdr,a0				*  4
	move.b	regHL,(a0)+				*  8
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_mn_HL(regFtbl)			* 
	endopcode 22



*--- INC HL		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 23
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	addq.w	#1,regHL				*  4
	
	FETCH						* =  4
	endopcode 23



*--- INC H		4 (4)			SZ H VNC
*						** * V0･
	opcode 24
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d0				*  4
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblINCrh(regZjmp,d0.w),regF		* 14
	
*	addi.w	#$0100,regHL				*  8
	add.w	regZjmp,regHL				*  4
	
	FETCH						* 
	endopcode 24



*--- DEC H		4 (4)			SZ H VNC
*						** * V1･
	opcode 25
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d0				*  4
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblDECrh(regZjmp,d0.w),regF		* 14
	
*	subi.w	#$0100,regHL				*  8
	sub.w	regZjmp,regHL				*  4
	
	FETCH						* 
	endopcode 25



*--- LD H,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 26
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regPC)+,(regSwap)			* 12
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* = 28
	endopcode 26



*--- DAA		4 (4)			SZ-H-VNC	CSZcH VN
*						** * P･*
	opcode 27
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	*-- flag_c (0001_0000)
	*-- flag_H (0000_1000)
	*-- flag_N (0000_0001)
	
	moveq.l	#flag_c.or.flag_H,d0			*  4	     000c_H000
	and.b	regF,d0					*  4
	
	andi.b	#flag_N,regF				*  8	     0000_000N
	add.b	regF,regF				*  4
	add.b	regF,regF				*  4
	or.b	regF,d0					*  4	      000cHN00
	
	lsl.w	#6,d0					* 18	 cHN_0000_0000
	
	move.b	regA,d0					*  4	 cHN_AAAA_AAAA
	add.w	d0,d0					*  4	cHNA_AAAA_AAA0
	lea	FtblDAA(regFtbl),a0			*  8
	move.b	0(a0,d0.w),regA				* 14
	move.b	1(a0,d0.w),regF				* 14
	
	FETCH						* 
	endopcode 27



*--- JR Z,e		12 (8) / 7 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode 28
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regAdr				*  8
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
		ext.w	regAdr				*     4
		adda.w	regAdr,regPC			*     8
		; メモリの範囲を越えた場合は HALT で補正される
@@:
	FETCH						* 
	endopcode 28



*--- ADD HL,HL		11 (7)			SZ H VNC
*						･･ x ･0*
	opcode 29
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	______NCc				*  8
	
	add.w	regHL,regHL				*  4
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 29



*--- LD HL,(mn)		16 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode 2A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	movea.l	regAdr,a0				*  4
	movep.w	1(a0),regHL				* 16
	move.b	(a0),regHL				*  8
	
	FETCH						* 
	endopcode 2A



*--- DEC HL		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 2B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	subq.w	#1,regHL				*  4
	
	FETCH						* =  4
	endopcode 2B



*--- INC L		4 (4)			SZ H VNC
*						** * V0･
	opcode 2C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblINCrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regHL,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	addq.b	#1,regHL				*  4
	
	FETCH						* 
	endopcode 2C



*--- DEC L		4 (4)			SZ H VNC
*						** * V1･
	opcode 2D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblDECrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regHL,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	subq.b	#1,regHL				*  4
	
	FETCH						* 
	endopcode 2D



*--- LD L,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 2E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regHL				*  8
	
	FETCH						* = 8
	endopcode 2E



*--- CPL		4 (3)			SZ H VNC
*						･･ 1 ･1･
	opcode 2F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	not.b	regA					*  4
	Fset	___H__N_				*  8
	
	FETCH						* = 12
	endopcode 2F



*--- JR NC,e		12 (8) / 7 (6)		SZ H VNC	CSZcH-VN
*						･･ ･ ･･･	･････ ･･
	opcode 30
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regAdr				*  8
	
*	moveq.l	#flag_C,d0				*  4
*	and.b	regF,d0					*  4
*	bne.s	@f					* 10/ 8 (T/F)
	tst.b	regF					*  4
	bmi.s	@f					* 10/ 8 (T/F)
		ext.w	regAdr				*     4
		adda.w	regAdr,regPC			*     8
		; メモリの範囲を越えた場合は HALT で補正される
@@:
	FETCH						* 
	endopcode 30



*--- LD SP,mn		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode 31
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	movea.l	regAdr,regSP				*  4
	
	FETCH						* = 36
	endopcode 31



*--- LD (mn),A		13 (13)			SZ H VNC
*						･･ ･ ･･･
	opcode 32
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_A,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.b	regA,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_mn_A(regFtbl)			* 
	endopcode 32



*--- INC SP		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 33
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	addq.w	#1,regSP				*  8
	
	FETCH						* 
	endopcode 33



*--- INC (HL)		11 (10)			SZ H VNC
*						** * V0･
*	!! ROM領域への書き込みのチェックはしない
	opcode 34
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	ext.w	regF					*  4
	move.b	(a0),regF				*  8
	lea	FtblINCrl(regFtbl),a1			*  8
	move.b	(a1,regF.w),regF			* 14
	
	addq.b	#1,(a0)					* 12
	
	FETCH						* 
	endopcode 34



*--- DEC (HL)		6 (4)			SZ H VNC
*						** * V1･
*	!! ROM領域への書き込みのチェックはしない
	opcode 35
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	ext.w	regF					*  4
	move.b	(a0),regF				*  8
	lea	FtblDECrl(regFtbl),a1			*  8
	move.b	(a1,regF.w),regF			* 14
	
	subq.b	#1,(a0)					* 12
	
	FETCH						* 
	endopcode 35



*--- LD (HL),n		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode 36
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_n,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	movea.l	regHL,a0				*  4
	move.b	(regPC)+,(a0)				* 12
	
@@:
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_n(regFtbl)			* 
	endopcode 36



*--- SCF		4 (3)			SZ H VNC
*						･･ 0 ･01
	opcode 37
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fset	_______Cc				*  8
	Fclr	___H__N_				*  8
	
	FETCH						* = 16
	endopcode 37



*--- JR C,e		12 (8) / 7 (6)		SZ H VNC	CSZcH VN
*						･･ ･ ･･･	･････ ･･
	opcode 38
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regAdr				*  8
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bpl.s	@f					* 10/ 8 (T/F)
.endif
		ext.w	regAdr				*     4
		adda.w	regAdr,regPC			*     8
		; メモリの範囲を越えた場合は HALT で補正される
@@:
	FETCH						* 
	endopcode 38



*--- ADD HL,SP		11 (7)			SZ H VNC
*						･･ x ･0*
	opcode 39
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	______NCc				*  8
	add.w	regSP,regHL				*  4
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode 39



*--- LD A,(mn)		13 (12)			SZ H VNC
*						･･ ･ ･･･
	opcode 3A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	move.l	regAdr,a0				*  4
	move.b	(a0),regA				*  8
	
	FETCH						* 
	endopcode 3A



*--- DEC SP		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 3B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	subq.w	#1,regSP				*  8
	
	FETCH						* 
	endopcode 3B



*--- INC A		4 (4)			SZ H VNC
*						** * V0･
	opcode 3C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblINCrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regA,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	addq.b	#1,regA					*  4
	
	FETCH						* 
	endopcode 3C



*--- DEC A		4 (4)			SZ H VNC
*						** * V1･
	opcode 3D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	FtblDECrl(regFtbl),a1			*  8
	ext.w	regF					*  4
	move.b	regA,regF				*  4
	move.b	(a1,regF.w),regF			* 14
	
	subq.b	#1,regA					*  4
	
	FETCH						* 
	endopcode 3D



*--- LD A,n		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 3E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,regA				*  8
	
	FETCH						* =  8
	endopcode 3E



*--- CCF		4 (3)			SZ H VNC	CSZcH VN
*						･･ x ･0*	*･･*x ･0
	opcode 3F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	Fclr	______N_				*  8
	bmi.s	@f					* 10/ 8 (T/F)
	Fset	_______Cc				*  8
	
	FETCH						* 
@@:
	Fclr	_______Cc				*  8
	
	FETCH						* 
	endopcode 3F



*--- LD B,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 40
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* = 0
	endopcode 40



*--- LD B,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 41
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	regBC,(regSwap)				*  8
	move.w	(regSwap)+,regBC			*  8
	
	FETCH						* = 24
	endopcode 41



*--- LD B,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 42
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,d0				*  4
	move.w	regDE,regBC				*  4
	move.b	d0,regBC				*  4
	
	FETCH						* = 12
	endopcode 42



*--- LD B,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 43
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	regDE,(regSwap)				*  8
	move.w	(regSwap)+,regBC			*  8
	
	FETCH						* = 24
	endopcode 43



*--- LD B,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 44
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,d0				*  4
	move.w	regHL,regBC				*  4
	move.b	d0,regBC				*  4
	
	FETCH						* = 12
	endopcode 44



*--- LD B,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 45
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	regHL,(regSwap)				*  8
	move.w	(regSwap)+,regBC			*  8
	
	FETCH						* = 24
	endopcode 45



*--- LD B,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 46
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,d0				*  4
	movea.l	regHL,a0				*  4
	movep.w	(a0),regBC				* 16
	move.b	d0,regBC				*  4
	
	
	FETCH						* = 32
	endopcode 46



*--- LD B,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 47
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	regA,(regSwap)				*  8
	move.w	(regSwap)+,regBC			*  8
	
	FETCH						* = 24
	endopcode 47



*--- LD C,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 48
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,regBC			*  8
	
	FETCH						* = 16
	endopcode 48



*--- LD C,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 49
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* =  0
	endopcode 49



*--- LD C,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 4A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,regBC			*  8
	
	FETCH						* = 16
	endopcode 4A



*--- LD C,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 4B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,regBC				*  4
	
	FETCH						* =  4
	endopcode 4B



*--- LD C,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 4C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,regBC			*  8
	
	FETCH						* = 16
	endopcode 4C



*--- LD C,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 4D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,regBC				*  4
	
	FETCH						* =  4
	endopcode 4D



*--- LD C,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 4E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),regBC				*  8
	
	FETCH						* = 12
	endopcode 4E



*--- LD C,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 4F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,regBC				*  4
	
	FETCH						* =  4
	endopcode 4F



*--- LD D,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 50
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,d0				*  4
	move.w	regBC,regDE				*  4
	move.b	d0,regDE				*  4
	
	FETCH						* = 12
	endopcode 50



*--- LD D,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 51
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	regBC,(regSwap)				*  8
	move.w	(regSwap)+,regDE			*  8
	
	FETCH						* = 24
	endopcode 51



*--- LD D,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 52
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* =  0
	endopcode 52



*--- LD D,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 53
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	regDE,(regSwap)				*  8
	move.w	(regSwap)+,regDE			*  8
	
	FETCH						* = 24
	endopcode 53



*--- LD D,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 54
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,d0				*  4
	move.w	regHL,regDE				*  4
	move.b	d0,regDE				*  4
	
	FETCH						* = 12
	endopcode 54



*--- LD D,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 55
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	regHL,(regSwap)				*  8
	move.w	(regSwap)+,regDE			*  8
	
	FETCH						* = 24
	endopcode 55



*--- LD D,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 56
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,d0				*  4
	movea.l	regHL,a0				*  4
	movep.w	(a0),regDE				* 16
	move.b	d0,regDE				*  4
	
	
	FETCH						* = 32
	endopcode 56



*--- LD D,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 57
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	regA,(regSwap)				*  8
	move.w	(regSwap)+,regDE			*  8
	
	FETCH						* = 24
	endopcode 57



*--- LD E,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 58
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,regDE			*  8
	
	FETCH						* = 16
	endopcode 58



*--- LD E,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 59
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,regDE				*  4
	
	FETCH						* =  4
	endopcode 59



*--- LD E,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 5A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,regDE			*  8
	
	FETCH						* = 16
	endopcode 5A



*--- LD E,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 5B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* =  0
	endopcode 5B



*--- LD E,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 5C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,regDE			*  8
	
	FETCH						* = 16
	endopcode 5C



*--- LD E,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 5D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,regDE				*  4
	
	FETCH						* =  4
	endopcode 5D



*--- LD E,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 5E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),regDE				*  8
	
	FETCH						* = 12
	endopcode 5E



*--- LD E,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 5F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,regDE				*  4
	
	FETCH						* =  4
	endopcode 5F



*--- LD H,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 60
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,d0				*  4
	move.w	regBC,regHL				*  4
	move.b	d0,regHL				*  4
	
	FETCH						* = 12
	endopcode 60



*--- LD H,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 61
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	regBC,(regSwap)				*  8
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* = 24
	endopcode 61



*--- LD H,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 62
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,d0				*  4
	move.w	regDE,regHL				*  4
	move.b	d0,regHL				*  4
	
	FETCH						* = 12
	endopcode 62



*--- LD H,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 63
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	regDE,(regSwap)				*  8
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* = 24
	endopcode 63



*--- LD H,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 64
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* =  0
	endopcode 64



*--- LD H,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 65
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	regHL,(regSwap)				*  8
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* = 24
	endopcode 65



*--- LD H,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 66
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,d0				*  4
	movea.l	regHL,a0				*  4
	movep.w	(a0),regHL				* 16
	move.b	d0,regHL				*  4
	
	FETCH						* = 32
	endopcode 66



*--- LD H,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 67
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	regA,(regSwap)				*  8
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* = 24
	endopcode 67



*--- LD L,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 68
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,regHL			*  8
	
	FETCH						* = 16
	endopcode 68



*--- LD L,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 69
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,regHL				*  4
	
	FETCH						* =  4
	endopcode 69



*--- LD L,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 6A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,regHL			*  8
	
	FETCH						* = 16
	endopcode 6A



*--- LD L,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 6B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,regHL				*  4
	
	FETCH						* =  4
	endopcode 6B



*--- LD L,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 6C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,regHL			*  8
	
	FETCH						* = 16
	endopcode 6C



*--- LD L,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 6D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* =  0
	endopcode 6D



*--- LD L,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 6E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),regHL				* 12
	
	FETCH						* = 16
	endopcode 6E



*--- LD L,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 6F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,regHL				*  4
	
	FETCH						* =  4
	endopcode 6F



*--- LD (HL),B		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 70
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_B,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	move.w	regBC,-(regSwap)			*  8
	movea.l	regHL,a0				*  4
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_B(regFtbl)			* 
	endopcode 70



*--- LD (HL),C		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 71
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_C,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	movea.l	regHL,a0				*  4
	move.b	regBC,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_C(regFtbl)			* 
	endopcode 71



*--- LD (HL),D		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 72
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_D,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	move.w	regDE,-(regSwap)			*  8
	movea.l	regHL,a0				*  4
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_D(regFtbl)			* 
	endopcode 72



*--- LD (HL),E		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 73
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_E,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	movea.l	regHL,a0				*  4
	move.b	regDE,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_E(regFtbl)			* 
	endopcode 73



*--- LD (HL),H		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 74
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_H,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	move.w	regHL,-(regSwap)			*  8
	movea.l	regHL,a0				*  4
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_H(regFtbl)			* 
	endopcode 74



*--- LD (HL),L		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 75
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_L,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	movea.l	regHL,a0				*  4
	move.b	regHL,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_L(regFtbl)			* 
	endopcode 75



*--- HALT		4 (3)			SZ H VNC
*						･･ ･ ･･･
	opcode 76
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	*-- regPC 補正
	move.w	regPC,regAdr				*  4
	subq.w	#1,regAdr				*  4
	movea.l	regAdr,regPC				*  4
	
	*-- 補正後の命令が HALT でなければその命令を実行
	move.b	(regPC)+,(regOP1fetch)			* 12
	cmpi.b	#$76,(regOP1fetch)			* 
	bne.w	_fetch_for_halt_2			* 10
	
	*-- 特殊命令番号
	move.b	(regPC),(regOP2fetch)			* 12
	move.w	(regOP2fetch),d0			*  8
	
	*-- アドレスチェック
	cmp.w	CHK_SPECIAL_ADR(regZjmp,d0.w),regAdr	* 14
	bne.w	_fetch_for_halt_1			* 10
	
	*-- 特殊命令実行
	addq.w	#1,regPC				*  8
	move.w	SPECIAL_PROC(regZjmp,d0.w),d1		* 14
	jmp	(regFtbl,d1.l)				* 14
	endopcode 76



*--- LD (HL),A		7 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode 77
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	CHECK_EXTSLOT	regHL,ExtSlotSrc_A,1		* 
	
	CHECK_ROM	regHL,d0			* 
	
	movea.l	regHL,a0				*  4
	move.b	regA,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_HL_A(regFtbl)			* 
	endopcode 77



*--- LD A,B		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 78
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,regA				*  8
	
	FETCH						* = 16
	endopcode 78



*--- LD A,C		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 79
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,regA				*  4
	
	FETCH						* =  4
	endopcode 79



*--- LD A,D		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 7A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,regA				*  8
	
	FETCH						* = 16
	endopcode 7A



*--- LD A,E		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 7B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,regA				*  4
	
	FETCH						* =  4
	endopcode 7B



*--- LD A,H		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 7C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,regA				*  8
	
	FETCH						* = 16
	endopcode 7C



*--- LD A,L		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 7D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,regA				*  4
	
	FETCH						* =  4
	endopcode 7D



*--- LD A,(HL)		7 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode 7E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),regA				*  8
	
	FETCH						* = 12
	endopcode 7E



*--- LD A,A		4 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode 7F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	FETCH						* =  0
	endopcode 7F



*--- ADD A,B		4 (4)			SZ H VNC
*						** * V0*
	opcode 80
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,d1				*  4
	move.b	regA,d1					*  4
	
	move.w	regBC,-(regSwap)			*  8
	add.b	(regSwap)+,regA				*  8
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 80



*--- ADD A,C		4 (4)			SZ H VNC
*						** * V0*
	opcode 81
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4
	
	add.b	regBC,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 81



*--- ADD A,D		4 (4)			SZ H VNC
*						** * V0*
	opcode 82
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d1				*  4
	move.b	regA,d1					*  4
	
	move.w	regDE,-(regSwap)			*  8
	add.b	(regSwap)+,regA				*  8
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 82



*--- ADD A,E		4 (4)			SZ H VNC
*						** * V0*
	opcode 83
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4
	
	add.b	regDE,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 83



*--- ADD A,H		4 (4)			SZ H VNC
*						** * V0*
	opcode 84
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d1				*  4
	move.b	regA,d1					*  4
	
	move.w	regHL,-(regSwap)			*  8
	add.b	(regSwap)+,regA				*  8
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 84



*--- ADD A,L		4 (4)			SZ H VNC
*						** * V0*
	opcode 85
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4
	
	add.b	regHL,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 85



*--- ADD A,(HL)		7 (6)			SZ H VNC
*						** * V0*
	opcode 86
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap),d1				*  8
	move.b	regA,d1					*  4
	
	add.b	(regSwap)+,regA				*  8
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 62
	endopcode 86



*--- ADD A,A		4 (4)			SZ H VNC
*						** * V0*
	opcode 87
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4
	
	add.b	regA,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 87



*--- ADC A,B		4 (4)			SZ H VNC
*						** * V0*
	opcode 88
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
* type 1 {
	move.w	regBC,d1				*  4
	move.b	regF,d0					*  4
	smi.b	d1					*  6/ 4 (T/F)
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	regBC,d0				*  4
	move.b	regA,d0					*  4
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	add.b	tblADCrh(regZjmp,d1.w),regA		* 14 = 70/68
* }
	
	
* type 2 {	この方法だとregFをロングワード使用してしまうので
*		アドレスレジスタを１本, regFの退避用に持たなければ
*		ならない (そこまでする必要がある？ 4clockのために...)
*	move.w	regBC,d0				*  4
*	ext.w	regF					*  4
*	smi.b	d0					*  6/ 4 (T/F)
*	ext.l	regF					*  4
*	move.w	regBC,regF				*  4
*	move.b	regA,d0					*  4
*	lea	FtblADD8,a1				* 12
*	move.b	(a1,regF.l),regF			* 14
*	add.b	tblADCrh(regZjmp,d0.l),regA		* 14 = 66/64
* }
	
* type 3 {
*	move.w	regBC,d1				*  4
*	clr.b	d1					*  4
*	
*	move.b	regF,d0					*  4
*	ext.w	d0					*  4
*	ext.l	d0					*  4
*	move.w	regBC,d0				*  4
*	move.b	regA,d0					*  4
*	lea	FtblADD8,a1				* 12
*	add.b	regF,regF				*  4
*	move.b	(a1,d0.l),regF				* 14
*	
*	addx.b	tblADCrh(regZjmp,d1.w),regA		* 14 = 72
* }
	
	FETCH					* 
	endopcode 88



*--- ADC A,C		4 (4)			SZ H VNC
*						** * V0*
	opcode 89
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regBC,-(regSwap)			*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4
	
	add.b	regF,regF				*  4
	addx.b	regBC,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* = 66
	endopcode 89



*--- ADC A,D		4 (4)			SZ H VNC
*						** * V0*
	opcode 8A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d1				*  4
	move.b	regF,d0					*  4
	smi.b	d1					*  6/ 4 (T/F)
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	regDE,d0				*  4
	move.b	regA,d0					*  4
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	add.b	tblADCrh(regZjmp,d1.w),regA		* 14 = 70/68
	
	FETCH						* 
	endopcode 8A



*--- ADC A,E		4 (4)			SZ H VNC
*						** * V0*
	opcode 8B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regDE,-(regSwap)			*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4
	
	add.b	regF,regF				*  4
	addx.b	regDE,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* 
	endopcode 8B



*--- ADC A,H		4 (4)			SZ H VNC
*						** * V0*
	opcode 8C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d1				*  4
	move.b	regF,d0					*  4
	smi.b	d1					*  6/ 4 (T/F)
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	regHL,d0				*  4
	move.b	regA,d0					*  4
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	add.b	tblADCrh(regZjmp,d1.w),regA		* 14 = 70/68
	
	FETCH						* 
	endopcode 8C



*--- ADC A,L		4 (4)			SZ H VNC
*						** * V0*
	opcode 8D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regHL,-(regSwap)			*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4
	
	add.b	regF,regF				*  4
	addx.b	regHL,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* 
	endopcode 8D



*--- ADC A,(HL)		7 (6)			SZ H VNC
*						** * V0*
	opcode 8E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(a0),d0					*  8
	
	add.b	regF,regF				*  4
	addx.b	d0,regA					*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* = 74
	endopcode 8E



*--- ADC A,A		4 (4)			SZ H VNC
*						** * V0*
	opcode 8F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4
	
	add.b	regF,regF				*  4
	addx.b	regA,regA				*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* = 66
	endopcode 8F



*--- SUB B		4 (4)			SZ H VNC
*						** * V1*
	opcode 90
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,d1				*  4
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	move.w	regBC,-(regSwap)			*  8
	sub.b	(regSwap)+,regA				*  8
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 90



*--- SUB C		4 (4)			SZ H VNC
*						** * V1*
	opcode 91
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	regBC,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 91



*--- SUB D		4 (4)			SZ H VNC
*						** * V1*
	opcode 92
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d1				*  4
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	move.w	regDE,-(regSwap)			*  8
	sub.b	(regSwap)+,regA				*  8
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 92



*--- SUB E		4 (4)			SZ H VNC
*						** * V1*
	opcode 93
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	regDE,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 93



*--- SUB H		4 (4)			SZ H VNC
*						** * V1*
	opcode 94
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d1				*  4
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	move.w	regHL,-(regSwap)			*  8
	sub.b	(regSwap)+,regA				*  8
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 94



*--- SUB L		4 (4)			SZ H VNC
*						** * V1*
	opcode 95
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	regHL,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 95



*--- SUB (HL)		7 (6)			SZ H VNC
*						** * V1*
	opcode 96
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	(a0),regA				*  8
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode 96



*--- SUB A		4 (4)			SZ H VNC
*						** * V1*
	opcode 97
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	regA,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* = 50
	endopcode 97



*--- SBC A,B		4 (4)			SZ H VNC
*						** * V1*
	opcode 98
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,d1				*  4
	move.b	regF,d0					*  4
	smi.b	d1					*  6/ 4 (T/F)
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	regBC,d0				*  4
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	sub.b	tblSBCrh(regZjmp,d1.w),regA		* 14 = 70/68
	
	FETCH						* 
	endopcode 98



*--- SBC A,C		4 (4)			SZ H VNC
*						** * V1*
	opcode 99
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regBC,-(regSwap)			*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	regBC,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14 = 66
	
	FETCH						* 
	endopcode 99



*--- SBC A,D		4 (4)			SZ H VNC
*						** * V1*
	opcode 9A
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d1				*  4
	move.b	regF,d0					*  4
	smi.b	d1					*  6/ 4 (T/F)
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	regDE,d0				*  4
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	sub.b	tblSBCrh(regZjmp,d1.w),regA		* 14 = 70/68
	
	FETCH						* 
	endopcode 9A



*--- SBC A,E		4 (4)			SZ H VNC
*						** * V1*
	opcode 9B
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regDE,-(regSwap)			*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	regDE,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14 = 66
	
	FETCH						* 
	endopcode 9B



*--- SBC A,H		4 (4)			SZ H VNC
*						** * V1*
	opcode 9C
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d1				*  4
	move.b	regF,d0					*  4
	smi.b	d1					*  6/ 4 (T/F)
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	regHL,d0				*  4
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	sub.b	tblSBCrh(regZjmp,d1.w),regA		* 14 = 70/68
	
	FETCH						* 
	endopcode 9C



*--- SBC A,L		4 (4)			SZ H VNC
*						** * V1*
	opcode 9D
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regHL,-(regSwap)			*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	regHL,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14 = 66
	
	FETCH						* 
	endopcode 9D



*--- SBC A,(HL)		7 (6)			SZ H VNC
*						** * V1*
	opcode 9E
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	movea.l	regHL,a0				*  4
	move.b	(a0),d1					*  8
	move.b	d1,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	d1,regA					*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* = 78
	endopcode 9E



*--- SBC A,A		4 (4)			SZ H VNC
*						** * V1*
	opcode 9F
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	regA,regA				*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14 = 66
	
	FETCH						* 
	endopcode 9F



*--- AND B		4 (4)			SZ H VNC
*						** 1 P00
	opcode A0
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	and.b	(regSwap)+,regA				*  8
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A0



*--- AND C		4 (4)			SZ H VNC
*						** 1 P00
	opcode A1
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	and.b	regBC,regA				*  4
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A1



*--- AND D		4 (4)			SZ H VNC
*						** 1 P00
	opcode A2
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	and.b	(regSwap)+,regA				*  8
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A2



*--- AND E		4 (4)			SZ H VNC
*						** 1 P00
	opcode A3
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	and.b	regDE,regA				*  4
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A3



*--- AND H		4 (4)			SZ H VNC
*						** 1 P00
	opcode A4
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	and.b	(regSwap)+,regA				*  8
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A4



*--- AND L		4 (4)			SZ H VNC
*						** 1 P00
	opcode A5
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	and.b	regHL,regA				*  4
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A5



*--- AND (HL)		7 (6)			SZ H VNC
*						** 1 P00
	opcode A6
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	and.b	(a0),regA				*  8
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A6



*--- AND A		4 (4)			SZ H VNC
*						** 1 P00
	opcode A7
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
*	and.b	regA,regA				*  4
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A7



*--- XOR B		4 (4)			SZ H VNC
*						** 0 P00
	opcode A8
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,d1				*  8
	eor.b	d1,regA					*  8
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A8



*--- XOR C		4 (4)			SZ H VNC
*						** 0 P00
	opcode A9
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	eor.b	regBC,regA				*  4
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode A9



*--- XOR D		4 (4)			SZ H VNC
*						** 0 P00
	opcode AA
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,d1				*  8
	eor.b	d1,regA					*  8
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode AA



*--- XOR E		4 (4)			SZ H VNC
*						** 0 P00
	opcode AB
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	eor.b	regDE,regA				*  4
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode AB



*--- XOR H		4 (4)			SZ H VNC
*						** 0 P00
	opcode AC
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,d1				*  8
	eor.b	d1,regA					*  8
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode AC



*--- XOR L		4 (4)			SZ H VNC
*						** 0 P00
	opcode AD
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	eor.b	regHL,regA				*  4
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode AD



*--- XOR (HL)		7 (6)			SZ H VNC
*						** 0 P00
	opcode AE
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	eor.b	d0,regA					*  4
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode AE



*--- XOR A		4 (4)			SZ H VNC
*						** 0 P00
	opcode AF
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	clr.b	regA					*  4
	
*	Fclr	S__H_VNCc				*  8
*	Fset	_Z______				*  8
	
	move.b	#set_Z___V__,regF			*  8
	
	FETCH						* 
	endopcode AF



*--- OR B		4 (4)			SZ H VNC
*						** 0 P00
	opcode B0
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	or.b	(regSwap)+,regA				*  8
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B0



*--- OR C		4 (4)			SZ H VNC
*						** 0 P00
	opcode B1
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	or.b	regBC,regA				*  4
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B1



*--- OR D		4 (4)			SZ H VNC
*						** 0 P00
	opcode B2
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	or.b	(regSwap)+,regA				*  8
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B2



*--- OR E		4 (4)			SZ H VNC
*						** 0 P00
	opcode B3
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	or.b	regDE,regA				*  4
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B3



*--- OR H		4 (4)			SZ H VNC
*						** 0 P00
	opcode B4
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	or.b	(regSwap)+,regA				*  8
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B4



*--- OR L		4 (4)			SZ H VNC
*						** 0 P00
	opcode B5
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	or.b	regHL,regA				*  4
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B5



*--- OR (HL)		7 (6)			SZ H VNC
*						** 0 P00
	opcode B6
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	or.b	(a0),regA				*  8
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B6



*--- OR A		4 (4)			SZ H VNC
*						** 0 P00
	opcode B7
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
*	or.b	regA,regA				*  4
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode B7



*--- CP B		4 (4)			SZ H VNC
*						** * V1*
	opcode B8
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,d1				*  4
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode B8



*--- CP C		4 (4)			SZ H VNC
*						** * V1*
	opcode B9
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regBC,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode B9



*--- CP D		4 (4)			SZ H VNC
*						** * V1*
	opcode BA
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,d1				*  4
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode BA



*--- CP E		4 (4)			SZ H VNC
*						** * V1*
	opcode BB
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regDE,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode BB



*--- CP H		4 (4)			SZ H VNC
*						** * V1*
	opcode BC
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,d1				*  4
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode BC



*--- CP L		4 (4)			SZ H VNC
*						** * V1*
	opcode BD
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regHL,-(regSwap)			*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode BD



*--- CP (HL)		7 (6)			SZ H VNC
*						** * V1*
	opcode BE
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode BE



*--- CP A		4 (4)			SZ H VNC
*						** * V1*
	opcode BF
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode BF



*--- RET NZ		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
	opcode C0
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- return
	movep.w	1(regSP),regAdr				*    16
	move.b	(regSP),regAdr				*     8
	addq.w	#2,regSP				*     8
	movea.l	regAdr,regPC				*     4
@@:
	FETCH						* 
	endopcode C0



*--- POP BC		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode C1
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regSP),regBC				* 16
	move.b	(regSP),regBC				*  8
	addq.w	#2,regSP				*  8
	
	FETCH						* 
	endopcode C1



*--- JP NZ,mn		10 (9)  / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode C2
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4 
	
	FETCH						*    
	
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode C2



*--- JP mn		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode C3
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						*  
	endopcode C3



*--- CALL NZ,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode C4
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*--call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						*    
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode C4



*--- PUSH BC		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode C5
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	regBC,-(regSP)				*  8
	
	FETCH						*  
	endopcode C5



*--- ADD A,n		7 (6)			SZ H VNC
*						** * V0*
	opcode C6
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,-(regSwap)			* 12
	move.w	(regSwap),d1				*  8
	move.b	regA,d1					*  4
	
	add.b	(regSwap)+,regA				*  8
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode C6



*--- RST 00H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode C7
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	clr.w	regAdr					*  4
	movea.l	regAdr,regPC				*  4
	
	FETCH						*  
	endopcode C7



*--- RET Z		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
	opcode C8
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- return
	movep.w	1(regSP),regAdr				*    16
	move.b	(regSP),regAdr				*     8
	addq.w	#2,regSP				*     8
	movea.l	regAdr,regPC				*     4
@@:
	FETCH						* 
	endopcode C8



*--- RET		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode C9
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regSP),regAdr				* 16
	move.b	(regSP),regAdr				*  8
	addq.w	#2,regSP				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode C9



*--- JP Z,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode CA
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode CA



*--- CB xx
	opcode CB
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,(regOP2fetch)			* 12
	move.w	(regOP2fetch),d0			*  8	(下位バイトは 0)
	jmp	OP_CB(regZjmp,d0.w)			* 14
	endopcode CB



*--- CALL Z,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode CC
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_Z,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode CC



*--- CALL mn		17 (16)			SZ H VNC
*						･･ ･ ･･･
	opcode CD
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	
	move.w	regPC,d0				*  4
	addq.w	#2,d0					*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	movea.l	regAdr,regPC				*  4
	
	FETCH						*  
	endopcode CD



*--- ADC A,n		7 (6)			SZ H VNC
*						** * V0*
	opcode CE
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(regPC)+,d0				*  8
	
	add.b	regF,regF				*  4
	addx.b	d0,regA					*  4
	
	lea	FtblADD8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* = 70
	endopcode CE



*--- RST 08H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode CF
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0008,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						*  
	endopcode CF



*--- RET NC		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
	opcode D0
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bmi.s	@f					* 10/ 8 (T/F)
.endif
		movep.w	1(regSP),regAdr			*    16
		move.b	(regSP),regAdr			*     8
		addq.w	#2,regSP			*     8
		movea.l	regAdr,regPC			*     4
@@:
	FETCH						* 
	endopcode D0



*--- POP DE		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode D1
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regSP),regDE				* 16
	move.b	(regSP),regDE				*  8
	addq.w	#2,regSP				*  8
	
	FETCH						* 
	endopcode D1



*--- JP NC,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode D2
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bmi.s	@f					* 10/ 8 (T/F)
.endif
		movep.w	1(regPC),regAdr			*    16
		move.b	(regPC),regAdr			*     8
		movea.l	regAdr,regPC			*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode D2



*--- OUT n,A		11 (10)			SZ H VNC
*						･･ ･ ･･･
	opcode D3
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,d0					*  4
	
	move.b	(regPC)+,(regZeroSwap)			* 12
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode D3



*--- CALL NC,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode D4
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bmi.s	@f					* 10/ 8 (T/F)
.endif
	
	*--call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode D4



*--- PUSH DE		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode D5
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	regDE,-(regSP)				*  8
	
	FETCH						* 
	endopcode D5



*--- SUB n		7 (6)			SZ H VNC
*						** * V1*
	opcode D6
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,-(regSwap)			* 12
	move.w	(regSwap),d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	(regSwap)+,regA				*  8
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode D6



*--- RST 10H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode D7
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0010,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode D7



*--- RET C		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
	opcode D8
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bpl.s	@f					* 10/ 8 (T/F)
.endif
		movep.w	1(regSP),regAdr			*    16
		move.b	(regSP),regAdr			*     8
		addq.w	#2,regSP			*     8
		movea.l	regAdr,regPC			*     4
@@:
	FETCH						* 
	endopcode D8



*--- EXX		4 (3) ?			SZ H VNC
*						･･ ･ ･･･
	opcode D9
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movem.w	regSave(regFtbl),d0/a0/a1		* 16+4*3 = 28
	
	movem.w	regBC/regDE/regHL,regSave(regFtbl)	* 12+4*3 = 24
	
	move.w	d0,regBC				*  4
	move.w	a0,regDE				*  4
	move.w	a1,regHL				*  4
	
	FETCH						* = 64
	endopcode D9



*--- JP C,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode DA
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bpl.s	@f					* 10/ 8 (T/F)
.endif
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	FETCH						* 
	endopcode DA



*--- IN A,n		11 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode DB
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,(regZeroSwap)			* 12
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.b	d1,regA					*  4
	
	FETCH						* 
	endopcode DB



*--- CALL C,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode DC
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
.if 0
	moveq.l	#flag_C,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
.else
	tst.b	regF					*  4
	bpl.s	@f					* 10/ 8 (T/F)
.endif
	
	*-- call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode DC



*--- DD __
	opcode DD
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	workIX(regFtbl),regIXY			*  8
	
	move.b	(regPC)+,(regOP2fetch)			* 12
	move.w	(regOP2fetch),d0			*  8	(下位バイトは 0)
	jmp	OP_DD(regZjmp,d0.w)			* 14
	endopcode DD



*--- SBC A,n		7 (6)			SZ H VNC
*						** * V1*
	opcode DE
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	(regPC)+,d1				*  8
	move.b	d1,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	d1,regA					*  4
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d0.l),regF				* 14
	
	FETCH						* 
	endopcode DE



*--- RST 18H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode DF
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0018,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode DF



*--- RET PO		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
*	if (P == 0) ret
	opcode E0
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_P,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- return
	movep.w	1(regSP),regAdr				*    16
	move.b	(regSP),regAdr				*     8
	addq.w	#2,regSP				*     8
	movea.l	regAdr,regPC				*     4
@@:
	FETCH						*  
	endopcode E0



*--- POP HL		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode E1
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regSP),regHL				* 16
	move.b	(regSP),regHL				*  8
	addq.w	#2,regSP				*  8
	
	FETCH						* 
	endopcode E1



*--- JP PO,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
*	if (P == 0) jump
	opcode E2
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_P,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode E2



*--- EX (SP),HL		19 (16)			SZ H VNC
*						･･ ･ ･･･
	opcode E3
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movep.w	1(regSP),d0				* 16
	move.b	(regSP),d0				*  8
	
	move.b	regHL,(regSP)				*  8
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,1(regSP)			* 12
	
	move.w	d0,regHL				*  4
	
	FETCH						* 
	endopcode E3



*--- CALL PO,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
*	if (P == 0) call
	opcode E4
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_P,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*--call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode E4



*--- PUSH HL		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode E5
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	regHL,-(regSP)				*  8
	
	FETCH						* 
	endopcode E5



*--- AND n		7 (6)			SZ H VNC
*						** 1 P00
	opcode E6
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	and.b	(regPC)+,regA				*  8
	
	lea	FtblANDr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode E6



*--- RST 20H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode E7
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0020,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode E7



*--- RET PE		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
*	if (P == 1) ret
	opcode E8
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_P,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- return
	movep.w	1(regSP),regAdr				*    16
	move.b	(regSP),regAdr				*     8
	addq.w	#2,regSP				*     8
	movea.l	regAdr,regPC				*     4
@@:
	FETCH						* 
	endopcode E8



*--- JP (HL)		4 (3)			SZ H VNC
*						･･ ･ ･･･
	opcode E9
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,regPC				*  4
	
	FETCH						* 
	endopcode E9



*--- JP PE,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
*	if (P == 1) jump
	opcode EA
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_P,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode EA



*--- EX DE,HL		4 (4) 			SZ H VNC
*						･･ ･ ･･･
	opcode EB
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	exg.l	regHL,regDE				*  6
	
	FETCH						* 
	endopcode EB



*--- CALL PE,mn		17 (16) / 6 (10)	SZ H VNC
*						･･ ･ ･･･
*	if (P == 1) call
	opcode EC
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_P,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode EC



*--- ED __
	opcode ED
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,(regOP2fetch)			* 12
	move.w	(regOP2fetch),d0			*  8	(下位バイトは 0)
	jmp	OP_ED(regZjmp,d0.w)			* 14
	endopcode ED



*--- XOR n		7 (6)			SZ H VNC
*						** 0 P00
	opcode EE
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,d0				*  8
	eor.b	d0,regA					*  4
	
	lea	FtblXORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode EE



*--- RST 28H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode EF
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0028,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode EF



*--- RET P		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
	opcode F0
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_S,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- return
	movep.w	1(regSP),regAdr				*    16
	move.b	(regSP),regAdr				*     8
	addq.w	#2,regSP				*     8
	movea.l	regAdr,regPC				*     4
@@:
	FETCH						* 
	endopcode F0



*--- POP AF		10 (9)			SZ H VNC
*						･･ ･ ･･･
	opcode F1
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regSP)+,regF				*  8
	ror.b	#1,regF					*  8
	
	move.b	(regSP)+,regA				*  8
	
	FETCH						* 
	endopcode F1



*--- JP P,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode F2
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_S,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode F2



*--- DI			4 (3)			SZ H VNC
*						･･ ･ ･･･
	opcode F3
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	clr.b	Z80_DIEI(regFtbl)			* 16
	
	FETCH						* 
	endopcode F3



*--- CALL P,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode F4
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_S,d0				*  4
	and.b	regF,d0					*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
	
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode F4



*--- PUSH AF		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode F5
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	regA,-(regSP)				*  8
	
	move.b	regF,d0					*  4
	rol.b	#1,d0					*  8
	move.b	d0,-(regSP)				*  8
	
	FETCH						* 
	endopcode F5



*--- OR n		7 (6)			SZ H VNC
*						** 0 P00
	opcode F6
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	or.b	(regPC)+,regA				*  8
	
	lea	FtblORr(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	FETCH						* 
	endopcode F6



*--- RST 30H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode F7
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0030,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode F7



*--- RET M		11 (10) / 5 (5)		SZ H VNC
*						･･ ･ ･･･
	opcode F8
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_S,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- return
	movep.w	1(regSP),regAdr				*    16
	move.b	(regSP),regAdr				*     8
	addq.w	#2,regSP				*     8
	movea.l	regAdr,regPC				*     4
@@:
	FETCH						* 
	endopcode F8



*--- LD SP,HL		6 (4)			SZ H VNC
*						･･ ･ ･･･
	opcode F9
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	movea.l	regHL,regSP				*  4
	
	FETCH						* 
	endopcode F9



*--- JP M,mn		10 (9) / 10 (6)		SZ H VNC
*						･･ ･ ･･･
	opcode FA
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_S,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- jump
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
	
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode FA



*--- EI			4 (3)			SZ H VNC
*						･･ ･ ･･･
	opcode FB
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	*-- EI直後の命令は、割り込み fetch は行われない
	; (EI状態だった場合は fetch する)
	move.b	(regPC)+,(regOP1fetch)			* 12
	move.w	(regOP1fetch),d1			*  8
	
	tas	Z80intrSuspend(regFtbl)			* 18
	bmi.s	@f					* 10/ 8 (T/F)
	
	*-- 割り込み発生
	move.b	Z80INTREQ(regFtbl),Z80intrFlag(regFtbl)	* 20
	
	*-- Z80 デバッガ用の割り込みフラグ
	move.b	#0,interrupt_inDebugger(regFtbl)	* 16
@@:
	
	*-- 割り込み許可
	move.b	#ENABLE_INT,Z80_DIEI(regFtbl)		* 16
	
	jmp	OP_Z(regZjmp,d1.w)			* 14
	endopcode FB



*--- CALL M,mn		17 (16) / 10 (6)	SZ H VNC
*						･･ ･ ･･･
	opcode FC
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	moveq.l	#flag_S,d0				*  4
	and.b	regF,d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- call
	movep.w	1(regPC),regAdr				*    16
	move.b	(regPC),regAdr				*     8
	
	move.w	regPC,d0				*     4
	addq.w	#2,d0					*     4
	move.w	d0,-(regSwap)				*     8
	move.b	(regSwap)+,-(regSP)			*    12
	move.b	d0,-(regSP)				*     8
	
	movea.l	regAdr,regPC				*     4
	
	FETCH						* 
	
@@:
	addq.w	#2,regPC				*  8
	
	FETCH						* 
	endopcode FC



*--- FD __
	opcode FD
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	lea	workIY(regFtbl),regIXY			*  8
	
	move.b	(regPC)+,(regOP2fetch)			* 12
	move.w	(regOP2fetch),d0			*  8	(下位バイトは 0)
	jmp	OP_FD(regZjmp,d0.w)			* 14
	endopcode FD



*--- CP n		7 (6)			SZ H VNC
*						** * V1*
	opcode FE
	ENTER_DEBUGGER					* 
	
	FETCH_INTERRUPT					* 
	
	move.b	(regPC)+,-(regSwap)			* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	FETCH						* 
	endopcode FE



*--- RST 38H		11 (11)			SZ H VNC
*						･･ ･ ･･･
	opcode FF
	ENTER_DEBUGGER					* 
	
_Z80intrRoutine:
	subq.w	#1,regPC				*  8 (2)
	addq.w	#4,(regOP1fetch)			* 12 (2)
	
	
	move.w	regPC,d0				*  4
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,-(regSP)			* 12
	move.b	d0,-(regSP)				*  8
	
	move.w	#$0038,regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode FF




*----------------------------------------------------------------------------------------------
* CB xx



*--- RLC B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB00
	move.w	regBC,d0				*  4
	smi.b	regBC					*  6/ 4 (T/F)
	
	move.b	FtblRLCrh(regZjmp,regBC.w),regF		* 14
	
	add.w	regBC,regBC				*  4
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB00



*--- RLC C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB01
	moveq.l	#0,d0					*  4
	move.b	regBC,d0				*  4
	lea	FtblRLCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.b	#1,regBC				*  8
	
	FETCH						* 
	endopcode CB01



*--- RLC D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB02
	move.w	regDE,d0				*  4
	smi.b	regDE					*  6/ 4 (T/F)
	
	move.b	FtblRLCrh(regZjmp,regDE.w),regF		* 14
	
	add.w	regDE,regDE				*  4
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB02



*--- RLC E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB03
	moveq.l	#0,d0					*  4
	move.b	regDE,d0				*  4
	lea	FtblRLCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.b	#1,regDE				*  8
	
	FETCH						* 
	endopcode CB03



*--- RLC H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB04
	move.w	regHL,d0				*  4
	smi.b	regHL					*  6/ 4 (T/F)
	
	move.b	FtblRLCrh(regZjmp,regHL.w),regF		* 14
	
	add.w	regHL,regHL				*  4
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB04



*--- RLC L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB05
	moveq.l	#0,d0					*  4
	move.b	regHL,d0				*  4
	lea	FtblRLCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.b	#1,regHL				*  8
	
	FETCH						* 
	endopcode CB05



*--- RLC (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB06
	movea.l	regHL,a0				*  4
	
	moveq.l	#0,d0					*  4
	move.b	(a0),d0					*  8
	
	lea	FtblRLCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.b	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	FETCH						* 
	endopcode CB06



*--- RLC A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB07
	lea	FtblRLCrl(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	rol.b	#1,regA					*  8
	
	FETCH						* 
	endopcode CB07



*--- RRC B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB08
	move.w	regBC,d0				*  4
	btst.l	#8,regBC				* 10
	sne.b	regBC					*  6/ 4 (T/F)
	
	move.b	FtblRRCrh(regZjmp,regBC.w),regF		* 14
	
	ror.w	#1,regBC				*  8
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB08



*--- RRC C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB09
	moveq.l	#0,d0					*  4
	move.b	regBC,d0				*  4
	lea	FtblRRCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.b	#1,regBC				*  8
	
	FETCH						* 
	endopcode CB09



*--- RRC D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB0A
	move.w	regDE,d0				*  4
	btst.l	#8,regDE				* 10
	sne.b	regDE					*  6/ 4 (T/F)
	
	move.b	FtblRRCrh(regZjmp,regDE.w),regF		* 14
	
	ror.w	#1,regDE				*  8
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB0A



*--- RRC E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB0B
	moveq.l	#0,d0					*  4
	move.b	regDE,d0				*  4
	lea	FtblRRCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.b	#1,regDE				*  8
	
	FETCH						* 
	endopcode CB0B



*--- RRC H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB0C
	move.w	regHL,d0				*  4
	btst.l	#8,regHL				* 10
	sne.b	regHL					*  6/ 4 (T/F)
	
	move.b	FtblRRCrh(regZjmp,regHL.w),regF		* 14
	
	ror.w	#1,regHL				*  8
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB0C



*--- RRC L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB0D
	moveq.l	#0,d0					*  4
	move.b	regHL,d0				*  4
	lea	FtblRRCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.b	#1,regHL				*  8
	
	FETCH						* 
	endopcode CB0D



*--- RRC (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB0E
	moveq.l	#0,d0					*  4
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	lea	FtblRRCrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.b	#1,d0					*  4
	move.b	d0,(a0)					*  8
	
	FETCH						* 
	endopcode CB0E



*--- RRC A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB0F
	lea	FtblRRCrl(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	ror.b	#1,regA					*  8
	
	FETCH						* 
	endopcode CB0F



*--- RL B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB10
	move.b	regBC,d0				*  4
	
	tst.b	regF					*  4
	smi.b	regBC					*  6/ 4
	
	move.b	FtblRLrh(regZjmp,regBC.w),regF		* 14
	
	add.w	regBC,regBC				*  4
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB10



*--- RL C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB11
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regBC,d0				*  4
	
	lea	FtblRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB11



*--- RL D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB12
	move.b	regDE,d0				*  4
	tst.b	regF					*  4
	smi.b	regDE					*  6/ 4
	
	move.b	FtblRLrh(regZjmp,regDE.w),regF		* 14
	
	add.w	regDE,regDE				*  4
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB12



*--- RL E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB13
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regDE,d0				*  4
	
	lea	FtblRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB13



*--- RL H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB14
	move.b	regHL,d0				*  4
	tst.b	regF					*  4
	smi.b	regHL					*  6/ 4
	
	move.b	FtblRLrh(regZjmp,regHL.w),regF		* 14
	
	add.w	regHL,regHL				*  4
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB14



*--- RL L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB15
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regHL,d0				*  4
	
	lea	FtblRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,regHL				*  4 = 50
	
	FETCH						* 
	endopcode CB15



*--- RL (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB16
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	lea	FtblRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	FETCH						*
	endopcode CB16



*--- RL A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB17
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regA,d0					*  4
	
	lea	FtblRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,regA					*  4
	
	FETCH						* 
	endopcode CB17



*--- RR B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB18
	move.b	regBC,d0				*  4
	tst.b	regF					*  4
	smi.b	regBC					*  6/ 4
	
	move.b	FtblRRrh(regZjmp,regBC.w),regF		* 14
	
	ror.w	#1,regBC				*  8
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB18



*--- RR C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB19
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regBC,d0				*  4
	
	lea	FtblRRrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB19



*--- RR D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB1A
	move.b	regDE,d0				*  4
	tst.b	regF					*  4
	smi.b	regDE					*  6/ 4
	
	move.b	FtblRRrh(regZjmp,regDE.w),regF		* 14
	
	ror.w	#1,regDE				*  8
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB1A



*--- RR E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB1B
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regDE,d0				*  4
	
	lea	FtblRRrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB1B



*--- RR H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB1C
	move.b	regHL,d0				*  4
	tst.b	regF					*  4
	smi.b	regHL					*  6/ 4
	
	move.b	FtblRRrh(regZjmp,regHL.w),regF		* 14
	
	ror.w	#1,regHL				*  8
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB1C



*--- RR L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB1D
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regHL,d0				*  4
	
	lea	FtblRRrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB1D



*--- RR (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB1E
	movea.l	regHL,a0				*  4
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	(a0),d0					*  8
	
	lea	FtblRRrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	FETCH						* 
	endopcode CB1E



*--- RR A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB1F
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	regA,d0					*  4
	
	lea	FtblRRrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,regA					*  4
	
	FETCH						* 
	endopcode CB1F



*--- SLA B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB20
	move.b	regBC,d0				*  4
	clr.b	regBC					*  4
	
	move.b	FtblSLArh(regZjmp,regBC.w),regF		* 14
	
	add.w	regBC,regBC				*  4
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB20



*--- SLA C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB21
	moveq.l	#0,d0					*  4
	move.b	regBC,d0				*  4
	lea	FtblSLArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	add.b	regBC,regBC				*  4
	
	FETCH						* 
	endopcode CB21



*--- SLA D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB22
	move.b	regDE,d0				*  4
	clr.b	regDE					*  4
	
	move.b	FtblSLArh(regZjmp,regDE.w),regF		* 14
	
	add.w	regDE,regDE				*  4
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB22



*--- SLA E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB23
	moveq.l	#0,d0					*  4
	move.b	regDE,d0				*  4
	lea	FtblSLArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	add.b	regDE,regDE				*  4
	
	FETCH						* 
	endopcode CB23



*--- SLA H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB24
	move.b	regHL,d0				*  4
	clr.b	regHL					*  4
	
	move.b	FtblSLArh(regZjmp,regHL.w),regF		* 14
	
	add.w	regHL,regHL				*  4
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB24



*--- SLA L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB25
	moveq.l	#0,d0					*  4
	move.b	regHL,d0				*  4
	lea	FtblSLArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	add.b	regHL,regHL				*  4
	
	FETCH						* 
	endopcode CB25



*--- SLA (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB26
	moveq.l	#0,d0					*  4
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	lea	FtblSLArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	add.b	d0,(a0)					* 12
	
	FETCH						* 
	endopcode CB26



*--- SLA A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB27
	lea	FtblSLArl(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	add.b	regA,regA				*  4
	
	FETCH						* 
	endopcode CB27



*--- SRA B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB28
	move.b	regBC,d0				*  4
	clr.b	regBC					*  4
	
	move.b	FtblSRArh(regZjmp,regBC.w),regF		* 14
	
	asr.w	#1,regBC				*  8
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB28



*--- SRA C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB29
	moveq.l	#0,d0					*  4
	move.b	regBC,d0				*  4
	lea	FtblSRArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	asr.b	#1,regBC				*  8
	
	FETCH						* 
	endopcode CB29



*--- SRA D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB2A
	move.b	regDE,d0				*  4
	clr.b	regDE					*  4
	
	move.b	FtblSRArh(regZjmp,regDE.w),regF		* 14
	
	asr.w	#1,regDE				*  8
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB2A



*--- SRA E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB2B
	moveq.l	#0,d0					*  4
	move.b	regDE,d0				*  4
	lea	FtblSRArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	asr.b	#1,regDE				*  8
	
	FETCH						* 
	endopcode CB2B



*--- SRA H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB2C
	move.b	regHL,d0				*  4
	clr.b	regHL					*  4
	
	move.b	FtblSRArh(regZjmp,regHL.w),regF		* 14
	
	asr.w	#1,regHL				*  8
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB2C



*--- SRA L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB2D
	moveq.l	#0,d0					*  4
	move.b	regHL,d0				*  4
	lea	FtblSRArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	asr.b	#1,regHL				*  8
	
	FETCH						* 
	endopcode CB2D



*--- SRA (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB2E
	moveq.l	#0,d0					*  4
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	lea	FtblSRArl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	asr.b	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	FETCH						* 
	endopcode CB2E



*--- SRA A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB2F
	lea	FtblSRArl(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	asr.b	#1,regA					*  8
	
	FETCH						* 
	endopcode CB2F



*--- SLI B	**
	opcode CB30
	IllegalOP	2
	endopcode CB30



*--- SLI C	**
	opcode CB31
	IllegalOP	2
	endopcode CB31



*--- SLI D	**
	opcode CB32
	IllegalOP	2
	endopcode CB32



*--- SLI E	**
	opcode CB33
	IllegalOP	2
	endopcode CB33



*--- SLI H	**
	opcode CB34
	IllegalOP	2
	endopcode CB34



*--- SLI L	**
	opcode CB35
	IllegalOP	2
	endopcode CB35



*--- SLI (HL)	**
	opcode CB36
	IllegalOP	2
	endopcode CB36



*--- SLI A	**
	opcode CB37
	IllegalOP	2
	endopcode CB37



*--- SRL B		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB38
	move.b	regBC,d0				*  4
	clr.b	regBC					*  4
	
	move.b	FtblSRLrh(regZjmp,regBC.w),regF		* 14
	
	lsr.w	#1,regBC				*  8
	move.b	d0,regBC				*  4
	
	FETCH						* 
	endopcode CB38



*--- SRL C		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB39
	moveq.l	#0,d0					*  4
	move.b	regBC,d0				*  4
	lea	FtblSRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	lsr.b	#1,regBC				*  8
	
	FETCH						* 
	endopcode CB39



*--- SRL D		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB3A
	move.b	regDE,d0				*  4
	clr.b	regDE					*  4
	
	move.b	FtblSRLrh(regZjmp,regDE.w),regF		* 14
	
	lsr.w	#1,regDE				*  8
	move.b	d0,regDE				*  4
	
	FETCH						* 
	endopcode CB3A



*--- SRL E		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB3B
	moveq.l	#0,d0					*  4
	move.b	regDE,d0				*  4
	lea	FtblSRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	lsr.b	#1,regDE				*  8
	
	FETCH						* 
	endopcode CB3B



*--- SRL H		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB3C
	move.b	regHL,d0				*  4
	clr.b	regHL					*  4
	
	move.b	FtblSRLrh(regZjmp,regHL.w),regF		* 14
	
	lsr.w	#1,regHL				*  8
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode CB3C



*--- SRL L		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB3D
	moveq.l	#0,d0					*  4
	move.b	regHL,d0				*  4
	lea	FtblSRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	lsr.b	#1,regHL				*  8
	
	FETCH						* 
	endopcode CB3D



*--- SRL (HL)		15 (13)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode CB3E
	moveq.l	#0,d0					*  4
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	lea	FtblSRLrl(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	lsr.b	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	FETCH						* 
	endopcode CB3E



*--- SRL A		8 (7)			SZ H VNC
*						** 0 P0*
	opcode CB3F
	lea	FtblSRLrl(regFtbl),a1			*  8
	move.b	(a1,regA.w),regF			* 14
	
	lsr.b	#1,regA					*  8
	
	FETCH						* 
	endopcode CB3F



*--- BIT 0,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB40
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit0H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB40



*--- BIT 0,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB41
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit0L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB41



*--- BIT 0,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB42
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit0H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB42



*--- BIT 0,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB43
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	moveq.l	#BITbit0L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB43



*--- BIT 0,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB44
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	btst.l	#BITbit0H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB44



*--- BIT 0,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB45
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit0L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB45



*--- BIT 0,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB46
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit0L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB46



*--- BIT 0,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB47
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit0L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB47



*--- BIT 1,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB48
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit1H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB48



*--- BIT 1,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB49
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit1L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB49



*--- BIT 1,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB4A
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	btst.l	#BITbit1H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB4A



*--- BIT 1,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB4B
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit1L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB4B



*--- BIT 1,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB4C
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit1H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB4C



*--- BIT 1,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB4D
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit1L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB4D



*--- BIT 1,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB4E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit1L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB4E



*--- BIT 1,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB4F
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit1L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*  8
@@:
	FETCH						* 
	endopcode CB4F



*--- BIT 2,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB50
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit2H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB50



*--- BIT 2,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB51
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit2L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB51



*--- BIT 2,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB52
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit2H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB52



*--- BIT 2,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB53
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit2L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB53



*--- BIT 2,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB54
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit2H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB54



*--- BIT 2,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB55
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit2L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB55



*--- BIT 2,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB56
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit2L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB56



*--- BIT 2,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB57
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit2L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB57



*--- BIT 3,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB58
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit3H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB58



*--- BIT 3,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB59
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit3L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB59



*--- BIT 3,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB5A
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit3H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB5A



*--- BIT 3,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB5B
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit3L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB5B



*--- BIT 3,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB5C
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit3H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB5C



*--- BIT 3,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB5D
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit3L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB5D



*--- BIT 3,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB5E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit3L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB5E



*--- BIT 3,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB5F
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit3L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB5F



*--- BIT 4,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB60
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit4H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB60



*--- BIT 4,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB61
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit4L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB61



*--- BIT 4,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB62
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit4H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB62



*--- BIT 4,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB63
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit4L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB63



*--- BIT 4,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB64
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit4H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB64



*--- BIT 4,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB65
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit4L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB65



*--- BIT 4,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB66
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit4L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB66



*--- BIT 4,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB67
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit4L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB67



*--- BIT 5,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB68
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit5H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB68



*--- BIT 5,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB69
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit5L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB69



*--- BIT 5,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB6A
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit5H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB6A



*--- BIT 5,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB6B
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit5L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB6B



*--- BIT 5,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB6C
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit5H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB6C



*--- BIT 5,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB6D
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit5L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB6D



*--- BIT 5,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB6E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit5L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB6E



*--- BIT 5,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB6F
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit5L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB6F



*--- BIT 6,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB70
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit6H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB70



*--- BIT 6,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB71
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit6L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB71



*--- BIT 6,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB72
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit6H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB72



*--- BIT 6,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB73
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit6L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB73



*--- BIT 6,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB74
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit6H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB74



*--- BIT 6,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB75
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit6L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB75



*--- BIT 6,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB76
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit6L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB76



*--- BIT 6,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB77
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit6L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB77



*--- BIT 7,B		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB78
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit7H,regBC				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB78



*--- BIT 7,C		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB79
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit7L,d0				*  4
	and.b	regBC,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB79



*--- BIT 7,D		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB7A
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit7H,regDE				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB7A



*--- BIT 7,E		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB7B
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit7L,d0				*  4
	and.b	regDE,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB7B



*--- BIT 7,H		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB7C
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.l	#BITbit7H,regHL				* 10
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB7C



*--- BIT 7,L		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB7D
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit7L,d0				*  4
	and.b	regHL,d0				*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB7D



*--- BIT 7,(HL)		12 (9)			SZ H VNC
*						x* 1 x0･
	opcode CB7E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit7L,d0				*  4
	movea.l	regHL,a0				*  4
	and.b	(a0),d0					*  8
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB7E



*--- BIT 7,A		8 (6)			SZ H VNC
*						x* 1 x0･
	opcode CB7F
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	moveq.l	#BITbit7L,d0				*  4
	and.b	regA,d0					*  4
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode CB7F



*--- RES 0,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB80
	andi.w	#RESbit0H,regBC				*  8
	
	FETCH						* 
	endopcode CB80



*--- RES 0,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB81
	andi.b	#RESbit0L,regBC				*  8
	
	FETCH						* 
	endopcode CB81



*--- RES 0,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB82
	andi.w	#RESbit0H,regDE				*  8
	
	FETCH						* 
	endopcode CB82



*--- RES 0,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB83
	andi.b	#RESbit0L,regDE				*  8
	
	FETCH						* 
	endopcode CB83



*--- RES 0,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB84
	andi.w	#RESbit0H,regHL				*  8
	
	FETCH						* 
	endopcode CB84



*--- RES 0,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB85
	andi.b	#RESbit0L,regHL				*  8
	
	FETCH						* 
	endopcode CB85



*--- RES 0,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CB86
	movea.l	regHL,a0				*  4
	andi.b	#RESbit0L,(a0)				* 16
	
	FETCH						* 
	endopcode CB86



*--- RES 0,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB87
	andi.b	#RESbit0L,regA				*  8
	
	FETCH						* 
	endopcode CB87



*--- RES 1,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB88
	andi.w	#RESbit1H,regBC				*  8
	
	FETCH						* 
	endopcode CB88



*--- RES 1,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB89
	andi.b	#RESbit1L,regBC				*  8
	
	FETCH						* 
	endopcode CB89



*--- RES 1,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB8A
	andi.w	#RESbit1H,regDE				*  8
	
	FETCH						* 
	endopcode CB8A



*--- RES 1,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB8B
	andi.b	#RESbit1L,regDE				*  8
	
	FETCH						* 
	endopcode CB8B



*--- RES 1,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB8C
	andi.w	#RESbit1H,regHL				*  8
	
	FETCH						* 
	endopcode CB8C



*--- RES 1,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB8D
	andi.b	#RESbit1L,regHL				*  8
	
	FETCH						* 
	endopcode CB8D



*--- RES 1,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CB8E
	movea.l	regHL,a0				*  4
	andi.b	#RESbit1L,(a0)				* 16
	
	FETCH						* 
	endopcode CB8E



*--- RES 1,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB8F
	andi.b	#RESbit1L,regA				*  8
	
	FETCH						* 
	endopcode CB8F



*--- RES 2,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB90
	andi.w	#RESbit2H,regBC				*  8
	
	FETCH						* 
	endopcode CB90



*--- RES 2,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB91
	andi.b	#RESbit2L,regBC				*  8
	
	FETCH						* 
	endopcode CB91



*--- RES 2.D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB92
	andi.w	#RESbit2H,regDE				*  8
	
	FETCH						* 
	endopcode CB92



*--- RES 2,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB93
	andi.b	#RESbit2L,regDE				*  8
	
	FETCH						* 
	endopcode CB93



*--- RES 2,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB94
	andi.w	#RESbit2H,regHL				*  8
	
	FETCH						* 
	endopcode CB94



*--- RES 2,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB95
	andi.b	#RESbit2L,regHL				*  8
	
	FETCH						* 
	endopcode CB95



*--- RES 2,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CB96
	movea.l	regHL,a0				*  4
	andi.b	#RESbit2L,(a0)				* 16
	
	FETCH						* 
	endopcode CB96



*--- RES 2,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB97
	andi.b	#RESbit2L,regA				*  8
	
	FETCH						* 
	endopcode CB97



*--- RES 3,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB98
	andi.w	#RESbit3H,regBC				*  8
	
	FETCH						* 
	endopcode CB98



*--- RES 3,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB99
	andi.b	#RESbit3L,regBC				*  8
	
	FETCH						* 
	endopcode CB99



*--- RES 3,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB9A
	andi.w	#RESbit3H,regDE				*  8
	
	FETCH						* 
	endopcode CB9A



*--- RES 3,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB9B
	andi.b	#RESbit3L,regDE				*  8
	
	FETCH						* 
	endopcode CB9B



*--- RES 3,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB9C
	andi.w	#RESbit3H,regHL				*  8
	
	FETCH						* 
	endopcode CB9C



*--- RES 3,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB9D
	andi.b	#RESbit3L,regHL				*  8
	
	FETCH						* 
	endopcode CB9D



*--- RES 3,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CB9E
	movea.l	regHL,a0				*  4
	andi.b	#RESbit3L,(a0)				* 16
	
	FETCH						* 
	endopcode CB9E



*--- RES 3,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CB9F
	andi.b	#RESbit3L,regA				*  8
	
	FETCH						* 
	endopcode CB9F



*--- RES 4,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA0
	andi.w	#RESbit4H,regBC				*  8
	
	FETCH						* 
	endopcode CBA0



*--- RES 4,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA1
	andi.b	#RESbit4L,regBC				*  8
	
	FETCH						* 
	endopcode CBA1



*--- RES 4,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA2
	andi.w	#RESbit4H,regDE				*  8
	
	FETCH						* 
	endopcode CBA2



*--- RES 4,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA3
	andi.b	#RESbit4L,regDE				*  8
	
	FETCH						* 
	endopcode CBA3



*--- RES 4,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA4
	andi.w	#RESbit4H,regHL				*  8
	
	FETCH						* 
	endopcode CBA4



*--- RES 4,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA5
	andi.b	#RESbit4L,regHL				*  8
	
	FETCH						* 
	endopcode CBA5



*--- RES 4,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBA6
	movea.l	regHL,a0				*  4
	andi.b	#RESbit4L,(a0)				* 16
	
	FETCH						* 
	endopcode CBA6



*--- RES 4,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA7
	andi.b	#RESbit4L,regA				*  8
	
	FETCH						* 
	endopcode CBA7



*--- RES 5,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA8
	andi.w	#RESbit5H,regBC				*  8
	
	FETCH						* 
	endopcode CBA8



*--- RES 5,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBA9
	andi.b	#RESbit5L,regBC				*  8
	
	FETCH						* 
	endopcode CBA9



*--- RES 5,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBAA
	andi.w	#RESbit5H,regDE				*  8
	
	FETCH						* 
	endopcode CBAA



*--- RES 5,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBAB
	andi.b	#RESbit5L,regDE				*  8
	
	FETCH						* 
	endopcode CBAB



*--- RES 5,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBAC
	andi.w	#RESbit5H,regHL				*  8
	
	FETCH						* 
	endopcode CBAC



*--- RES 5,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBAD
	andi.b	#RESbit5L,regHL				*  8
	
	FETCH						* 
	endopcode CBAD



*--- RES 5,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBAE
	movea.l	regHL,a0				*  4
	andi.b	#RESbit5L,(a0)				* 16
	
	FETCH						* 
	endopcode CBAE



*--- RES 5,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBAF
	andi.b	#RESbit5L,regA				*  8
	
	FETCH						* 
	endopcode CBAF



*--- RES 6,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB0
	andi.w	#RESbit6H,regBC				*  8
	
	FETCH						* 
	endopcode CBB0



*--- RES 6,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB1
	andi.b	#RESbit6L,regBC				*  8
	
	FETCH						* 
	endopcode CBB1



*--- RES 6,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB2
	andi.w	#RESbit6H,regDE				*  8
	
	FETCH						* 
	endopcode CBB2



*--- RES 6,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB3
	andi.b	#RESbit6L,regDE				*  8
	
	FETCH						* 
	endopcode CBB3



*--- RES 6,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB4
	andi.w	#RESbit6H,regHL				*  8
	
	FETCH						* 
	endopcode CBB4



*--- RES 6,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB5
	andi.b	#RESbit6L,regHL				*  8
	
	FETCH						* 
	endopcode CBB5



*--- RES 6,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBB6
	movea.l	regHL,a0				*  4
	andi.b	#RESbit6L,(a0)				* 16
	
	FETCH						* 
	endopcode CBB6



*--- RES 6,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB7
	andi.b	#RESbit6L,regA				*  8
	
	FETCH						* 
	endopcode CBB7



*--- RES 7,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB8
	andi.w	#RESbit7H,regBC				*  8
	
	FETCH						* 
	endopcode CBB8



*--- RES 7,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBB9
	andi.b	#RESbit7L,regBC				*  8
	
	FETCH						* 
	endopcode CBB9



*--- RES 7,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBBA
	andi.w	#RESbit7H,regDE				*  8
	
	FETCH						* 
	endopcode CBBA



*--- RES 7,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBBB
	andi.b	#RESbit7L,regDE				*  8
	
	FETCH						* 
	endopcode CBBB



*--- RES 7,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBBC
	andi.w	#RESbit7H,regHL				*  8
	
	FETCH						* 
	endopcode CBBC



*--- RES 7,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBBD
	andi.b	#RESbit7L,regHL				*  8
	
	FETCH						* 
	endopcode CBBD



*--- RES 7,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBBE
	movea.l	regHL,a0				*  4
	andi.b	#RESbit7L,(a0)				* 16
	
	FETCH						* 
	endopcode CBBE



*--- RES 7,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBBF
	andi.b	#RESbit7L,regA				*  8
	
	FETCH						* 
	endopcode CBBF



*--- SET 0,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC0
	ori.w	#SETbit0H,regBC				*  8
	
	FETCH						* 
	endopcode CBC0



*--- SET 0,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC1
	ori.b	#SETbit0L,regBC				*  8
	
	FETCH						* 
	endopcode CBC1



*--- SET 0,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC2
	ori.w	#SETbit0H,regDE				*  8
	
	FETCH						* 
	endopcode CBC2



*--- SET 0,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC3
	ori.b	#SETbit0L,regDE				*  8
	
	FETCH						* 
	endopcode CBC3



*--- SET 0,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC4
	ori.w	#SETbit0H,regHL				*  8
	
	FETCH						* 
	endopcode CBC4



*--- SET 0,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC5
	ori.b	#SETbit0L,regHL				*  8
	
	FETCH						* 
	endopcode CBC5



*--- SET 0,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBC6
	movea.l	regHL,a0				*  4
	ori.b	#SETbit0L,(a0)				* 16
	
	FETCH						* 
	endopcode CBC6



*--- SET 0,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC7
	ori.b	#SETbit0L,regA				*  8
	
	FETCH						* 
	endopcode CBC7



*--- SET 1,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC8
	ori.w	#SETbit1H,regBC				*  8
	
	FETCH						* 
	endopcode CBC8



*--- SET 1,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBC9
	ori.b	#SETbit1L,regBC				*  8
	
	FETCH						* 
	endopcode CBC9



*--- SET 1,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBCA
	ori.w	#SETbit1H,regDE				*  8
	
	FETCH						* 
	endopcode CBCA



*--- SET 1,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBCB
	ori.b	#SETbit1L,regDE				*  8
	
	FETCH						* 
	endopcode CBCB



*--- SET 1,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBCC
	ori.w	#SETbit1H,regHL				*  8
	
	FETCH						* 
	endopcode CBCC



*--- SET 1,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBCD
	ori.b	#SETbit1L,regHL				*  8
	
	FETCH						* 
	endopcode CBCD



*--- SET 1,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBCE
	movea.l	regHL,a0				*  4
	ori.b	#SETbit1L,(a0)				* 16
	
	FETCH						* 
	endopcode CBCE



*--- SET 1,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBCF
	ori.b	#SETbit1L,regA				*  8
	
	FETCH						* 
	endopcode CBCF



*--- SET 2,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD0
	ori.w	#SETbit2H,regBC				*  8
	
	FETCH						* 
	endopcode CBD0



*--- SET 2,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD1
	ori.b	#SETbit2L,regBC				*  8
	
	FETCH						* 
	endopcode CBD1



*--- SET 2,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD2
	ori.w	#SETbit2H,regDE				*  8
	
	FETCH						* 
	endopcode CBD2



*--- SET 2,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD3
	ori.b	#SETbit2L,regDE				*  8
	
	FETCH						* 
	endopcode CBD3



*--- SET 2,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD4
	ori.w	#SETbit2H,regHL				*  8
	
	FETCH						* 
	endopcode CBD4



*--- SET 2,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD5
	ori.b	#SETbit2L,regHL				*  8
	
	FETCH						* 
	endopcode CBD5



*--- SET 2,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBD6
	movea.l	regHL,a0				*  4
	ori.b	#SETbit2L,(a0)				* 16
	
	FETCH						* 
	endopcode CBD6



*--- SET 2,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD7
	ori.b	#SETbit2L,regA				*  8
	
	FETCH						* 
	endopcode CBD7



*--- SET 3,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD8
	ori.w	#SETbit3H,regBC				*  8
	
	FETCH						* 
	endopcode CBD8



*--- SET 3,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBD9
	ori.b	#SETbit3L,regBC				*  8
	
	FETCH						* 
	endopcode CBD9



*--- SET 3,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBDA
	ori.w	#SETbit3H,regDE				*  8
	
	FETCH						* 
	endopcode CBDA



*--- SET 3,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBDB
	ori.b	#SETbit3L,regDE				*  8
	
	FETCH						* 
	endopcode CBDB



*--- SET 3,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBDC
	ori.w	#SETbit3H,regHL				*  8
	
	FETCH						* 
	endopcode CBDC



*--- SET 3,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBDD
	ori.b	#SETbit3L,regHL				*  8
	
	FETCH						* 
	endopcode CBDD



*--- SET 3,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBDE
	movea.l	regHL,a0				*  4
	ori.b	#SETbit3L,(a0)				* 16
	
	FETCH						* 
	endopcode CBDE



*--- SET 3,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBDF
	ori.b	#SETbit3L,regA				*  8
	
	FETCH						* 
	endopcode CBDF



*--- SET 4,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE0
	ori.w	#SETbit4H,regBC				*  8
	
	FETCH						* 
	endopcode CBE0



*--- SET 4,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE1
	ori.b	#SETbit4L,regBC				*  8
	
	FETCH						* 
	endopcode CBE1



*--- SET 4,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE2
	ori.w	#SETbit4H,regDE				*  8
	
	FETCH						* 
	endopcode CBE2



*--- SET 4,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE3
	ori.b	#SETbit4L,regDE				*  8
	
	FETCH						* 
	endopcode CBE3



*--- SET 4,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE4
	ori.w	#SETbit4H,regHL				*  8
	
	FETCH						* 
	endopcode CBE4



*--- SET 4,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE5
	ori.b	#SETbit4L,regHL				*  8
	
	FETCH						* 
	endopcode CBE5



*--- SET 4,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBE6
	movea.l	regHL,a0				*  4
	ori.b	#SETbit4L,(a0)				* 16
	
	FETCH						* 
	endopcode CBE6



*--- SET 4,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE7
	ori.b	#SETbit4L,regA				*  8
	
	FETCH						* 
	endopcode CBE7



*--- SET 5,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE8
	ori.w	#SETbit5H,regBC				*  8
	
	FETCH						* 
	endopcode CBE8



*--- SET 5,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBE9
	ori.b	#SETbit5L,regBC				*  8
	
	FETCH						* 
	endopcode CBE9



*--- SET 5,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBEA
	ori.w	#SETbit5H,regDE				*  8
	
	FETCH						* 
	endopcode CBEA



*--- SET 5,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBEB
	ori.b	#SETbit5L,regDE				*  8
	
	FETCH						* 
	endopcode CBEB



*--- SET 5,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBEC
	ori.w	#SETbit5H,regHL				*  8
	
	FETCH						* 
	endopcode CBEC



*--- SET 5,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBED
	ori.b	#SETbit5L,regHL				*  8
	
	FETCH						* 
	endopcode CBED



*--- SET 5,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBEE
	movea.l	regHL,a0				*  4
	ori.b	#SETbit5L,(a0)				* 16
	
	FETCH						* 
	endopcode CBEE



*--- SET 5,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBEF
	ori.b	#SETbit5L,regA				*  8
	
	FETCH						* 
	endopcode CBEF



*--- SET 6,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF0
	ori.w	#SETbit6H,regBC				*  8
	
	FETCH						* 
	endopcode CBF0



*--- SET 6,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF1
	ori.b	#SETbit6L,regBC				*  8
	
	FETCH						* 
	endopcode CBF1



*--- SET 6,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF2
	ori.w	#SETbit6H,regDE				*  8
	
	FETCH						* 
	endopcode CBF2



*--- SET 6,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF3
	ori.b	#SETbit6L,regDE				*  8
	
	FETCH						* 
	endopcode CBF3



*--- SET 6,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF4
	ori.w	#SETbit6H,regHL				*  8
	
	FETCH						* 
	endopcode CBF4



*--- SET 6,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF5
	ori.b	#SETbit6L,regHL				*  8
	
	FETCH						* 
	endopcode CBF5



*--- SET 6,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBF6
	movea.l	regHL,a0				*  4
	ori.b	#SETbit6L,(a0)				* 16
	
	FETCH						* 
	endopcode CBF6



*--- SET 6,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF7
	ori.b	#SETbit6L,regA				*  8
	
	FETCH						* 
	endopcode CBF7



*--- SET 7,B		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF8
	ori.w	#SETbit7H,regBC				*  8
	
	FETCH						* 
	endopcode CBF8



*--- SET 7,C		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBF9
	ori.b	#SETbit7L,regBC				*  8
	
	FETCH						* 
	endopcode CBF9



*--- SET 7,D		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBFA
	ori.w	#SETbit7H,regDE				*  8
	
	FETCH						* 
	endopcode CBFA



*--- SET 7,E		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBFB
	ori.b	#SETbit7L,regDE				*  8
	
	FETCH						* 
	endopcode CBFB



*--- SET 7,H		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBFC
	ori.w	#SETbit7H,regHL				*  8
	
	FETCH						* 
	endopcode CBFC



*--- SET 7,L		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBFD
	ori.b	#SETbit7L,regHL				*  8
	
	FETCH						* 
	endopcode CBFD



*--- SET 7,(HL)		15 (13)			SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode CBFE
	movea.l	regHL,a0				*  4
	ori.b	#SETbit7L,(a0)				* 16
	
	FETCH						* 
	endopcode CBFE



*--- SET 7,A		8 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode CBFF
	ori.b	#SETbit7L,regA				*  8
	
	FETCH						* 
	endopcode CBFF




*---------------------------------------------------------------
* DD xx



*---
	opcode DD00
	IllegalOP	2
	endopcode DD00



*---
	opcode DD01
	IllegalOP	2
	endopcode DD01



*---
	opcode DD02
	IllegalOP	2
	endopcode DD02



*---
	opcode DD03
	IllegalOP	2
	endopcode DD03



*---
	opcode DD04
	IllegalOP	2
	endopcode DD04



*---
	opcode DD05
	IllegalOP	2
	endopcode DD05



*---
	opcode DD06
	IllegalOP	2
	endopcode DD06



*---
	opcode DD07
	IllegalOP	2
	endopcode DD07



*---
	opcode DD08
	IllegalOP	2
	endopcode DD08



*--- ADD Ixy,BC		15 (10)			SZ H VNC
*						･･ x ･0*
	opcode DD09
	Fclr	______NCc				*  8
	
	add.w	regBC,(regIXY)				*  8
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode DD09



*---
	opcode DD0A
	IllegalOP	2
	endopcode DD0A



*---
	opcode DD0B
	IllegalOP	2
	endopcode DD0B



*---
	opcode DD0C
	IllegalOP	2
	endopcode DD0C



*---
	opcode DD0D
	IllegalOP	2
	endopcode DD0D



*---
	opcode DD0E
	IllegalOP	2
	endopcode DD0E



*---
	opcode DD0F
	IllegalOP	2
	endopcode DD0F



*---
	opcode DD10
	IllegalOP	2
	endopcode DD10



*---
	opcode DD11
	IllegalOP	2
	endopcode DD11



*---
	opcode DD12
	IllegalOP	2
	endopcode DD12



*---
	opcode DD13
	IllegalOP	2
	endopcode DD13



*---
	opcode DD14
	IllegalOP	2
	endopcode DD14



*---
	opcode DD15
	IllegalOP	2
	endopcode DD15



*---
	opcode DD16
	IllegalOP	2
	endopcode DD16



*---
	opcode DD17
	IllegalOP	2
	endopcode DD17



*---
	opcode DD18
	IllegalOP	2
	endopcode DD18



*--- ADD Ixy,DE		15 (10)			SZ H VNC
*						･･ x ･0*
	opcode DD19
	Fclr	______NCc				*  8
	
	add.w	regDE,(regIXY)				*  8
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode DD19



*---
	opcode DD1A
	IllegalOP	2
	endopcode DD1A



*---
	opcode DD1B
	IllegalOP	2
	endopcode DD1B



*---
	opcode DD1C
	IllegalOP	2
	endopcode DD1C



*---
	opcode DD1D
	IllegalOP	2
	endopcode DD1D



*---
	opcode DD1E
	IllegalOP	2
	endopcode DD1E



*---
	opcode DD1F
	IllegalOP	2
	endopcode DD1F



*---
	opcode DD20
	IllegalOP	2
	endopcode DD20



*--- LD Ixy,mn		14 (12)			SZ H VNC
*						･･ ･ ･･･
	opcode DD21
	move.b	(regPC)+,1(regIXY)			* 16	(L)
	move.b	(regPC)+,(regIXY)			* 12	(H)
	
	FETCH						* 
	endopcode DD21



*--- LD (mn),Ixy	20 (19)			SZ H VNC
*						･･ ･ ･･･
*	!! ページの境界をまたいだ書き込みのチェックはしていない
*	!! 上位バイトの書き込みのみチェック
	opcode DD22
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	
	CHECK_ROM regAdr,d0				* 
	
	movea.l	regAdr,a0				*  4
	
	move.b	1(regIXY),(a0)+				* 16	(L)
	move.b	(regIXY),(a0)				* 12	(H)
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_mn_Ixy(regFtbl)		* 
	endopcode DD22



*--- INC Ixy		10 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode DD23
	addq.w	#1,(regIXY)				* 12
	
	FETCH						* 
	endopcode DD23



*--- INC IxyH	**				SZ H VNC
*						** * V0･
	opcode DD24
	IllegalOP	2
	
	
	move.w	(regIXY),d0				*  8
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblINCrh(regZjmp,d0.w),regF		* 14
	
	addq.b	#1,(regIXY)				* 12
	
	FETCH						* 
	endopcode DD24



*--- DEC IxyH	**				SZ H VNC
*						** * V1･
	opcode DD25
	IllegalOP	2
	
	
	move.w	(regIXY),d0				*  8
	tst.b	regF					*  4
	smi.b	d0					*  6/ 4 (T/F)
	move.b	FtblDECrh(regZjmp,d0.w),regF		* 14
	
	subq.b	#1,(regIXY)				* 12
	
	FETCH						* 
	endopcode DD25



*--- LD IxyH,n	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD26
	IllegalOP	2
	
	
	move.b	(regPC)+,(regIXY)			* 12
	
	FETCH						* 
	endopcode DD26



*---
	opcode DD27
	IllegalOP	2
	endopcode DD27



*---
	opcode DD28
	IllegalOP	2
	endopcode DD28



*--- ADD Ixy,Ixy	15 (10)			SZ H VNC
*						･･ x ･0*
	opcode DD29
	Fclr	______NCc				*  8
	
	asl.w	(regIXY)				* 12
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode DD29



*--- LD Ixy,(mn)	20 (18)			SZ H VNC
*						･･ ･ ･･･
	opcode DD2A
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0)+,1(regIXY)				* 16
	move.b	(a0),(regIXY)				* 12
	
	FETCH						* 
	endopcode DD2A



*--- DEC Ixy		10 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode DD2B
	subq.w	#1,(regIXY)				* 12
	
	FETCH						* 
	endopcode DD2B



*--- INC IxyL	**				SZ H VNC
*						** * V0･
	opcode DD2C
	IllegalOP	2
	
	
	lea	FtblINCrl(regFtbl),a0			*  8
	ext.w	regF					*  4
	move.b	1(regIXY),regF				* 12
	move.b	(a0,regF.w),regF			* 14
	
	addq.b	#1,1(regIXY)				* 16
	
	FETCH						* 
	endopcode DD2C



*--- DEC IxyL	**				SZ H VNC
*						** * V1･
	opcode DD2D
	IllegalOP	2
	
	
	move.b	1(regIXY),d0				* 12
	lea	FtblDECrl(regFtbl),a0			*  8
	ext.w	d0					*  4
	move.b	d0,regF					*  4
	or.b	(a0,regF.w),regF			* 14
	
	subq.b	#1,1(regIXY)				* 16
	
	FETCH						* 
	endopcode DD2D



*--- LD IxyL,n	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD2E
	IllegalOP	2
	
	
	move.b	(regPC)+,1(regIXY)			* 16
	
	FETCH						* 
	endopcode DD2E



*---
	opcode DD2F
	IllegalOP	2
	endopcode DD2F


*---
	opcode DD30
	IllegalOP	2
	endopcode DD30



*---
	opcode DD31
	IllegalOP	2
	endopcode DD31



*---
	opcode DD32
	IllegalOP	2
	endopcode DD32



*---
	opcode DD33
	IllegalOP	2
	endopcode DD33



*--- INC (Ixy+d)	23 (18)			SZ H VNC
*						** * V0･
*	!! ROM領域への書き込みのチェックはしない
	opcode DD34
	move.b	(regPC)+,regAdr				*  4
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	ext.w	regF					*  4
	move.b	(a0),regF				*  8
	
	addq.b	#1,(a0)					* 12
	
	lea	FtblINCrl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DD34



*--- DEC (Ixy+d)	23 (18)			SZ H VNC
*						** * V1･
*	!! ROM領域への書き込みのチェックはしない
	opcode DD35
	move.b	(regPC)+,regAdr				*  4
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	ext.w	regF					*  4
	move.b	(a0),regF				*  8
	
	subq.b	#1,(a0)					* 12
	
	lea	FtblDECrl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DD35



*--- LD (Ixy+d),n	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD36
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_n,0		* 
	
	move.b	(regPC)+,d1				*  8
	
	CHECK_ROM	regAdr,d0			* 
	
	move.b	d1,(a0)					*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_n(regFtbl)			* 
	endopcode DD36



*---
	opcode DD37
	IllegalOP	2
	endopcode DD37



*---
	opcode DD38
	IllegalOP	2
	endopcode DD38



*--- ADD Ixy,SP	15 (10)				SZ H VNC
*						･･ x ･0*
	opcode DD39
	Fclr	______NCc				*  8
	
	move.w	regSP,d0				*  4
	add.w	d0,(regIXY)				* 12
	bcc.s	@f					* 10/ 8 (T/F)
		Fset	_______Cc			*     8
@@:
	FETCH						* 
	endopcode DD39



*---
	opcode DD3A
	IllegalOP	2
	endopcode DD3A



*---
	opcode DD3B
	IllegalOP	2
	endopcode DD3B



*---
	opcode DD3C
	IllegalOP	2
	endopcode DD3C



*---
	opcode DD3D
	IllegalOP	2
	endopcode DD3D



*---
	opcode DD3E
	IllegalOP	2
	endopcode DD3E



*---
	opcode DD3F
	IllegalOP	2
	endopcode DD3F



*---
	opcode DD40
	IllegalOP	2
	endopcode DD40



*---
	opcode DD41
	IllegalOP	2
	endopcode DD41



*---
	opcode DD42
	IllegalOP	2
	endopcode DD42



*---
	opcode DD43
	IllegalOP	2
	endopcode DD43



*--- LD B,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD44
	IllegalOP	2
	
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,(regIXY)			* 12
	
	FETCH						* 
	endopcode DD44



*--- LD B,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD45
	IllegalOP	2
	
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,1(regIXY)			* 16
	
	FETCH						* 
	endopcode DD45



*--- LD B,(Ixy+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD46
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.w	regBC,-(regSwap)			*  8
	move.b	(a0),(regSwap)				* 12
	move.w	(regSwap)+,regBC			*  8
	
	FETCH						* 
	endopcode DD46



*---
	opcode DD47
	IllegalOP	2
	endopcode DD47



*---
	opcode DD48
	IllegalOP	2
	endopcode DD48



*---
	opcode DD49
	IllegalOP	2
	endopcode DD49



*---
	opcode DD4A
	IllegalOP	2
	endopcode DD4A



*---
	opcode DD4B
	IllegalOP	2
	endopcode DD4B



*--- LD C,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD4C
	IllegalOP	2
	
	
	move.b	regBC,(regIXY)				*  8
	
	FETCH						* 
	endopcode DD4C



*--- LD C,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD4D
	IllegalOP	2
	
	
	move.b	regBC,1(regIXY)				* 12
	
	FETCH						* 
	endopcode DD4D



*--- LD C,(Ixy+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD4E
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),regBC				*  8
	
	FETCH						* 
	endopcode DD4E



*---
	opcode DD4F
	IllegalOP	2
	endopcode DD4F



*---
	opcode DD50
	IllegalOP	2
	endopcode DD50



*---
	opcode DD51
	IllegalOP	2
	endopcode DD51



*---
	opcode DD52
	IllegalOP	2
	endopcode DD52



*---
	opcode DD53
	IllegalOP	2
	endopcode DD53



*--- LD D,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD54
	IllegalOP	2
	
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,(regIXY)			* 12
	
	FETCH						* 
	endopcode DD54



*--- LD D,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD55
	IllegalOP	2
	
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,1(regIXY)			* 16
	
	FETCH						* 
	endopcode DD55



*--- LD D,(IX+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD56
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.w	regDE,-(regSwap)			*  8
	move.b	(a0),(regSwap)				* 12
	move.w	(regSwap)+,regDE			*  8
	
	FETCH						* 
	endopcode DD56



*---
	opcode DD57
	IllegalOP	2
	endopcode DD57



*---
	opcode DD58
	IllegalOP	2
	endopcode DD58



*---
	opcode DD59
	IllegalOP	2
	endopcode DD59



*---
	opcode DD5A
	IllegalOP	2
	endopcode DD5A



*---
	opcode DD5B
	IllegalOP	2
	endopcode DD5B



*--- LD E,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD5C
	IllegalOP	2
	
	
	move.b	(regIXY),regDE				*  8
	
	FETCH						* 
	endopcode DD5C



*--- LD E,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD5D
	IllegalOP	2
	
	
	move.b	1(regIXY),regDE				* 12
	
	FETCH						* 
	endopcode DD5D



*--- LD E,(Ixy+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD5E
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),regDE				*  8
	
	FETCH						* 
	endopcode DD5E



*---
	opcode DD5F
	IllegalOP	2
	endopcode DD5F



*---
	opcode DD60
	IllegalOP	2
	endopcode DD60



*---
	opcode DD61
	IllegalOP	2
	endopcode DD61



*---
	opcode DD62
	IllegalOP	2
	endopcode DD62



*---
	opcode DD63
	IllegalOP	2
	endopcode DD63



*--- LD H,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD64
	IllegalOP	2
	
	
	move.b	regHL,d0				*  4
	move.w	(regIXY),regHL				*  8
	move.b	d0,regHL				*  4
	
	FETCH						* 
	endopcode DD64



*--- LD H,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD65
	IllegalOP	2
	
	
	move.w	regHL,-(regSwap)			*  8
	move.b	1(regIXY),(regSwap)			* 16
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* 
	endopcode DD65



*--- LD H,(Ixy+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD66
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.w	regHL,-(regSwap)			*  8
	move.b	(a0),(regSwap)				* 12
	move.w	(regSwap)+,regHL			*  8
	
	FETCH						* 
	endopcode DD66



*---
	opcode DD67
	IllegalOP	2
	endopcode DD67



*---
	opcode DD68
	IllegalOP	2
	endopcode DD68



*---
	opcode DD69
	IllegalOP	2
	endopcode DD69



*---
	opcode DD6A
	IllegalOP	2
	endopcode DD6A



*---
	opcode DD6B
	IllegalOP	2
	endopcode DD6B



*--- LD L,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD6C
	IllegalOP	2
	
	
	move.b	(regIXY),regHL				*  8
	
	FETCH						* 
	endopcode DD6C



*--- LD L,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD6D
	IllegalOP	2
	
	
	move.b	1(regIXY),regHL				* 12
	
	FETCH					*
	endopcode DD6D



*--- LD L,(Ixy+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD6E
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),regHL				*  8
	
	FETCH						* 
	endopcode DD6E



*---
	opcode DD6F
	IllegalOP	2
	endopcode DD6F



*--- LD (Ixy+d),B	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD70
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_B,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_B(regFtbl)			* 
	endopcode DD70



*--- LD (Ixy+d),C	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD71
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_C,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.b	regBC,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_C(regFtbl)			* 
	endopcode DD71



*--- LD (Ixy+d),D	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD72
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_D,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_D(regFtbl)			* 
	endopcode DD72



*--- LD (Ixy+d),E	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD73
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_E,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.b	regDE,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_E(regFtbl)			* 
	endopcode DD73



*--- LD (Ixy+d),H	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD74
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_H,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_H(regFtbl)			* 
	endopcode DD74



*--- LD (Ixy+d),L	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD75
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_L,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.b	regHL,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_L(regFtbl)			* 
	endopcode DD75



*---
	opcode DD76
	IllegalOP	2

_fetch_for_halt_1:
	*-- 割り込み禁止状態ならZ80デバッガへ
	tst.b	Z80_DIEI(regFtbl)			* 16
	bne.s	_fetch_for_halt_2			* 
	
	ENTER_DEBUGGER2					* 

_fetch_for_halt_2:
	move.w	(regOP1fetch),d1			*  8
	jmp	OP_Z(regZjmp,d1.w)			* 14
	endopcode DD76



*--- LD (Ixy+d),A	19 (15)			SZ H VNC
*						･･ ･ ･･･
	opcode DD77
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	CHECK_EXTSLOT	regAdr,ExtSlotSrc_A,0		* 
	
	CHECK_ROM	regAdr,d0			* 
	
	move.b	regA,(a0)				*  8
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_Ixy_A(regFtbl)			* 
	endopcode DD77



*---
	opcode DD78
	IllegalOP	2
	endopcode DD78



*---
	opcode DD79
	IllegalOP	2
	endopcode DD79



*---
	opcode DD7A
	IllegalOP	2
	endopcode DD7A



*---
	opcode DD7B
	IllegalOP	2
	endopcode DD7B



*--- LD A,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD7C
	IllegalOP	2
	
	
	move.b	(regIXY),regA				*  8
	
	FETCH						* 
	endopcode DD7C



*--- LD A,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DD7D
	IllegalOP	2
	
	
	move.b	1(regIXY),regA				* 12
	
	FETCH						* 
	endopcode DD7D



*--- LD A,(Ixy+d)	19 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DD7E
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),regA				*  8
	
	FETCH						* 
	endopcode DD7E



*---
	opcode DD7F
	IllegalOP	2
	endopcode DD7F



*---
	opcode DD80
	IllegalOP	2
	endopcode DD80



*---
	opcode DD81
	IllegalOP	2
	endopcode DD81



*---
	opcode DD82
	IllegalOP	2
	endopcode DD82



*---
	opcode DD83
	IllegalOP	2
	endopcode DD83



*--- ADD A,IxyH	**				SZ H VNC
*						** * V0*
	opcode DD84
	IllegalOP	2
	
	
	move.w	(regIXY),d1				*  8
	move.b	regA,d1					*  4
	
	add.b	(regIXY),regA				*  8
	
	lea	FtblADD8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DD84



*--- ADD A,IxyL	**				SZ H VNC
*						** * V0*
	opcode DD85
	IllegalOP	2
	
	
	move.b	1(regIXY),-(regSwap)			* 16
	move.w	(regSwap),d1				*  8
	move.b	regA,d1					*  4
	
	add.b	(regSwap)+,regA				*  8
	
	lea	FtblADD8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DD85



*--- ADD A,(Ixy+d)	19 (14)			SZ H VNC
*						** * V0*
	opcode DD86
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4
	
	add.b	(a0),regA				*  8
	
	lea	FtblADD8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DD86



*---
	opcode DD87
	IllegalOP	2
	endopcode DD87



*---
	opcode DD88
	IllegalOP	2
	endopcode DD88



*---
	opcode DD89
	IllegalOP	2
	endopcode DD89



*---
	opcode DD8A
	IllegalOP	2
	endopcode DD8A



*---
	opcode DD8B
	IllegalOP	2
	endopcode DD8B



*--- ADC A,IxyH	**				SZ H VNC
*						** * V0*
	opcode DD8C
	IllegalOP	2
	
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	(regIXY),d0				*  8
	move.b	regA,d0					*  4
	
	move.b	(regIXY),d1				*  8
	add.b	regF,regF				*  4
	addx.b	d1,regA					*  4
	
	lea	FtblADD8,a0				* 12
	move.b	(a0,d0.l),regF				* 14 = 66
	
	FETCH						* 
	endopcode DD8C



*--- ADC A,IxyL	**				SZ H VNC
*						** * V0*
	opcode DD8D
	IllegalOP	2
	
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	1(regIXY),d0				* 12
	
	add.b	regF,regF				*  4
	addx.b	d0,regA					*  4
	
	lea	FtblADD8,a0				* 12
	move.b	(a0,d0.l),regF				* 14
	
	FETCH						* = 74
	endopcode DD8D



*--- ADC A,(Ixy+d)	19 (14)			SZ H VNC
*						** * V0*
	opcode DD8E
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(a0),d0					*  8
	
	add.b	regF,regF				*  4
	addx.b	d0,regA					*  4
	
	lea	FtblADD8,a0				* 12
	move.b	(a0,d0.l),regF				* 14
	
	FETCH						* 
	endopcode DD8E



*---
	opcode DD8F
	IllegalOP	2
	endopcode DD8F



*---
	opcode DD90
	IllegalOP	2
	endopcode DD90



*---
	opcode DD91
	IllegalOP	2
	endopcode DD91



*---
	opcode DD92
	IllegalOP	2
	endopcode DD92



*---
	opcode DD93
	IllegalOP	2
	endopcode DD93



*--- SUB IxyH	**				SZ H VNC
*						** * V1*
	opcode DD94
	IllegalOP	2
	
	
	move.w	(regIXY),d1				*  8
	move.b	regA,d1					*  4
	
	sub.b	(regIXY),regA				*  8
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DD94



*--- SUB IxyL	**				SZ H VNC
*						** * V1*
	opcode DD95
	IllegalOP	2
	
	
	move.b	1(regIXY),-(regSwap)			* 16
	move.w	(regSwap),d1				*  8
	move.b	regA,d1					*  4
	
	sub.b	(regSwap)+,regA				*  8
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DD95



*--- SUB (Ixy+d)	19 (14)			SZ H VNC
*						** * V1*
	opcode DD96
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	sub.b	(a0),regA				*  8
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DD96



*---
	opcode DD97
	IllegalOP	2
	endopcode DD97



*---
	opcode DD98
	IllegalOP	2
	endopcode DD98



*---
	opcode DD99
	IllegalOP	2
	endopcode DD99



*---
	opcode DD9A
	IllegalOP	2
	endopcode DD9A



*---
	opcode DD9B
	IllegalOP	2
	endopcode DD9B



*--- SBC A,IxyH	**				SZ H VNC
*						** * V1*
	opcode DD9C
	IllegalOP	2
	
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	(regIXY),d0				*  8
	move.b	regA,d0					*  4
	
	move.b	(regIXY),d1				*  8
	add.b	regF,regF				*  4
	subx.b	d1,regA					*  4
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d0.l),regF				* 14
	
	FETCH						* 
	endopcode DD9C



*--- SBC A,IxyL	**				SZ H VNC
*						** * V1*
	opcode DD9D
	IllegalOP	2
	
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.w	(regIXY),d1				*  8
	move.b	d1,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4
	
	add.b	regF,regF				*  4
	subx.b	d1,regA					*  4
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d0.l),regF				* 14
	
	FETCH						* 
	endopcode DD9D



*--- SBC A,(Ixy+d)	19 (14)			SZ H VNC
*						** * V1*
	opcode DD9E
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	move.b	(a0),d1					*  8
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	ext.l	d0					*  4
	move.b	d1,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	regA,d0					*  4	[src:dst] (dst-src)
	
	add.b	regF,regF				*  4
	subx.b	d1,regA					*  4
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d0.l),regF				* 14
	
	FETCH						* 
	endopcode DD9E



*---
	opcode DD9F
	IllegalOP	2
	endopcode DD9F



*---
	opcode DDA0
	IllegalOP	2
	endopcode DDA0



*---
	opcode DDA1
	IllegalOP	2
	endopcode DDA1



*---
	opcode DDA2
	IllegalOP	2
	endopcode DDA2



*---
	opcode DDA3
	IllegalOP	2
	endopcode DDA3



*--- AND IxyH	**				SZ H VNC
*						** 1 P00
	opcode DDA4
	IllegalOP	2
	
	
	and.b	(regIXY),regA				*  8
	
	lea	FtblANDr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDA4



*--- AND IxyL	**				SZ H VNC
*						** 1 P00
	opcode DDA5
	IllegalOP	2
	
	
	and.b	1(regIXY),regA				* 12
	
	lea	FtblANDr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDA5



*--- AND (Ixy+d)	19 (14)			SZ H VNC
*						** 1 P00
	opcode DDA6
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	and.b	(a0),regA				*  8
	
	lea	FtblANDr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDA6



*---
	opcode DDA7
	IllegalOP	2
	endopcode DDA7



*---
	opcode DDA8
	IllegalOP	2
	endopcode DDA8



*---
	opcode DDA9
	IllegalOP	2
	endopcode DDA9



*---
	opcode DDAA
	IllegalOP	2
	endopcode DDAA



*---
	opcode DDAB
	IllegalOP	2
	endopcode DDAB



*--- XOR IxyH	**				SZ H VNC
*						** 0 P00
	opcode DDAC
	IllegalOP	2
	
	
	move.b	(regIXY),d0				*  8
	eor.b	d0,regA					*  4
	
	lea	FtblXORr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDAC



*--- XOR IxyL	**				SZ H VNC
*						** 0 P00
	opcode DDAD
	IllegalOP	2
	
	
	move.w	(regIXY),d0				*  8
	eor.b	d0,regA					*  4
	
	lea	FtblXORr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDAD



*--- XOR (Ixy+d)	19 (14)			SZ H VNC
*						** 0 P00
	opcode DDAE
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	move.b	(a0),d0					*  8
	eor.b	d0,regA					*  4
	
	lea	FtblXORr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDAE



*---
	opcode DDAF
	IllegalOP	2
	endopcode DDAF



*---
	opcode DDB0
	IllegalOP	2
	endopcode DDB0



*---
	opcode DDB1
	IllegalOP	2
	endopcode DDB1



*---
	opcode DDB2
	IllegalOP	2
	endopcode DDB2



*---
	opcode DDB3
	IllegalOP	2
	endopcode DDB3



*--- OR IxyH	**				SZ H VNC
*						** 0 P00
	opcode DDB4
	IllegalOP	2
	
	
	or.b	(regIXY),regA				*  8
	
	lea	FtblORr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDB4



*--- OR IxyL	**				SZ H VNC
*						** 0 P00
	opcode DDB5
	IllegalOP	2
	
	
	or.b	1(regIXY),regA				* 12
	
	lea	FtblORr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDB5



*--- OR (Ixy+d)		19 (14)			SZ H VNC
*						** 0 P00
	opcode DDB6
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	or.b	(a0),regA				*  8
	
	lea	FtblORr(regFtbl),a0			*  8
	move.b	(a0,regA.w),regF			* 14
	
	FETCH						* 
	endopcode DDB6



*---
	opcode DDB7
	IllegalOP	2
	endopcode DDB7



*---
	opcode DDB8
	IllegalOP	2
	endopcode DDB8



*---
	opcode DDB9
	IllegalOP	2
	endopcode DDB9



*---
	opcode DDBA
	IllegalOP	2
	endopcode DDBA



*---
	opcode DDBB
	IllegalOP	2
	endopcode DDBB



*--- CP IxyH	**				SZ H VNC
*						** * V1*
	opcode DDBC
	IllegalOP	2
	
	
	move.w	(regIXY),d1				*  8
	move.b	regA,d1					*  4
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DDBC



*--- CP IxyL	**				SZ H VNC
*						** * V1*
	opcode DDBD
	IllegalOP	2
	
	
	move.b	1(regIXY),-(regSwap)			* 16
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DDBD



*--- CP (Ixy+d)		19 (14)			SZ H VNC
*						** * V1*
	opcode DDBE
	move.b	(regPC)+,regAdr				*  8
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	
	lea	FtblSUB8,a0				* 12
	move.b	(a0,d1.l),regF				* 14
	
	FETCH						* 
	endopcode DDBE



*---
	opcode DDBF
	IllegalOP	2
	endopcode DDBF



*---
	opcode DDC0
	IllegalOP	2
	endopcode DDC0



*---
	opcode DDC1
	IllegalOP	2
	endopcode DDC1



*---
	opcode DDC2
	IllegalOP	2
	endopcode DDC2



*---
	opcode DDC3
	IllegalOP	2
	endopcode DDC3



*---
	opcode DDC4
	IllegalOP	2
	endopcode DDC4



*---
	opcode DDC5
	IllegalOP	2
	endopcode DDC5



*---
	opcode DDC6
	IllegalOP	2
	endopcode DDC6



*---
	opcode DDC7
	IllegalOP	2
	endopcode DDC7



*---
	opcode DDC8
	IllegalOP	2
	endopcode DDC8



*---
	opcode DDC9
	IllegalOP	2
	endopcode DDC9



*---
	opcode DDCA
	IllegalOP	2
	endopcode DDCA



*---------------
* DD CB -d- XX
	opcode DDCB
	move.b	(regPC)+,regAdr				*  8
	
	move.b	(regPC)+,(regOP2fetch)			* 12
	move.w	(regOP2fetch),d0			*  8
	jmp	OP_DDCB(regZjmp,d0.w)			* 14
	endopcode DDCB



*---
	opcode DDCC
	IllegalOP	2
	endopcode DDCC



*---
	opcode DDCD
	IllegalOP	2
	endopcode DDCD



*---
	opcode DDCE
	IllegalOP	2
	endopcode DDCE



*---
	opcode DDCF
	IllegalOP	2
	endopcode DDCF



*---
	opcode DDD0
	IllegalOP	2
	endopcode DDD0



*---
	opcode DDD1
	IllegalOP	2
	endopcode DDD1



*---
	opcode DDD2
	IllegalOP	2
	endopcode DDD2



*---
	opcode DDD3
	IllegalOP	2
	endopcode DDD3



*---
	opcode DDD4
	IllegalOP	2
	endopcode DDD4



*---
	opcode DDD5
	IllegalOP	2
	endopcode DDD5



*---
	opcode DDD6
	IllegalOP	2
	endopcode DDD6



*---
	opcode DDD7
	IllegalOP	2
	endopcode DDD7



*---
	opcode DDD8
	IllegalOP	2
	endopcode DDD8



*---
	opcode DDD9
	IllegalOP	2
	endopcode DDD9



*---
	opcode DDDA
	IllegalOP	2
	endopcode DDDA



*---
	opcode DDDB
	IllegalOP	2
	endopcode DDDB



*---
	opcode DDDC
	IllegalOP	2
	endopcode DDDC



*---
	opcode DDDD
	IllegalOP	2
	endopcode DDDD



*---
	opcode DDDE
	IllegalOP	2
	endopcode DDDE



*---
	opcode DDDF
	IllegalOP	2
	endopcode DDDF



*---
	opcode DDE0
	IllegalOP	2
	endopcode DDE0



*--- POP Ixy		14 (12)			SZ H VNC
*						･･ ･ ･･･
	opcode DDE1
	move.b	(regSP)+,1(regIXY)			* 16
	move.b	(regSP)+,(regIXY)			* 12
	
	FETCH						* 
	endopcode DDE1



*---
	opcode DDE2
	IllegalOP	2
	endopcode DDE2



*--- EX (SP),Ixy	23 (19)			SZ H VNC
*						･･ ･ ･･･
	opcode DDE3
	move.b	(regIXY)+,d0				*  8	(H)
	move.b	(regIXY)+,d1				*  8	(L)
	
	move.b	(regSP)+,-(regIXY)			* 12	(L)
	move.b	(regSP)+,-(regIXY)			* 12	(H)
	
	move.b	d0,-(regSP)				*  8	(H)
	move.b	d1,-(regSP)				*  8	(L)
	
	FETCH						* 
	endopcode DDE3



*---
	opcode DDE4
	IllegalOP	2
	endopcode DDE4



*--- PUSH Ixy		15 (14)			SZ H VNC
*						･･ ･ ･･･
	opcode DDE5
	move.b	(regIXY)+,-(regSP)			* 12	(H)
	move.b	(regIXY)+,-(regSP)			* 12	(L)
	
	FETCH						* 
	endopcode DDE5



*---
	opcode DDE6
	IllegalOP	2
	endopcode DDE6



*---
	opcode DDE7
	IllegalOP	2
	endopcode DDE7



*---
	opcode DDE8
	IllegalOP	2
	endopcode DDE8



*--- JP (Ixy)		8 (6)			SZ H VNC
*						･･ ･ ･･･
	opcode DDE9
	move.w	(regIXY),regAdr				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode DDE9



*---
	opcode DDEA
	IllegalOP	2
	endopcode DDEA



*---
	opcode DDEB
	IllegalOP	2
	endopcode DDEB



*---
	opcode DDEC
	IllegalOP	2
	endopcode DDEC



*---
	opcode DDED
	IllegalOP	2
	endopcode DDED



*---
	opcode DDEE
	IllegalOP	2
	endopcode DDEE



*---
	opcode DDEF
	IllegalOP	2
	endopcode DDEF



*---
	opcode DDF0
	IllegalOP	2
	endopcode DDF0



*---
	opcode DDF1
	IllegalOP	2
	endopcode DDF1



*---
	opcode DDF2
	IllegalOP	2
	endopcode DDF2



*---
	opcode DDF3
	IllegalOP	2
	endopcode DDF3



*---
	opcode DDF4
	IllegalOP	2
	endopcode DDF4



*---
	opcode DDF5
	IllegalOP	2
	endopcode DDF5



*---
	opcode DDF6
	IllegalOP	2
	endopcode DDF6



*---
	opcode DDF7
	IllegalOP	2
	endopcode DDF7



*---
	opcode DDF8
	IllegalOP	2
	endopcode DDF8



*--- LD SP,Ixy		10 (7)			SZ H VNC
*						･･ ･ ･･･
	opcode DDF9
	move.w	(regIXY),regAdr				*  8
	movea.l	regAdr,regSP				*  4
	
	FETCH						* 
	endopcode DDF9



*---
	opcode DDFA
	IllegalOP	2
	endopcode DDFA



*---
	opcode DDFB
	IllegalOP	2
	endopcode DDFB



*---
	opcode DDFC
	IllegalOP	2
	endopcode DDFC



*---
	opcode DDFD
	IllegalOP	2
	endopcode DDFD



*---
	opcode DDFE
	IllegalOP	2
	endopcode DDFE



*---
	opcode DDFF
	IllegalOP	2
	endopcode DDFF



*---------------------------------------------------------------------------------------
* DD CB -d- xx
*
*	; regAdr.b に -d- が設定された状態で実行される。（ext.wはしていない）




*--- LD B,RLC(Ixy+d)	**
	opcode DDCB00
	IllegalOP	4
	endopcode DDCB00



*--- LD C,RLC(Ixy+d)	**
	opcode DDCB01
	IllegalOP	4
	endopcode DDCB01



*--- LD D,RLC(Ixy+d)	**
	opcode DDCB02
	IllegalOP	4
	endopcode DDCB02



*--- LD E,RLC(Ixy+d)	**
	opcode DDCB03
	IllegalOP	4
	endopcode DDCB03



*--- RLC IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB04
	IllegalOP	4
	
	
	moveq.l	#0,d0					*  4
	move.b	(regIXY),d0				*  8
	
	lea	FtblRLCrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	rol.b	#1,d0					*  8
	move.b	d0,(regIXY)				*  8
	
	FETCH						* 
	endopcode DDCB04



*--- RLC IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB05
	IllegalOP	4
	
	
	move.w	(regIXY),d1				*  8
	
	moveq.l	#0,d0					*  4
	move.b	d1,d0					*  4
	
	lea	FtblRLCrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	rol.b	#1,d1					*  8
	move.w	d1,(regIXY)				*  8
	
	FETCH						* 
	endopcode DDCB05



*--- RLC (Ixy+d)	23 (19)			SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB06
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	moveq.l	#0,d0					*  4
	move.b	(a0),d0					*  8
	
	move.w	d0,regF					*  4
	
	rol.b	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	lea	FtblRLCrl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB06



*---
	opcode DDCB07
	IllegalOP	4
	endopcode DDCB07



*---
	opcode DDCB08
	IllegalOP	4
	endopcode DDCB08



*---
	opcode DDCB09
	IllegalOP	4
	endopcode DDCB09



*---
	opcode DDCB0A
	IllegalOP	4
	endopcode DDCB0A



*---
	opcode DDCB0B
	IllegalOP	4
	endopcode DDCB0B



*--- RRC IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB0C
	IllegalOP	4
	
	
	moveq.l	#0,d0					*  4
	move.b	(regIXY),d0				*  8
	
	lea	FtblRRCrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	ror.b	#1,d0					*  8
	move.b	d0,(regIXY)				*  8
	
	FETCH						* 
	endopcode DDCB0C



*--- RRC IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB0D
	IllegalOP	4
	
	
	move.w	(regIXY),d1				*  8
	
	moveq.l	#0,d0					*  4
	move.b	d1,d0					*  4
	lea	FtblRRCrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	ror.b	#1,d1					*  8
	move.w	d1,(regIXY)				*  8
	
	FETCH						* 
	endopcode DDCB0D



*--- RRC (Ixy+d)				SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB0E
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	moveq.l	#0,d0					*  4
	move.b	(a0),d0					*  8
	
	move.w	d0,regF					*  4
	
	ror.b	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	lea	FtblRRCrl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB0E



*---
	opcode DDCB0F
	IllegalOP	4
	endopcode DDCB0F



*---
	opcode DDCB10
	IllegalOP	4
	endopcode DDCB10



*---
	opcode DDCB11
	IllegalOP	4
	endopcode DDCB11



*---
	opcode DDCB12
	IllegalOP	4
	endopcode DDCB12



*---
	opcode DDCB13
	IllegalOP	4
	endopcode DDCB13



*--- RL IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB14
	IllegalOP	4
	
	
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	(regIXY),d0				*  8
	
	lea	FtblRLrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,(regIXY)				*  8
	
	FETCH						* 
	endopcode DDCB14



*--- RL IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB15
	IllegalOP	4
	
	
.if 0
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	1(regIXY),d0				* 12
	
	lea	FtblRLrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	rol.w	#1,d0					*  8
	move.b	d0,1(regIXY)				* 12
	
	FETCH						*
.endif
	endopcode DDCB15



*--- RL (IX+d)					SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB16
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	ext.w	regF					*  4
	move.b	(a0),regF				*  8
	
	move.w	regF,d0					*  4
	
	rol.w	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	lea	FtblRLrl(regFtbl),a1			*  8
	move.b	(a1,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB16



*---
	opcode DDCB17
	IllegalOP	4
	endopcode DDCB17



*---
	opcode DDCB18
	IllegalOP	4
	endopcode DDCB18



*---
	opcode DDCB19
	IllegalOP	4
	endopcode DDCB19



*---
	opcode DDCB1A
	IllegalOP	4
	endopcode DDCB1A



*---
	opcode DDCB1B
	IllegalOP	4
	endopcode DDCB1B



*--- RR IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB1C
	IllegalOP	4
	
	
.if 0
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	(regIXY),d0				*  8
	
	lea	FtblRRrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,(regIXY)				*  8
	
	FETCH						* 
.endif
	endopcode DDCB1C



*--- RR IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB1D
	IllegalOP	4
	
	
.if 0
	move.b	regF,d0					*  4
	ext.w	d0					*  4
	move.b	1(regIXY),d0				* 12
	
	lea	FtblRRrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	ror.w	#1,d0					*  8
	move.b	d0,1(regIXY)				* 12
	
	FETCH						* 
.endif
	endopcode DDCB1D



*--- RR (Ixy+d)					SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB1E
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	ext.w	regF					*  4
	move.b	(a0),regF				*  8
	
	move.w	regF,d0					*  4
	
	ror.w	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	lea	FtblRRrl(regFtbl),a1			* 12
	move.b	(a1,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB1E



*---
	opcode DDCB1F
	IllegalOP	4
	endopcode DDCB1F



*---
	opcode DDCB20
	IllegalOP	4
	endopcode DDCB20



*---
	opcode DDCB21
	IllegalOP	4
	endopcode DDCB21



*---
	opcode DDCB22
	IllegalOP	4
	endopcode DDCB22



*---
	opcode DDCB23
	IllegalOP	4
	endopcode DDCB23



*--- SLA IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB24
	IllegalOP	4
	
	
.if 0
	moveq.l	#0,d0					*  4
	move.b	(regIXY),d0				*  8
	lea	FtblSLArl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	add.b	d0,(regIXY)				* 12
	
	FETCH						* 
.endif
	endopcode DDCB24



*--- SLA IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB25
	IllegalOP	4
	
	
.if 0
	move.w	(regIXY),d1				*  8
	
	moveq.l	#0,d0					*  4
	move.b	d1,d0					*  4
	lea	FtblSLArl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	add.b	d1,d1					*  4
	move.w	d1,(regIXY)				*  8
	
	FETCH						* 
.endif
	endopcode DDCB25



*--- SLA (Ixy+d)				SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB26
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	moveq.l	#0,d0					*  4
	move.b	(a0),d0					*  8
	
	move.w	d0,regF					*  4
	
	add.b	d0,(a0)					* 12
	
	lea	FtblSLArl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB26



*---
	opcode DDCB27
	IllegalOP	4
	endopcode DDCB27



*---
	opcode DDCB28
	IllegalOP	4
	endopcode DDCB28



*---
	opcode DDCB29
	IllegalOP	4
	endopcode DDCB29



*---
	opcode DDCB2A
	IllegalOP	4
	endopcode DDCB2A



*---
	opcode DDCB2B
	IllegalOP	4
	endopcode DDCB2B



*--- SRA IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB2C
	IllegalOP	4
	
	
.if 0
	moveq.l	#0,d0					*  4
	move.b	(regIXY),d0				*  8
	lea	FtblSRArl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	asr.b	#1,d0					*  8
	move.b	d0,(regIXY)				*  8
	
	FETCH						* 
.endif
	endopcode DDCB2C



*--- SRA IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB2D
	IllegalOP	4
	
	
.if 0
	move.w	(regIXY),d1				*  8
	
	moveq.l	#0,d0					*  4
	move.b	d1,d0					*  4
	lea	FtblSRArl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	asr.b	#1,d1					*  8
	move.w	d1,(regIXY)				*  8
	
	FETCH						* 
.endif
	endopcode DDCB2D



*--- SRA (Ixy+d)				SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB2E
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	moveq.l	#0,d0					*  4
	move.b	(a0),d0					*  8
	
	move.w	d0,regF					*  4
	
	asr.b	#1,d0					*  8
	move.b	d0,(a0)					*  8
	
	lea	FtblSRArl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB2E



*---
	opcode DDCB2F
	IllegalOP	4
	endopcode DDCB2F



*---
	opcode DDCB30
	IllegalOP	4
	endopcode DDCB30



*---
	opcode DDCB31
	IllegalOP	4
	endopcode DDCB31



*---
	opcode DDCB32
	IllegalOP	4
	endopcode DDCB32



*---
	opcode DDCB33
	IllegalOP	4
	endopcode DDCB33



*---
	opcode DDCB34
	IllegalOP	4
	endopcode DDCB34



*---
	opcode DDCB35
	IllegalOP	4
	endopcode DDCB35



*---
	opcode DDCB36
	IllegalOP	4
	endopcode DDCB36



*---
	opcode DDCB37
	IllegalOP	4
	endopcode DDCB37



*---
	opcode DDCB38
	IllegalOP	4
	endopcode DDCB38



*---
	opcode DDCB39
	IllegalOP	4
	endopcode DDCB39



*---
	opcode DDCB3A
	IllegalOP	4
	endopcode DDCB3A



*---
	opcode DDCB3B
	IllegalOP	4
	endopcode DDCB3B



*--- SRL IxyH	**				SZ H VNC
*						** 0 P0*
	opcode DDCB3C
	IllegalOP	4
	
	
.if 0
	moveq.l	#0,d0					*  4
	move.b	(regIXY),d0				*  8
	lea	FtblSRLrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	lsr.b	#1,d0					*  8
	move.b	d0,(regIXY)				*  8
	
	FETCH						* 
.endif
	endopcode DDCB3C



*--- SRL IxyL	**				SZ H VNC
*						** 0 P0*
	opcode DDCB3D
	IllegalOP	4
	
	
.if 0
	moveq.l	#0,d0					*  4
	move.b	1(regIXY),d0				* 12
	lea	FtblSRLrl(regFtbl),a0			*  8
	move.b	(a0,d0.w),regF				* 14
	
	lsr.b	#1,d0					*  8
	move.b	d0,1(regIXY)				* 12
	
	FETCH						* 
.endif
	endopcode DDCB3D



*--- SRL (Ixy+d)				SZ H VNC
*						** 0 P0*
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB3E
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	moveq.l	#0,d0					*  4
	move.b	(a0),d0					*  8
	
	move.w	d0,regF					*  4
	
	lsr.b	#1,d0					*  8
	move.b	d0,(a0)					* 12
	
	lea	FtblSRLrl(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode DDCB3E



*---
	opcode DDCB3F
	IllegalOP	4
	endopcode DDCB3F



*---
	opcode DDCB40
	IllegalOP	4
	endopcode DDCB40



*---
	opcode DDCB41
	IllegalOP	4
	endopcode DDCB41



*---
	opcode DDCB42
	IllegalOP	4
	endopcode DDCB42



*---
	opcode DDCB43
	IllegalOP	4
	endopcode DDCB43



*--- BIT 0,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB44
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#0,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB44



*--- BIT 0,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB45
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#0,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB45



*--- BIT 0,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB46
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#0,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB46



*---
	opcode DDCB47
	IllegalOP	4
	endopcode DDCB47



*---
	opcode DDCB48
	IllegalOP	4
	endopcode DDCB48



*---
	opcode DDCB49
	IllegalOP	4
	endopcode DDCB49



*---
	opcode DDCB4A
	IllegalOP	4
	endopcode DDCB4A



*---
	opcode DDCB4B
	IllegalOP	4
	endopcode DDCB4B



*--- BIT 1,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB4C
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#1,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB4C



*--- BIT 1,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB4D
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#1,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB4D



*--- BIT 1,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB4E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#1,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB4E



*---
	opcode DDCB4F
	IllegalOP	4
	endopcode DDCB4F



*---
	opcode DDCB50
	IllegalOP	4
	endopcode DDCB50



*---
	opcode DDCB51
	IllegalOP	4
	endopcode DDCB51



*---
	opcode DDCB52
	IllegalOP	4
	endopcode DDCB52



*---
	opcode DDCB53
	IllegalOP	4
	endopcode DDCB53



*--- BIT 2,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB54
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#2,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB54



*--- BIT 2,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB55
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#2,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB55



*--- BIT 2,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB56
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#2,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB56



*---
	opcode DDCB57
	IllegalOP	4
	endopcode DDCB57



*---
	opcode DDCB58
	IllegalOP	4
	endopcode DDCB58



*---
	opcode DDCB59
	IllegalOP	4
	endopcode DDCB59



*---
	opcode DDCB5A
	IllegalOP	4
	endopcode DDCB5A



*---
	opcode DDCB5B
	IllegalOP	4
	endopcode DDCB5B



*--- BIT 3,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB5C
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#3,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB5C



*--- BIT 3,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB5D
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#3,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB5D



*--- BIT 3,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB5E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#3,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB5E



*---
	opcode DDCB5F
	IllegalOP	4
	endopcode DDCB5F



*---
	opcode DDCB60
	IllegalOP	4
	endopcode DDCB60



*---
	opcode DDCB61
	IllegalOP	4
	endopcode DDCB61



*---
	opcode DDCB62
	IllegalOP	4
	endopcode DDCB62



*---
	opcode DDCB63
	IllegalOP	4
	endopcode DDCB63



*--- BIT 4,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB64
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#4,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB64



*--- BIT 4,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB65
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#4,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB65



*--- BIT 4,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB66
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#4,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB66



*---
	opcode DDCB67
	IllegalOP	4
	endopcode DDCB67



*---
	opcode DDCB68
	IllegalOP	4
	endopcode DDCB68



*---
	opcode DDCB69
	IllegalOP	4
	endopcode DDCB69



*---
	opcode DDCB6A
	IllegalOP	4
	endopcode DDCB6A



*---
	opcode DDCB6B
	IllegalOP	4
	endopcode DDCB6B



*--- BIT 5,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB6C
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#5,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB6C



*--- BIT 5,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB6D
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#5,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB6D



*--- BIT 5,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB6E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#5,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB6E



*---
	opcode DDCB6F
	IllegalOP	4
	endopcode DDCB6F



*---
	opcode DDCB70
	IllegalOP	4
	endopcode DDCB70



*---
	opcode DDCB71
	IllegalOP	4
	endopcode DDCB71



*---
	opcode DDCB72
	IllegalOP	4
	endopcode DDCB72



*---
	opcode DDCB73
	IllegalOP	4
	endopcode DDCB73



*--- BIT 6,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB74
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#6,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB74



*--- BIT 6,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB75
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#6,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB75



*--- BIT 6,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB76
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#6,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB76



*---
	opcode DDCB77
	IllegalOP	4
	endopcode DDCB77



*---
	opcode DDCB78
	IllegalOP	4
	endopcode DDCB78



*---
	opcode DDCB79
	IllegalOP	4
	endopcode DDCB79



*---
	opcode DDCB7A
	IllegalOP	4
	endopcode DDCB7A



*---
	opcode DDCB7B
	IllegalOP	4
	endopcode DDCB7B



*--- BIT 7,IxyH	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB7C
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#7,(regIXY)				* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB7C



*--- BIT 7,IxyL	**				SZ H VNC
*						x* 1 x0･
	opcode DDCB7D
	IllegalOP	4
	
	
.if 0
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	btst.b	#7,1(regIXY)				* 16
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
.endif
	endopcode DDCB7D



*--- BIT 7,(Ixy+d)				SZ H VNC
*						x* 1 x0･
	opcode DDCB7E
	Fset	___H____				*  8
	Fclr	_Z____N_				*  8
	
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	btst.b	#7,(a0)					* 12
	
	bne.s	@f					* 10/ 8 (T/F)
	Fset	_Z______				*     8
@@:
	FETCH						* 
	endopcode DDCB7E



*---
	opcode DDCB7F
	IllegalOP	4
	endopcode DDCB7F



*---
	opcode DDCB80
	IllegalOP	4
	endopcode DDCB80



*---
	opcode DDCB81
	IllegalOP	4
	endopcode DDCB81



*---
	opcode DDCB82
	IllegalOP	4
	endopcode DDCB82



*---
	opcode DDCB83
	IllegalOP	4
	endopcode DDCB83



*--- RES 0,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB84
	IllegalOP	4
	
	
	bclr.b	#0,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCB84



*--- RES 0,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB85
	IllegalOP	4
	
	
	bclr.b	#0,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCB85



*--- RES 0,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB86
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#0,(a0)					* 16
	
	FETCH						* 
	endopcode DDCB86



*---
	opcode DDCB87
	IllegalOP	4
	endopcode DDCB87



*---
	opcode DDCB88
	IllegalOP	4
	endopcode DDCB88



*---
	opcode DDCB89
	IllegalOP	4
	endopcode DDCB89



*---
	opcode DDCB8A
	IllegalOP	4
	endopcode DDCB8A



*---
	opcode DDCB8B
	IllegalOP	4
	endopcode DDCB8B



*--- RES 1,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB8C
	IllegalOP	4
	
	
	bclr.b	#1,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCB8C



*--- RES 1,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB8D
	IllegalOP	4
	
	
	bclr.b	#1,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCB8D



*--- RES 1,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB8E
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#1,(a0)					* 16
	
	FETCH						* 
	endopcode DDCB8E



*---
	opcode DDCB8F
	IllegalOP	4
	endopcode DDCB8F



*---
	opcode DDCB90
	IllegalOP	4
	endopcode DDCB90



*---
	opcode DDCB91
	IllegalOP	4
	endopcode DDCB91



*---
	opcode DDCB92
	IllegalOP	4
	endopcode DDCB92



*---
	opcode DDCB93
	IllegalOP	4
	endopcode DDCB93



*--- RES 2,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB94
	IllegalOP	4
	
	
	bclr.b	#2,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCB94



*--- RES 2,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB95
	IllegalOP	4
	
	
	bclr.b	#2,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCB95



*--- RES 2,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB96
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#2,(a0)					* 16
	
	FETCH						* 
	endopcode DDCB96



*---
	opcode DDCB97
	IllegalOP	4
	endopcode DDCB97



*---
	opcode DDCB98
	IllegalOP	4
	endopcode DDCB98



*---
	opcode DDCB99
	IllegalOP	4
	endopcode DDCB99



*---
	opcode DDCB9A
	IllegalOP	4
	endopcode DDCB9A



*---
	opcode DDCB9B
	IllegalOP	4
	endopcode DDCB9B



*--- RES 3,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB9C
	IllegalOP	4
	
	
	bclr.b	#3,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCB9C



*--- RES 3,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCB9D
	IllegalOP	4
	
	
	bclr.b	#3,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCB9D



*--- RES 3,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCB9E
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#3,(a0)					* 16
	
	FETCH						* 
	endopcode DDCB9E



*---
	opcode DDCB9F
	IllegalOP	4
	endopcode DDCB9F



*---
	opcode DDCBA0
	IllegalOP	4
	endopcode DDCBA0



*---
	opcode DDCBA1
	IllegalOP	4
	endopcode DDCBA1



*---
	opcode DDCBA2
	IllegalOP	4
	endopcode DDCBA2



*---
	opcode DDCBA3
	IllegalOP	4
	endopcode DDCBA3



*--- RES 4,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBA4
	IllegalOP	4
	
	
	bclr.b	#4,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBA4



*--- RES 4,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBA5
	IllegalOP	4
	
	
	bclr.b	#4,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBA5



*--- RES 4,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBA6
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#4,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBA6



*---
	opcode DDCBA7
	IllegalOP	4
	endopcode DDCBA7



*---
	opcode DDCBA8
	IllegalOP	4
	endopcode DDCBA8



*---
	opcode DDCBA9
	IllegalOP	4
	endopcode DDCBA9



*---
	opcode DDCBAA
	IllegalOP	4
	endopcode DDCBAA



*---
	opcode DDCBAB
	IllegalOP	4
	endopcode DDCBAB



*--- RES 5,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBAC
	IllegalOP	4
	
	
	bclr.b	#5,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBAC



*--- RES 5,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBAD
	IllegalOP	4
	
	
	bclr.b	#5,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBAD



*--- RES 5,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBAE
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#5,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBAE



*---
	opcode DDCBAF
	IllegalOP	4
	endopcode DDCBAF



*---
	opcode DDCBB0
	IllegalOP	4
	endopcode DDCBB0



*---
	opcode DDCBB1
	IllegalOP	4
	endopcode DDCBB1



*---
	opcode DDCBB2
	IllegalOP	4
	endopcode DDCBB2



*---
	opcode DDCBB3
	IllegalOP	4
	endopcode DDCBB3



*--- RES 6,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBB4
	IllegalOP	4
	
	
	bclr.b	#6,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBB4



*--- RES 6,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBB5
	IllegalOP	4
	
	
	bclr.b	#6,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBB5



*--- RES 6,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBB6
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#6,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBB6



*---
	opcode DDCBB7
	IllegalOP	4
	endopcode DDCBB7



*---
	opcode DDCBB8
	IllegalOP	4
	endopcode DDCBB8



*---
	opcode DDCBB9
	IllegalOP	4
	endopcode DDCBB9



*---
	opcode DDCBBA
	IllegalOP	4
	endopcode DDCBBA



*---
	opcode DDCBBB
	IllegalOP	4
	endopcode DDCBBB



*--- RES 7,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBBC
	IllegalOP	4
	
	
	bclr.b	#7,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBBC



*--- RES 7,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBBD
	IllegalOP	4
	
	
	bclr.b	#7,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBBD



*--- RES 7,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBBE
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bclr.b	#7,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBBE



*---
	opcode DDCBBF
	IllegalOP	4
	endopcode DDCBBF



*---
	opcode DDCBC0
	IllegalOP	4
	endopcode DDCBC0



*---
	opcode DDCBC1
	IllegalOP	4
	endopcode DDCBC1



*---
	opcode DDCBC2
	IllegalOP	4
	endopcode DDCBC2



*---
	opcode DDCBC3
	IllegalOP	4
	endopcode DDCBC3



*--- SET 0,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBC4
	IllegalOP	4
	
	
	bset.b	#0,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBC4



*--- SET 0,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBC5
	IllegalOP	4
	
	
	bset.b	#0,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBC5



*--- SET 0,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBC6
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#0,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBC6



*---
	opcode DDCBC7
	IllegalOP	4
	endopcode DDCBC7



*---
	opcode DDCBC8
	IllegalOP	4
	endopcode DDCBC8



*---
	opcode DDCBC9
	IllegalOP	4
	endopcode DDCBC9



*---
	opcode DDCBCA
	IllegalOP	4
	endopcode DDCBCA



*---
	opcode DDCBCB
	IllegalOP	4
	endopcode DDCBCB



*--- SET 1,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBCC
	IllegalOP	4
	
	
	bset.b	#1,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBCC



*--- SET 1,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBCD
	IllegalOP	4
	
	
	bset.b	#1,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBCD



*--- SET 1,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBCE
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#1,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBCE



*---
	opcode DDCBCF
	IllegalOP	4
	endopcode DDCBCF



*---
	opcode DDCBD0
	IllegalOP	4
	endopcode DDCBD0



*---
	opcode DDCBD1
	IllegalOP	4
	endopcode DDCBD1



*---
	opcode DDCBD2
	IllegalOP	4
	endopcode DDCBD2



*---
	opcode DDCBD3
	IllegalOP	4
	endopcode DDCBD3



*--- SET 2,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBD4
	IllegalOP	4
	
	
	bset.b	#2,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBD4



*--- SET 2,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBD5
	IllegalOP	4
	
	
	bset.b	#2,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBD5



*--- SET 2,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBD6
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#2,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBD6



*---
	opcode DDCBD7
	IllegalOP	4
	endopcode DDCBD7



*---
	opcode DDCBD8
	IllegalOP	4
	endopcode DDCBD8



*---
	opcode DDCBD9
	IllegalOP	4
	endopcode DDCBD9



*---
	opcode DDCBDA
	IllegalOP	4
	endopcode DDCBDA



*---
	opcode DDCBDB
	IllegalOP	4
	endopcode DDCBDB



*--- SET 3,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBDC
	IllegalOP	4
	
	
	bset.b	#3,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBDC



*--- SET 3,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBDD
	IllegalOP	4
	
	
	bset.b	#3,(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBDD



*--- SET 3,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBDE
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#3,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBDE



*---
	opcode DDCBDF
	IllegalOP	4
	endopcode DDCBDF



*---
	opcode DDCBE0
	IllegalOP	4
	endopcode DDCBE0



*---
	opcode DDCBE1
	IllegalOP	4
	endopcode DDCBE1



*---
	opcode DDCBE2
	IllegalOP	4
	endopcode DDCBE2



*---
	opcode DDCBE3
	IllegalOP	4
	endopcode DDCBE3



*--- SET 4,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBE4
	IllegalOP	4
	
	
	bset.b	#4,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBE4



*--- SET 4,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBE5
	IllegalOP	4
	
	
	bset.b	#4,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBE5



*--- SET 4,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBE6
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#4,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBE6



*---
	opcode DDCBE7
	IllegalOP	4
	endopcode DDCBE7



*---
	opcode DDCBE8
	IllegalOP	4
	endopcode DDCBE8



*---
	opcode DDCBE9
	IllegalOP	4
	endopcode DDCBE9



*---
	opcode DDCBEA
	IllegalOP	4
	endopcode DDCBEA



*---
	opcode DDCBEB
	IllegalOP	4
	endopcode DDCBEB



*--- SET 5,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBEC
	IllegalOP	4
	
	
	bset.b	#5,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBEC



*--- SET 5,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBED
	IllegalOP	4
	
	
	bset.b	#5,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBED



*--- SET 5,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBEE
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#5,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBEE



*---
	opcode DDCBEF
	IllegalOP	4
	endopcode DDCBEF



*---
	opcode DDCBF0
	IllegalOP	4
	endopcode DDCBF0



*---
	opcode DDCBF1
	IllegalOP	4
	endopcode DDCBF1



*---
	opcode DDCBF2
	IllegalOP	4
	endopcode DDCBF2



*---
	opcode DDCBF3
	IllegalOP	4
	endopcode DDCBF3



*--- SET 6,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBF4
	IllegalOP	4
	
	
	bset.b	#6,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBF4



*--- SET 6,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBF5
	IllegalOP	4
	
	
	bset.b	#6,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBF5



*--- SET 6,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBF6
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	
	movea.l	regAdr,a0				*  4
	
	bset.b	#6,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBF6



*---
	opcode DDCBF7
	IllegalOP	4
	endopcode DDCBF7



*---
	opcode DDCBF8
	IllegalOP	4
	endopcode DDCBF8



*---
	opcode DDCBF9
	IllegalOP	4
	endopcode DDCBF9



*---
	opcode DDCBFA
	IllegalOP	4
	endopcode DDCBFA



*---
	opcode DDCBFB
	IllegalOP	4
	endopcode DDCBFB



*--- SET 7,IxyH	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBFC
	IllegalOP	4
	
	
	bset.b	#7,(regIXY)				* 16
	
	FETCH						* 
	endopcode DDCBFC



*--- SET 7,IxyL	**				SZ H VNC
*						･･ ･ ･･･
	opcode DDCBFD
	IllegalOP	4
	
	
	bset.b	#7,1(regIXY)				* 20
	
	FETCH						* 
	endopcode DDCBFD



*--- SET 7,(Ixy+d)				SZ H VNC
*						･･ ･ ･･･
*	!! ROM領域への書き込みのチェックはしない
	opcode DDCBFE
	ext.w	regAdr					*  4
	add.w	(regIXY),regAdr				*  8
	movea.l	regAdr,a0				*  4
	
	bset.b	#7,(a0)					* 16
	
	FETCH						* 
	endopcode DDCBFE



*---
	opcode DDCBFF
	IllegalOP	4
	endopcode DDCBFF



*----------------------------------------------------------------
* ED xx



*--- IN0 B,(n)	** HD64180			SZ H VNC
*							
	opcode ED00
	IllegalOP	2
	endopcode ED00



*--- OUT0 (n),B	** HD64180			SZ H VNC
*							
	opcode ED01
	IllegalOP	2
	endopcode ED01



*---
	opcode ED02
	IllegalOP	2
	endopcode ED02



*---
	opcode ED03
	IllegalOP	2
	endopcode ED03



*--- TST B	** HD64180			SZ H VNC
*							
	opcode ED04
	IllegalOP	2
	endopcode ED04



*---
	opcode ED05
	IllegalOP	2
	endopcode ED05



*---
	opcode ED06
	IllegalOP	2
	endopcode ED06



*---
	opcode ED07
	IllegalOP	2
	endopcode ED07



*--- IN0 C,(n)	** HD64180			SZ H VNC
*							
	opcode ED08
	IllegalOP	2
	endopcode ED08



*--- OUT0 (n),C	** HD64180			SZ H VNC
*							
	opcode ED09
	IllegalOP	2
	endopcode ED09



*---
	opcode ED0A
	IllegalOP	2
	endopcode ED0A



*---
	opcode ED0B
	IllegalOP	2
	endopcode ED0B



*--- TST C	** HD64180			SZ H VNC
*						
	opcode ED0C
	IllegalOP	2
	endopcode ED0C



*---
	opcode ED0D
	IllegalOP	2
	endopcode ED0D



*---
	opcode ED0E
	IllegalOP	2
	endopcode ED0E



*---
	opcode ED0F
	IllegalOP	2
	endopcode ED0F



*--- IN0 D,(n)	** HD64180			SZ H VNC
*						
	opcode ED10
	IllegalOP	2
	endopcode ED10



*--- OUT0 (n),D	** HD64180			SZ H VNC
*						
	opcode ED11
	IllegalOP	2
	endopcode ED11



*---
	opcode ED12
	IllegalOP	2
	endopcode ED12



*---
	opcode ED13
	IllegalOP	2
	endopcode ED13



*--- TST D	** HD64180			SZ H VNC
*						
	opcode ED14
	IllegalOP	2
	endopcode ED14



*---
	opcode ED15
	IllegalOP	2
	endopcode ED15



*---
	opcode ED16
	IllegalOP	2
	endopcode ED16



*---
	opcode ED17
	IllegalOP	2
	endopcode ED17



*--- IN0 E,(n)	** HD64180			SZ H VNC
*						
	opcode ED18
	IllegalOP	2
	endopcode ED18



*--- OUT0 (n),E	** HD64180			SZ H VNC
*						
	opcode ED19
	IllegalOP	2
	endopcode ED19



*---
	opcode ED1A
	IllegalOP	2
	endopcode ED1A



*---
	opcode ED1B
	IllegalOP	2
	endopcode ED1B



*--- TST E	** HD64180			SZ H VNC
*						
	opcode ED1C
	IllegalOP	2
	endopcode ED1C



*---
	opcode ED1D
	IllegalOP	2
	endopcode ED1D



*---
	opcode ED1E
	IllegalOP	2
	endopcode ED1E



*---
	opcode ED1F
	IllegalOP	2
	endopcode ED1F



*--- IN0 H,(n)	** HD64180			SZ H VNC
*						
	opcode ED20
	IllegalOP	2
	endopcode ED20



*--- OUT0 (n),H	** HD64180			SZ H VNC
*						
	opcode ED21
	IllegalOP	2
	endopcode ED21



*---
	opcode ED22
	IllegalOP	2
	endopcode ED22



*---
	opcode ED23
	IllegalOP	2
	endopcode ED23



*--- TST H	** HD64180			SZ H VNC
*						
	opcode ED24
	IllegalOP	2
	endopcode ED24



*---
	opcode ED25
	IllegalOP	2
	endopcode ED25



*---
	opcode ED26
	IllegalOP	2
	endopcode ED26



*---
	opcode ED27
	IllegalOP	2
	endopcode ED27



*--- IN0 L,(n)	** HD64180			SZ H VNC
*						
	opcode ED28
	IllegalOP	2
	endopcode ED28



*--- OUT0 (n),L	** HD64180			SZ H VNC
*						
	opcode ED29
	IllegalOP	2
	endopcode ED29



*---
	opcode ED2A
	IllegalOP	2
	endopcode ED2A



*---
	opcode ED2B
	IllegalOP	2
	endopcode ED2B



*--- TST L	** HD64180			SZ H VNC
*						
	opcode ED2C
	IllegalOP	2
	endopcode ED2C



*---
	opcode ED2D
	IllegalOP	2
	endopcode ED2D



*---
	opcode ED2E
	IllegalOP	2
	endopcode ED2E



*---
	opcode ED2F
	IllegalOP	2
	endopcode ED2F



*---
	opcode ED30
	IllegalOP	2
	endopcode ED30



*---
	opcode ED31
	IllegalOP	2
	endopcode ED31



*---
	opcode ED32
	IllegalOP	2
	endopcode ED32



*---
	opcode ED33
	IllegalOP	2
	endopcode ED33



*--- TST (HL)	** HD64180			SZ H VNC
*						
	opcode ED34
	IllegalOP	2
	endopcode ED34



*---
	opcode ED35
	IllegalOP	2
	endopcode ED35



*---
	opcode ED36
	IllegalOP	2
	endopcode ED36



*---
	opcode ED37
	IllegalOP	2
	endopcode ED37



*--- IN0 A,(n)	** HD64180			SZ H VNC
*						
	opcode ED38
	IllegalOP	2
	endopcode ED38



*--- OUT0 (n),A	** HD64180			SZ H VNC
*						
	opcode ED39
	IllegalOP	2
	endopcode ED39



*---
	opcode ED3A
	IllegalOP	2
	endopcode ED3A



*---
	opcode ED3B
	IllegalOP	2
	endopcode ED3B



*--- TST A	** HD64180			SZ H VNC
*						
	opcode ED3C
	IllegalOP	2
	endopcode ED3C



*---
	opcode ED3D
	IllegalOP	2
	endopcode ED3D



*---
	opcode ED3E
	IllegalOP	2
	endopcode ED3E



*---
	opcode ED3F
	IllegalOP	2
	endopcode ED3F



*--- IN B,(C)					SZ H VNC
*						** 0 P0･
	opcode ED40
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.w	regBC,-(regSwap)			*  8
	move.b	d1,(regSwap)				*  8
	move.w	(regSwap)+,regBC			*  8
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED40



*--- OUT (C),B					SZ H VNC
*						･･ ･ ･･･
	opcode ED41
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,d0				*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED41



*--- SBC HL,BC					SZ H VNC
*						** x V1*
	opcode ED42
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	subx.w	regBC,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblSBC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED42



*--- LD (mn),BC					SZ H VNC
*						･･ ･ ･･･
*	!! ページの境界をまたいだ書き込みのチェックはしていない
*	!! 上位バイトの書き込みのみチェック
	opcode ED43
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	
	CHECK_ROM regAdr,d0				* 
	
	movea.l	regAdr,a0				*  4
	
	move.b	regBC,(a0)+				*  8
	move.w	regBC,-(regSwap)			*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						*  
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_mn_BC(regFtbl)			* 
	endopcode ED43



*--- NEG					SZ H VNC
*						** * V1*
	opcode ED44
	move.b	regA,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8	[src:dst] (dst-src)
	
	lea	FtblSUB8,a1				* 12
	move.b	(a1,d1.l),regF				* 14
	
	neg.b	regA					*  4
	
	FETCH						* 
	endopcode ED44



*--- RETN					SZ H VNC
*						･･ ･ ･･･
	opcode ED45
	NotImpliment	2
	endopcode ED45



*--- IM 0					SZ H VNC
*						･･ ･ ･･･
	opcode ED46
	NotImpliment	2
	endopcode ED46



*--- LD I,A					SZ H VNC
*						･･ ･ ･･･
	opcode ED47
	move.w	regA,workI(regFtbl)			* 12
	
	FETCH						* 
	endopcode ED47



*--- IN C,(C)					SZ H VNC
*						** 0 P0･
	opcode ED48
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.b	d1,regBC				*  4
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED48



*--- OUT (C),C					SZ H VNC
*						･･ ･ ･･･
	opcode ED49
	move.b	regBC,d0				*  4
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED49



*--- ADC HL,BC					SZ H VNC
*						** x V0*
	opcode ED4A
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	addx.w	regBC,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblADC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED4A



*--- LD BC,(mn)					SZ H VNC
*						･･ ･ ･･･
	opcode ED4B
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	movea.l	regAdr,a0				*  4
	
	movep.w	1(a0),regBC				* 16
	move.b	(a0),regBC				*  8
	
	FETCH						*  
	endopcode ED4B



*--- MLT BC	** HD64180			SZ H VNC
*							
	opcode ED4C
	IllegalOP	2
	endopcode ED4C



*--- RETI					SZ H VNC
*						･･ ･ ･･･
	opcode ED4D
	movep.w	1(regSP),regAdr				* 16
	move.b	(regSP),regAdr				*  8
	addq.w	#2,regSP				*  8
	movea.l	regAdr,regPC				*  4
	
	FETCH						* 
	endopcode ED4D



*---
	opcode ED4E
	IllegalOP	2
	endopcode ED4E



*--- LD R,A					SZ H VNC
*						･･ ･ ･･･
	opcode ED4F
	FETCH						*  
	endopcode ED4F



*--- IN D,(C)					SZ H VNC
*						** 0 P0･
	opcode ED50
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.w	regDE,-(regSwap)			*  8
	move.b	d1,(regSwap)				*  8
	move.w	(regSwap)+,regDE			*  8
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED50



*--- OUT (C),D					SZ H VNC
*						･･ ･ ･･･
	opcode ED51
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,d0				*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED51



*--- SBC HL,DE					SZ H VNC
*						** x V1*
	opcode ED52
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	subx.w	regDE,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblSBC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED52



*--- LD (mn),DE					SZ H VNC
*						･･ ･ ･･･
*	!! ページの境界をまたいだ書き込みのチェックはしていない
*	!! 上位バイトの書き込みのみチェック
	opcode ED53
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	
	CHECK_ROM regAdr,d0				* 
	
	movea.l	regAdr,a0				*  4
	
	move.b	regDE,(a0)+				*  8
	move.w	regDE,-(regSwap)			*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_mn_DE(regFtbl)			* 
	endopcode ED53



*---
	opcode ED54
	IllegalOP	2
	endopcode ED54



*---
	opcode ED55
	IllegalOP	2
	endopcode ED55



*--- IM 1					SZ H VNC
*						･･ ･ ･･･
	opcode ED56
	FETCH						* 
	endopcode ED56



*--- LD A,I					SZ H VNC
*						** 0 I0･
*	Z80_DIEI : =0 disable  !=0 enable
*
	opcode ED57
	move.w	workI(regFtbl),regA			* 12
	
	ext.w	regF					*  4
	move.b	regA,regF				*  4
	lea	FtblIFF(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	or.b	Z80_DIEI(regFtbl),regF			* 12
	
	FETCH						* 
	endopcode ED57



*--- IN E,(C)					SZ H VNC
*						** 0 P0･
	opcode ED58
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.b	d1,regDE				*  4
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED58



*--- OUT (C),E					SZ H VNC
*						･･ ･ ･･･
	opcode ED59
	move.b	regDE,d0				*  4
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED59



*--- ADC HL,DE					SZ H VNC
*						** x V0*
	opcode ED5A
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	addx.w	regDE,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblADC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED5A



*--- LD DE,(mn)					SZ H VNC
*						･･ ･ ･･･
	opcode ED5B
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	movea.l	regAdr,a0				*  4
	
	movep.w	1(a0),regDE				* 16
	move.b	(a0),regDE				*  8
	
	FETCH						*  
	endopcode ED5B



*--- MLT DE	** HD64180			SZ H VNC
*							
	opcode ED5C
	IllegalOP	2
	endopcode ED5C



*---
	opcode ED5D
	IllegalOP	2
	endopcode ED5D



*--- IM 2					SZ H VNC
*						･･ ･ ･･･
	opcode ED5E
	NotImpliment	2
	endopcode ED5E



*--- LD A,R					SZ H VNC
*						** 0 I0･
	opcode ED5F
*	trap	#9
	
	moveq.l	#$7f,d0					*  4	IOCS _ONTIME
	trap	#15					* 34+?
	
	move.b	d0,regA					*  4
	andi.b	#%0111_1111,regA			*  8
	
	
	ext.w	regF					*  4
	move.b	regA,regF				*  4
	lea	FtblIFF(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	or.b	Z80_DIEI(regFtbl),regF			* 12
	
	moveq.l	#0,d1					*  4
	
	FETCH						* 
	endopcode ED5F



*--- IN H,(C)					SZ H VNC
*						** 0 P0･
	opcode ED60
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.w	regHL,-(regSwap)			*  8
	move.b	d1,(regSwap)				*  8
	move.w	(regSwap)+,regHL			*  8
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED60



*--- OUT (C),H					SZ H VNC
*						･･ ･ ･･･
	opcode ED61
	move.w	regHL,-(regSwap)			*  8
	move.b	(regSwap)+,d0				*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED61



*--- SBC HL,HL					SZ H VNC
*						** x V1*
	opcode ED62
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	subx.w	regHL,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblSBC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED62



*---
	opcode ED63
	IllegalOP	2
	endopcode ED63



*--- TST n	** HD64180			SZ H VNC
*						
	opcode ED64
	IllegalOP	2
	endopcode ED64



*---
	opcode ED65
	IllegalOP	2
	endopcode ED65



*---
	opcode ED66
	IllegalOP	2
	endopcode ED66



*--- RRD					SZ H VNC
*						** 0 P0･
*	!! ROM領域への書き込みのチェックはしない
	opcode ED67
.if 0	* (HL)の状態でﾌﾗｸﾞ設定
	movea.l	regHL,a0				*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(a0),d0					*  8
	ror.w	#4,d0					* 14
	move.b	d0,(a0)					*  8
	
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,regA				*  8
	ror.b	#4,regA					* 14
	
	lea	FtblRRLD(regFtbl),a0			*  8
	ext.w	regF					*  4
	move.b	d0,regF					*  4
	move.b	(a0,regF.w),regF			* 14 
.else	
	* A の状態でﾌﾗｸﾞ設定
	movea.l	regHL,a0				*  4
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(a0),d0					*  8
	
	; aaaa aaaa nnnn nnnn
	; hhhh llll hhhh llll
	
	ror.w	#4,d0					* 14
	
	; nnnn aaaa aaaa nnnn
	; llll hhhh llll hhhh
	
	move.b	d0,(a0)					*  8
	
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,regA				*  8
	ror.b	#4,regA					* 14
	
	lea	FtblRRLD(regFtbl),a0			*  8
	ext.w	regF					*  4
	move.b	regA,regF				*  4
	move.b	(a0,regF.w),regF			* 14 
.endif
	
	FETCH						* 
	endopcode ED67



*--- IN L,(C)					SZ H VNC
*						** 0 P0･
	opcode ED68
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.b	d1,regHL				*  4
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED68



*--- OUT (C),L					SZ H VNC
*						･･ ･ ･･･
	opcode ED69
	move.b	regHL,d0				*  4
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED69



*--- ADC HL,HL					SZ H VNC
*						** x V0*
	opcode ED6A
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	addx.w	regHL,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblADC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED6A



*---
	opcode ED6B
	IllegalOP	2
	endopcode ED6B



*--- MLT HL	** HD64180			SZ H VNC
*						
	opcode ED6C
	IllegalOP	2
	endopcode ED6C



*---
	opcode ED6D
	IllegalOP	2
	endopcode ED6D



*---
	opcode ED6E
	IllegalOP	2
	endopcode ED6E



*--- RLD					SZ H VNC
*						** 0 P0･
*	!! ROM領域への書き込みのチェックはしない
	opcode ED6F
.if 0	* (HL)の状態でﾌﾗｸﾞ設定
	movea.l	regHL,a0				*  4
	rol.b	#4,regA					* 14
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(a0),d0					*  8
	rol.w	#4,d0					* 14
	
	move.b	d0,(a0)					*  8
	
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,regA				*  8
	
	lea	FtblRRLD(regFtbl),a0			*  8
	ext.w	regF					*  4
	move.b	d0,regF					*  4
	move.b	(a0,regF.w),regF			* 14
.else
	* A の状態でﾌﾗｸﾞ設定
	movea.l	regHL,a0				*  4
	rol.b	#4,regA					* 14
	
	; aaaa aaaa
	; llll hhhh
	
	move.b	regA,-(regSwap)				*  8
	move.w	(regSwap)+,d0				*  8
	move.b	(a0),d0					*  8
	
	; aaaa aaaa nnnn nnnn
	; llll hhhh hhhh llll
	
	rol.w	#4,d0					* 14
	
	; aaaa nnnn nnnn aaaa
	; hhhh hhhh llll llll
	
	move.b	d0,(a0)					*  8
	
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,regA				*  8
	
	lea	FtblRRLD(regFtbl),a0			*  8
	ext.w	regF					*  4
	move.b	regA,regF				*  4
	move.b	(a0,regF.w),regF			* 14
.endif
	
	FETCH						* 
	endopcode ED6F



*--- IN F,(C)	**
	opcode ED70
	IllegalOP	2
	endopcode ED70



*--- OUT (C),0	**
	opcode ED71
	clr.b	d0					*  4
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED71


*--- SBC HL,SP					SZ H VNC
*						** x V1*
	opcode ED72
	move.w	regSP,d0				*  4
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	subx.w	d0,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblSBC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED72



*--- LD (mn),SP					SZ H VNC
*						･･ ･ ･･･
*	!! ページの境界をまたいだ書き込みのチェックはしていない
*	!! 上位バイトの書き込みのみチェック
	opcode ED73
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	
	CHECK_ROM regAdr,d0				* 
	
	movea.l	regAdr,a0				*  4
	
	move.w	regSP,d0				*  4
	move.b	d0,(a0)+				*  8
	move.w	d0,-(regSwap)				*  8
	move.b	(regSwap)+,(a0)				* 12
	
	FETCH						* 
	
	;---
	CHECK_ROM_SKIP_LABEL				* 
	
	jmp	RomWrite_mn_SP(regFtbl)			* 
	endopcode ED73



*--- TSTIO (n)	** HD64180			SZ H VNC
*						
	opcode ED74
	IllegalOP	2
	endopcode ED74



*---
	opcode ED75
	IllegalOP	2
	endopcode ED75



*--- SLP	** HD64180			SZ H VNC
*						
	opcode ED76
	IllegalOP	2
	endopcode ED76



*---
	opcode ED77
	IllegalOP	2
	endopcode ED77



*--- IN A,(C)					SZ H VNC
*						** 0 P0･
	opcode ED78
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	move.b	d1,regA					*  4
	
	ext.w	regF					*  4
	move.b	d1,regF					*  4
	lea	FtblIN(regFtbl),a0			*  8
	move.b	(a0,regF.w),regF			* 14
	
	FETCH						* 
	endopcode ED78



*--- OUT (C),A					SZ H VNC
*						･･ ･ ･･･
	opcode ED79
	move.b	regA,d0					*  4
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
	FETCH						* 
	endopcode ED79



*--- ADC HL,SP					SZ H VNC
*						** x V1*
	opcode ED7A
	move.w	regSP,d0				*  4
	add.b	regF,regF				*  4
	;-- set Z flag
	moveq.l	#0,d1					*  4
	addx.w	d0,regHL				*  4
	
	move.w	sr,d0					*  6
	ext.w	d0					*  4
	lea	FtblADC16(regFtbl),a1			*  8
	move.b	(a1,d0.w),regF				* 14
	
	FETCH						* 
	endopcode ED7A



*--- LD SP,(mn)					SZ H VNC
*						･･ ･ ･･･
	opcode ED7B
	movep.w	1(regPC),regAdr				* 16
	move.b	(regPC),regAdr				*  8
	addq.w	#2,regPC				*  8
	movea.l	regAdr,a0				*  4
	
	movep.w	1(a0),regAdr				* 16
	move.b	(a0),regAdr				*  8
	movea.l	regAdr,regSP				*  4
	
	FETCH						* = 60
	endopcode ED7B



*--- MLT SP	** HD64180			SZ H VNC
*						
	opcode ED7C
	IllegalOP	2
	endopcode ED7C



*---
	opcode ED7D
	IllegalOP	2
	endopcode ED7D



*---
	opcode ED7E
	IllegalOP	2
	endopcode ED7E



*---
	opcode ED7F
	IllegalOP	2
	endopcode ED7F



*---
	opcode ED80
	IllegalOP	2
	endopcode ED80



*---
	opcode ED81
	IllegalOP	2
	endopcode ED81



*---
	opcode ED82
	IllegalOP	2
	endopcode ED82



*--- OTIM	** HD64180			SZ H VNC
*						
	opcode ED83
	IllegalOP	2
	endopcode ED83



*---
	opcode ED84
	IllegalOP	2
	endopcode ED84



*---
	opcode ED85
	IllegalOP	2
	endopcode ED85



*---
	opcode ED86
	IllegalOP	2
	endopcode ED86



*---
	opcode ED87
	IllegalOP	2
	endopcode ED87



*---
	opcode ED88
	IllegalOP	2
	endopcode ED88



*---
	opcode ED89
	IllegalOP	2
	endopcode ED89



*---
	opcode ED8A
	IllegalOP	2
	endopcode ED8A



*--- OTDM	** HD64180			SZ H VNC
*						
	opcode ED8B
	IllegalOP	2
	endopcode ED8B



*---
	opcode ED8C
	IllegalOP	2
	endopcode ED8C



*---
	opcode ED8D
	IllegalOP	2
	endopcode ED8D



*---
	opcode ED8E
	IllegalOP	2
	endopcode ED8E



*---
	opcode ED8F
	IllegalOP	2
	endopcode ED8F



*---
	opcode ED90
	IllegalOP	2
	endopcode ED90



*---
	opcode ED91
	IllegalOP	2
	endopcode ED91



*---
	opcode ED92
	IllegalOP	2
	endopcode ED92



*--- OTIMR	** HD64180			SZ H VNC
*						
	opcode ED93
	IllegalOP	2
	endopcode ED93



*---
	opcode ED94
	IllegalOP	2
	endopcode ED94



*---
	opcode ED95
	IllegalOP	2
	endopcode ED95



*---
	opcode ED96
	IllegalOP	2
	endopcode ED96



*---
	opcode ED97
	IllegalOP	2
	endopcode ED97



*---
	opcode ED98
	IllegalOP	2
	endopcode ED98



*---
	opcode ED99
	IllegalOP	2
	endopcode ED99



*---
	opcode ED9A
	IllegalOP	2
	endopcode ED9A



*--- OTDMR	** HD64180			SZ H VNC
*						
	opcode ED9B
	IllegalOP	2
	endopcode ED9B



*---
	opcode ED9C
	IllegalOP	2
	endopcode ED9C



*---
	opcode ED9D
	IllegalOP	2
	endopcode ED9D



*---
	opcode ED9E
	IllegalOP	2
	endopcode ED9E



*---
	opcode ED9F
	IllegalOP	2
	endopcode ED9F



*--- LDI					SZ H VNC
*						･･ 0 *0･
*	!! ROM領域への書き込みのチェックはしない
	opcode EDA0
	movea.l	regHL,a0				*  4
	movea.l	regDE,a1				*  4
	move.b	(a0),(a1)				* 12	(HL) -> (DE)
	
	addq.w	#1,regDE				*  4
	addq.w	#1,regHL				*  4
	
	Fclr	___H_VN_				*  8
	subq.w	#1,regBC				*  4
	beq.s	@f					* 10/ 8 (T/F)
		Fset	_____V__			*     8
@@:
	FETCH						* 
	endopcode EDA0



*--- CPI					SZ H VNC
*						** * *1･
*				SZ_H____ = A - (HL)
*				++HL
*				--BC
*				if (BC == 0) PV = 0
*				else PV = 1
	opcode EDA1
	movea.l	regHL,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	lea	FtblSUB8,a1				* 12
	moveq.l	#flag_Cc,d0				*  4
	and.b	regF,d0					*  4
	move.b	#setSZ_H__N_,regF			*  8
	and.b	(a1,d1.l),regF				* 14
	or.b	d0,regF					*  4
	
	addq.w	#1,regHL				*  4
	subq.w	#1,regBC				*  4
	beq.s	@f					* 10/ 8 (T/F)
	Fset	_____V__				*  8
@@:
	FETCH						* 
	endopcode EDA1



*--- INI					SZ H VNC
*						x* x x1･
	opcode EDA2
	Fset	_Z____N_				*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	movea.l	regHL,a0				*  4
	move.b	d1,(a0)					*  8
	
	addq.w	#1,regHL				*  4
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bls.s	@f					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcs.s	@f					* 10/ 8 (T/F)
	Fclr	_Z______				*  8
@@:
	FETCH						* 
	endopcode EDA2



*--- OUTI					SZ H VNC
*						x* x x1･
	opcode EDA3
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	Fset	_Z____N_				*  8
	
	addq.w	#1,regHL				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bls.s	@f					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcs.s	@f					* 10/ 8 (T/F)
	Fclr	_Z______				*  8
@@:
	FETCH						* 
	endopcode EDA3



*---
	opcode EDA4
	IllegalOP	2
	endopcode EDA4



*---
	opcode EDA5
	IllegalOP	2
	endopcode EDA5



*---
	opcode EDA6
	IllegalOP	2
	endopcode EDA6



*---
	opcode EDA7
	IllegalOP	2
	endopcode EDA7



*--- LDD					SZ H VNC
*						･･ 0 *0･
*	!! ROM領域への書き込みのチェックはしない
	opcode EDA8
	movea.l	regHL,a0				*  4
	movea.l	regDE,a1				*  4
	move.b	(a0),(a1)				* 12
	
	subq.w	#1,regDE				*  4
	subq.w	#1,regHL				*  4
	
	Fclr	___H_VN_				*  8
	subq.w	#1,regBC				*  4
	beq.s	@f					* 10/ 8 (T/F)
		Fset	_____V__			*  8
@@:
	FETCH						* 
	endopcode EDA8



*--- CPD					SZ H VNC
*						** * *1･
*				SZ_H____ = A - (HL)
*				--HL
*				--BC
*				if (BC == 0) PV = 0
*				else PV = 1
	opcode EDA9
	movea.l	regHL,a0				*  4
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	lea	FtblSUB8,a1				* 12
	moveq.l	#flag_Cc,d0				*  4
	and.b	regF,d0					*  4
	move.b	#setSZ_H__N_,regF			*  8
	and.b	(a1,d1.l),regF				* 14
	or.b	d0,regF					*  4
	
	subq.w	#1,regHL				*  4
	subq.w	#1,regBC				*  4
	beq.s	@f					* 10/ 8 (T/F)
	Fset	_____V__				*  8
@@:
	FETCH						* 
	endopcode EDA9



*--- IND					SZ H VNC
*						x* x x1･
	opcode EDAA
	Fset	_Z____N_				*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	movea.l	regHL,a0				*  4
	move.b	d1,(a0)					*  8
	
	subq.w	#1,regHL				*  4
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bls.s	@f					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcs.s	@f					* 10/ 8 (T/F)
	Fclr	_Z______				*  8
@@:
	FETCH						* 
	endopcode EDAA



*--- OUTD					SZ H VNC
*						x* x x1･
	opcode EDAB
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	Fset	_Z____N_				*  8
	
	subq.w	#1,regHL				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bls.s	@f					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcs.s	@f					* 10/ 8 (T/F)
	Fclr	_Z______				*  8
@@:
	FETCH						* 
	endopcode EDAB



*---
	opcode EDAC
	IllegalOP	2
	endopcode EDAC



*---
	opcode EDAD
	IllegalOP	2
	endopcode EDAD



*---
	opcode EDAE
	IllegalOP	2
	endopcode EDAE



*---
	opcode EDAF
	IllegalOP	2
	endopcode EDAF



*--- LDIR					SZ H VNC
*						･･ 0 00･
*	!! ROM領域への書き込みのチェックはしない
	opcode EDB0
@@:
	movea.l	regHL,a0				*  4
	movea.l	regDE,a1				*  4
	move.b	(a0),(a1)				* 12	(HL) -> (DE)
	
	addq.w	#1,regHL				*  4
	addq.w	#1,regDE				*  4
	subq.w	#1,regBC				*  4
	bne.s	@b					* 10/ 8 (T/F)
	
	Fclr	___H_VN_				*  8
	FETCH						* 
	endopcode EDB0



*--- CPIR					SZ H VNC
*						** * *1･
*			for (;;)
*			{
*				SZ_H____ = A - (HL)
*				++HL
*				--BC
*				if (BC == 0) PV = 0
*				else PV = 1
*				if (BC == 0 || Z == 1) break;
*			}
	opcode EDB1
@@:
	movea.l	regHL,a0				*  4
	addq.w	#1,regHL				*  4
	subq.w	#1,regBC				*  4
	beq.s	@f					* 10/ 8 (T/F)
	cmp.b	(a0),regA				*  8
	bne.s	@b					* 10/ 8 (T/F)
@@:
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	lea	FtblSUB8,a1				* 12
	moveq.l	#setSZ_H__N_,d0				*  4
	and.b	(a1,d1.l),d0				* 14
	andi.b	#flag_Cc,regF				*  8	!!
	or.b	d0,regF					*  4
	
	tst.w	regBC					*  4
	beq.s	@f					* 10/ 8 (T/F)
	Fset	_____V__				*  8
@@:
	FETCH						* 
	endopcode EDB1



*--- INIR					SZ H VNC
*						x1 x x1･
	opcode EDB2
	Fset	_Z____N_				*  8
	
@@:
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	movea.l	regHL,a0				*  4
	move.b	d1,(a0)					*  8
	
	addq.w	#1,regHL				*  4
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bhi.s	@b					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcc.s	@b					* 10/ 8 (T/F)
	
	FETCH						* 
	endopcode EDB2



*--- OTIR					SZ H VNC
*						x1 x x1･
	opcode EDB3
@@:
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	addq.w	#1,regHL				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bhi.s	@b					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcc.s	@b					* 10/ 8 (T/F)
	
	Fset	_Z____N_				*  8
	
	FETCH						* 
	endopcode EDB3



*---
	opcode EDB4
	IllegalOP	2
	endopcode EDB4



*---
	opcode EDB5
	IllegalOP	2
	endopcode EDB5



*---
	opcode EDB6
	IllegalOP	2
	endopcode EDB6



*---
	opcode EDB7
	IllegalOP	2
	endopcode EDB7



*--- LDDR					SZ H VNC
*						･･ 0 00･
*	!! ROM領域への書き込みのチェックはしない
	opcode EDB8
@@:
	movea.l	regHL,a0				*  4
	movea.l	regDE,a1				*  4
	move.b	(a0),(a1)				* 12	(HL) -> (DE)
	
	subq.w	#1,regHL				*  4
	subq.w	#1,regDE				*  4
	
	subq.w	#1,regBC				*  4
	bne.s	@b					* 10/ 8 (T/F)
	
	Fclr	___H_VN_				*  8
	
	FETCH						* 
	endopcode EDB8



*--- CPDR					SZ H VNC
*						** * *1･
*			for (;;)
*			{
*				SZ_H____ = A - (HL)
*				--HL
*				--BC
*				if (BC == 0) PV = 0
*				else PV = 1
*				if (BC == 0 || Z == 1) break;
*			}
	opcode EDB9
@@:
	movea.l	regHL,a0				*  4
	subq.w	#1,regHL				*  4
	subq.w	#1,regBC				*  4
	beq.s	@f					* 10/ 8 (T/F)
	cmp.b	(a0),regA				*  8
	bne.s	@b					* 10/ 8 (T/F)
@@:
	move.b	(a0),-(regSwap)				* 12
	move.w	(regSwap)+,d1				*  8
	move.b	regA,d1					*  4	[src:dst] (dst-src)
	lea	FtblSUB8,a1				* 12
	moveq.l	#setSZ_H__N_,d0				*  4
	and.b	(a1,d1.l),d0				* 14
	andi.b	#flag_Cc,regF				*  8
	or.b	d0,regF					*  4
	
	tst.w	regBC					*  4
	beq.s	@f					* 10/ 8 (T/F)
		Fset	_____V__			*     8
@@:
	FETCH						* 
	endopcode EDB9



*--- INDR					SZ H VNC
*						x1 x x1･
	opcode EDBA
	Fset	_Z____N_				*  8
	
@@:
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d0			*  8
	jsr	PROC_IN(regIO,d0.w)			* 22
	
	movea.l	regHL,a0				*  4
	move.b	d1,(a0)					*  8
	
	subq.w	#1,regHL				*  4
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bhi.s	@b					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcc.s	@b					* 10/ 8 (T/F)
	
	FETCH						* 
	endopcode EDBA



*--- OTDR					SZ H VNC
*						x1 x x1･
	opcode EDBB
@@:
	movea.l	regHL,a0				*  4
	move.b	(a0),d0					*  8
	
	move.b	regBC,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	jsr	PROC_OUT(regIO,d1.w)			* 22
	
*	subi.w	#$0100,regBC				*  8
	sub.w	regZjmp,regBC				*  4
	subq.w	#1,regHL				*  4
	
*	cmpi.w	#$00ff,regBC				*  8
*	bhi.s	@b					* 10/ 8 (T/F)
	cmp.w	regZjmp,regBC				*  4
	bcc.s	@b					* 10/ 8 (T/F)
	
	Fset	_Z____N_				*  8
	
	FETCH						* 
	endopcode EDBB



*---
	opcode EDBC
	IllegalOP	2
	endopcode EDBC



*---
	opcode EDBD
	IllegalOP	2
	endopcode EDBD



*---
	opcode EDBE
	IllegalOP	2
	endopcode EDBE



*---
	opcode EDBF
	IllegalOP	2
	endopcode EDBF



*---
	opcode EDC0
	IllegalOP	2
	endopcode EDC0



*---
	opcode EDC1
	IllegalOP	2
	endopcode EDC1



*---
	opcode EDC2
	IllegalOP	2
	endopcode EDC2



*---
	opcode EDC3
	IllegalOP	2
	endopcode EDC3



*---
	opcode EDC4
	IllegalOP	2
	endopcode EDC4



*---
	opcode EDC5
	IllegalOP	2
	endopcode EDC5



*---
	opcode EDC6
	IllegalOP	2
	endopcode EDC6



*---
	opcode EDC7
	IllegalOP	2
	endopcode EDC7



*---
	opcode EDC8
	IllegalOP	2
	endopcode EDC8



*---
	opcode EDC9
	IllegalOP	2
	endopcode EDC9



*---
	opcode EDCA
	IllegalOP	2
	endopcode EDCA



*---
	opcode EDCB
	IllegalOP	2
	endopcode EDCB



*---
	opcode EDCC
	IllegalOP	2
	endopcode EDCC



*---
	opcode EDCD
	IllegalOP	2
	endopcode EDCD



*---
	opcode EDCE
	IllegalOP	2
	endopcode EDCE



*---
	opcode EDCF
	IllegalOP	2
	endopcode EDCF



*---
	opcode EDD0
	IllegalOP	2
	endopcode EDD0



*---
	opcode EDD1
	IllegalOP	2
	endopcode EDD1



*---
	opcode EDD2
	IllegalOP	2
	endopcode EDD2



*---
	opcode EDD3
	IllegalOP	2
	endopcode EDD3



*---
	opcode EDD4
	IllegalOP	2
	endopcode EDD4



*---
	opcode EDD5
	IllegalOP	2
	endopcode EDD5



*---
	opcode EDD6
	IllegalOP	2
	endopcode EDD6



*---
	opcode EDD7
	IllegalOP	2
	endopcode EDD7



*---
	opcode EDD8
	IllegalOP	2
	endopcode EDD8



*---
	opcode EDD9
	IllegalOP	2
	endopcode EDD9



*---
	opcode EDDA
	IllegalOP	2
	endopcode EDDA



*---
	opcode EDDB
	IllegalOP	2
	endopcode EDDB



*---
	opcode EDDC
	IllegalOP	2
	endopcode EDDC



*---
	opcode EDDD
	IllegalOP	2
	endopcode EDDD



*---
	opcode EDDE
	IllegalOP	2
	endopcode EDDE



*---
	opcode EDDF
	IllegalOP	2
	endopcode EDDF



*---
	opcode EDE0
	IllegalOP	2
	endopcode EDE0



*---
	opcode EDE1
	IllegalOP	2
	endopcode EDE1



*---
	opcode EDE2
	IllegalOP	2
	endopcode EDE2



*---
	opcode EDE3
	IllegalOP	2
	endopcode EDE3



*---
	opcode EDE4
	IllegalOP	2
	endopcode EDE4



*---
	opcode EDE5
	IllegalOP	2
	endopcode EDE5



*---
	opcode EDE6
	IllegalOP	2
	endopcode EDE6



*---
	opcode EDE7
	IllegalOP	2
	endopcode EDE7



*---
	opcode EDE8
	IllegalOP	2
	endopcode EDE8



*---
	opcode EDE9
	IllegalOP	2
	endopcode EDE9



*---
	opcode EDEA
	IllegalOP	2
	endopcode EDEA



*---
	opcode EDEB
	IllegalOP	2
	endopcode EDEB



*---
	opcode EDEC
	IllegalOP	2
	endopcode EDEC



*---
	opcode EDED
	IllegalOP	2
	endopcode EDED



*---
	opcode EDEE
	IllegalOP	2
	endopcode EDEE



*---
	opcode EDEF
	IllegalOP	2
	endopcode EDEF



*---
	opcode EDF0
	IllegalOP	2
	endopcode EDF0



*---
	opcode EDF1
	IllegalOP	2
	endopcode EDF1



*---
	opcode EDF2
	IllegalOP	2
	endopcode EDF2



*---
	opcode EDF3
	IllegalOP	2
	endopcode EDF3



*---
	opcode EDF4
	IllegalOP	2
	endopcode EDF4



*---
	opcode EDF5
	IllegalOP	2
	endopcode EDF5



*---
	opcode EDF6
	IllegalOP	2
	endopcode EDF6



*---
	opcode EDF7
	IllegalOP	2
	endopcode EDF7



*---
	opcode EDF8
	IllegalOP	2
	endopcode EDF8



*---
	opcode EDF9
	IllegalOP	2
	endopcode EDF9



*---
	opcode EDFA
	IllegalOP	2
	endopcode EDFA



*---
	opcode EDFB
	IllegalOP	2
	endopcode EDFB



*---
	opcode EDFC
	IllegalOP	2
	endopcode EDFC



*---
	opcode EDFD
	IllegalOP	2
	endopcode EDFD



*---
	opcode EDFE
	IllegalOP	2
	endopcode EDFE



*---
	opcode EDFF
	IllegalOP	2
	endopcode EDFF


