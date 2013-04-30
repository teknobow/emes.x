;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;


	.even

	.xdef	NMIroutine,COPYinterrupt
	.xdef	_interrupt_inDebugger
	.xdef	_Org_NMIintr,_Org_COPYintr

	.xref	Xoptab,CBoptab,DDoptab,DDCBoptab,EDoptab,FDoptab,FDCBoptab,STRtab

	.xref	Z80mem,Zjmp

	.xref	exitMSXemu


CR	equ	$0d
LF	equ	$0a

	*    +------------+
	* -40|  regAdr    |	d2
	*    +------------+
	* -36|  regA      |	d3
	*    +------------+
	* -32|  regF      |	d4
	*    +------------+
	* -28|  regBC     |	d5
	*    +------------+
	* -24|  regDE     |	d6
	*    +------------+
	* -20|  regHL     |	d7
	*    +------------+
	* -16|  a2        |	regVadr
	*    +------------+
	* -12|  regIO     |	a3
	*    +------------+
	*  -8|  regZjmp   |	a4
	*    +------------+
	*  -4|  regPC     |	a5
	*    +------------+
	*a6->|  regSP     |	a6
	*    +------------+


Zregs	reg	regAdr/regA/regF/regBC/regDE/regHL/regVadr/regIO/regZjmp/regPC

offA2	.equ	-35
offA	.equ	-33
offF2	.equ	-31
offF	.equ	-29
offBC	.equ	-26
offDE	.equ	-22
offHL	.equ	-18
offVRAM	.equ	-16
offPCr	.equ	-4
offPC	.equ	-2
offSP	.equ	2


Z80INTREQ		.equ	_Z80INTREQ-Ftbl
interrupt_inDebugger	.equ	_interrupt_inDebugger-Ftbl
traceFlag		.equ	_traceFlag-Ftbl


_debugger:
	
.if 1	;-- DEBUG
	
	movem.l	d0-d1,-(sp)
	
	;d1
	move.l	d1,d0
	swap.w	d0
	tst.w	d0
	bne	_debugger_err
	
	;regA
	move.l	regA,d0
	clr.b	d0
	tst.w	d0
	bne	_debugger_err
	
	swap.w	d0
	clr.b	d0
	tst.w	d0
	bne	_debugger_err
	
	;
	move.w	(Z80mem),d1
	
	;regBC
	move.l	regBC,d0
	swap.w	d0
	cmp.w	d0,d1
	bne	_debugger_err
	
	;regDE
	move.l	regDE,d0
	swap.w	d0
	cmp.w	d0,d1
	bne	_debugger_err
	
	;regHL
	move.l	regHL,d0
	swap.w	d0
	cmp.w	d0,d1
	bne	_debugger_err
	
	;regPC
	move.l	regPC,d0
	swap.w	d0
	cmp.w	d0,d1
	bne	_debugger_err
	
	;regSP
	move.l	regSP,d0
	swap.w	d0
	cmp.w	d0,d1
	bne	_debugger_err
	
	;regAdr
	move.l	regAdr,d0
	swap.w	d0
	cmp.w	d0,d1
	bne	_debugger_err
	
	;regZjmp
	cmp.l	(Zjmp),regZjmp
	bne	_debugger_err
	
	movem.l	(sp)+,d0-d1
	
	;-- DEBUG
.endif
	
	
	*-- トレースモード ?
	tst.b	traceFlag(regFtbl)
	bne	_debugger2
	
	
	*-- ブレイクポイントが設定されていなければZ80デバッガへ
	move.w	breakPoint_old(regZjmp),d0
	beq	_debugger2
	
	*-- ブレイクポイントかどうか
	addq.w	#1,d0
	cmp.w	regPC,d0
	beq	_debugger2
	
	*-- ブレイクポイントでなければ割り込みの検出を行いエミュレートの再開
	tas	interrupt_inDebugger(regFtbl)
	bmi	1f
	
	* 割り込み発生
	subq.w	#1,regPC
	move.w	#$ff08,d1
	jmp	OP_Z(regZjmp,d1.w)
	
1:	
	move.b	#8,d1			* 
	jmp	OP_Z(regZjmp,d1.w)		* 14 (4) = 34


.if 1
_debugger_err:
	movem.l	(sp)+,d0-d1
.endif
_debugger2:
	subq.l	#1,regPC
	
	link	a6,#-40			;regSP
	movem.l	Zregs,-40(a6)
	
	jsr	printRegs(pc)
	
	*--
	move.b	#-1,traceFlag+44(regFtbl)
	move.b	#IN_Z80DEBUGGER,Z80INTREQ+44(regFtbl)
	move.w	#IN_Z80DEBUGGER,44(regOP1fetch)
	
	
debugger_loop:
	
	;-- キー入力待ち
	move.b	_keywaitFlag(pc),d0
	beq.s	exitDebugger
	
	*--
	move.w	#$08,-(sp)		* GETC
	.dc.w	$ff0c			* DOS KFLUSH
	addq.w	#2,sp
	
	cmpi.b	#'g',d0
	bne	@f
	
	*-- トレースモード解除
	tst.w	breakPoint_old(regZjmp)
	bne	1f
	
	* break point なし
	move.b	#0,traceFlag+44(regFtbl)
	move.b	#$04,Z80INTREQ+44(regFtbl)
	move.w	#$08,44(regOP1fetch)
	
	bra	exitDebugger
1:
	* break point あり
	move.b	#0,traceFlag+44(regFtbl)
	move.b	#IN_Z80DEBUGGER,Z80INTREQ+44(regFtbl)
	move.w	#IN_Z80DEBUGGER,44(regOP1fetch)
	
	bra	exitDebugger

@@:
	cmpi.b	#'b',d0
	beq	enter68kDebugger
	
	
	cmpi.b	#'p',d0
	bne	@f
	
	;-- キー入力待ちなしモードへ
	sf.b	_keywaitFlag
	bra	exitDebugger
@@:
	
	;---
	cmpi.b	#'v',d0
	bne	@f
	
	jsr	printVDPreg(pc)
	bra	debugger_loop
	
@@:
	;---
	cmpi.b	#'q',d0
	bne	@f
	
	jmp	exitMSXemu
@@:	
	;---
exitDebugger:
	
	movem.l	-40(a6),Zregs
	unlk	a6
	
	
	move.b	(regPC)+,(regOP1fetch)		* 12 (2)
	move.w	(regOP1fetch),d1		*  8 (2)
	move.b	#8,d1			* 
	jmp	OP_Z(regZjmp,d1.w)		* 14 (4) = 34
	
	
	
	
	;------------------------------
enter68kDebugger:
	
	movem.l	-40(a6),Zregs
	unlk	a6
	
	move.b	(regPC)+,(regOP1fetch)		* 12 (2)
	move.w	(regOP1fetch),d1		*  8 (2)
	move.b	#8,d1			* 
	
	*-- 68000 デバッガにはいる
	trap	#9
	
	jmp	OP_Z(regZjmp,d1.w)		* 14 (4) = 34


	;-- 




*----------------------------------------------------
* 
* AF :xxxx BC :xxxx DE :xxxx HL :xxxx IX:xxxx IY:xxxx VDP:xxxxx IFF:x
* AF':xxxx BC':xxxx DE':xxxx HL':xxxx PC:xxxx SP:xxxx flag:[SZ_H_VNC]
* xxxx xx xx xx xx  nimonic operand

printRegs:
	moveq.l	#0,d0
	
	*-- A regster
	lea	AregValue(pc),a3
	move.b	offA(a6),d0
	jsr	hex2chr(pc)
	
	*-- F register
	lea	FregValue(pc),a3
	move.b	offF(a6),d0
	rol.b	#1,d0
	jsr	hex2chr(pc)
	
	*-- flag status
	lea	flagValue,a3
	jsr	flag2chr(pc)
	
	*-- BC register
	lea	BCregValue(pc),a3
	move.w	offBC(a6),d0
	jsr	word2chr(pc)
	
	*-- DE register
	lea	DEregValue(pc),a3
	move.w	offDE(a6),d0
	jsr	word2chr(pc)
	
	*-- HL register
	lea	HLregValue(pc),a3
	move.w	offHL(a6),d0
	jsr	word2chr(pc)
	
	*-- IX register
	lea	IXregValue(pc),a3
	move.w	_workIX,d0
	jsr	word2chr(pc)
	
	*-- IY register
	lea	IYregValue(pc),a3
	move.w	_workIY,d0
	jsr	word2chr(pc)
	
	*-- A' register
	lea	A2regValue(pc),a3
	move.b	offA2(a6),d0
	jsr	hex2chr(pc)
	
	*-- F' register
	lea	F2regValue(pc),a3
	move.b	offF2(a6),d0
	rol.b	#1,d0
	jsr	hex2chr(pc)
	
	*-- BC' register
	lea	BC2regValue(pc),a3
	move.w	_workBC2,d0
	jsr	word2chr(pc)
	
	*-- DE' register
	lea	DE2regValue(pc),a3
	move.w	_workDE2,d0
	jsr	word2chr(pc)
	
	*-- HL' register
	lea	HL2regValue(pc),a3
	move.w	_workHL2,d0
	jsr	word2chr(pc)
	
	*-- PC register
	lea	PCregValue(pc),a3
	move.w	offPC(a6),d0
	jsr	word2chr(pc)
	
	*-- SP register
	lea	SPregValue(pc),a3
	move.w	offSP(a6),d0
	jsr	word2chr(pc)
	
	;-- VRAM register
	lea	VRAMregValue(pc),a3
	move.l	offVRAM(a6),d0
	jsr	vdp2chr(pc)
	
	;-- IFF
	lea	IFFValue(pc),a3
	move.b	_Z80_DIEI(pc),d0
	bne	1f
	move.b	#'0',(a3)+
	bra	2f
1:
	move.b	#'1',(a3)+
2:
	
	;-- dump領域クリア
	lea	memoryValue(pc),a3
	moveq.l	#16-1,d0
@@:
	move.b	#' ',(a3)+
	dbra	d0,@b
	
	*-- disassemble
	lea	disasmLine(pc),a3
	move.l	offPCr(a6),a0
	jsr	disasm(pc)		* ret : d2.w ... instruction size
	
	*-- EOS
	move.b	#CR,(a3)+
	move.b	#LF,(a3)+
	clr.b	(a3)
	
	*-- dump
	lea	memoryValue(pc),a3
	move.l	offPCr(a6),a0
	jsr	dumpMemory(pc)
	
	
.if 0
	moveq.l	#$21,d0			* _B_PRINT
	lea	ZregsLine,a1
	trap	#15
.else
	pea.l	ZregsLine
	.dc.w	$ff09
	addq.w	#4,sp
.endif
	
	
	rts
	
ZregsLine:
		.dc.b	"AF :"
AregValue:	.dc.b	"xx"
FregValue:	.dc.b	"xx"
		.dc.b	" BC :"
BCregValue:	.dc.b	"xxxx"
		.dc.b	" DE :"
DEregValue:	.dc.b	"xxxx"
		.dc.b	" HL :"
HLregValue:	.dc.b	"xxxx"
		.dc.b	" IX:"
IXregValue:	.dc.b	"xxxx"
		.dc.b	" IY:"
IYregValue:	.dc.b	"xxxx"
		.dc.b	" VDP:"
VRAMregValue:	.dc.b	"xxxxx"
		.dc.b	" IFF:"
IFFValue:	.dc.b	"x"
		.dc.b	$0d,$0a
		.dc.b	"AF':"
A2regValue:	.dc.b	"xx"
F2regValue:	.dc.b	"xx"
		.dc.b	" BC':"
BC2regValue:	.dc.b	"xxxx"
		.dc.b	" DE':"
DE2regValue:	.dc.b	"xxxx"
		.dc.b	" HL':"
HL2regValue:	.dc.b	"xxxx"
		.dc.b	" PC:"
PCregValue:	.dc.b	"xxxx"
		.dc.b	" SP:"
SPregValue:	.dc.b	"xxxx"
		.dc.b	" flag:["
flagValue:	.dc.b	"SZ_H_VNC]"
		.dc.b	$0d,$0a
memoryValue:	.dc.b	"xxxx xx xx xx xx  "
disasmLine:	.dcb.b	128,0

	.even



;------------------------------------------------------
;
; PatGen:xxxxx PatName:xxxxx Color:xxxxx SprPatGen:xxxxx SprAtr:xxxxx
;  0:xx  1:xx  2:xx  3:xx  4:xx  5:xx  6:xx  7:xx  8:xx  9:00
; 10:xx 11:xx 12:xx 13:xx 14:xx 15:xx 16:xx 17:xx 18:xx 19:xx
; 20:xx 21:xx 22:xx 23:xx
;             32:xx 33:xx 34:xx 35:xx 36:xx 37:xx 38:xx 39:xx
; 40:xx 41:xx 42:xx 43:xx 44:xx 45:xx 46:xx

printVDPreg:
	lea	PatGenValue(pc),a3
	move.l	_PatGenTbl(pc),d0
	jsr	vdp2chr(pc)
	
	lea	PatNameValue(pc),a3
	move.l	_PatNameTbl(pc),d0
	jsr	vdp2chr(pc)
	
	lea	ColorValue(pc),a3
	move.l	_ColorTbl(pc),d0
	jsr	vdp2chr(pc)
	
	lea	SprPatGenValue(pc),a3
	move.l	_SprPatGenTbl(pc),d0
	jsr	vdp2chr(pc)
	
	lea	SprAtrValue(pc),a3
	move.l	_SprAtrTbl(pc),d0
	jsr	vdp2chr(pc)
	
	.irp	num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
	lea	VDPValue_&num(pc),a3
	move.b	_VDPreg_&num(pc),d0
	jsr	hex2chr(pc)
	.endm
	
	.irp	num,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46
	lea	VDPValue_&num(pc),a3
	move.b	_VDPreg_&num(pc),d0
	jsr	hex2chr(pc)
	.endm
	
	
	pea.l	VDPregsLine
	.dc.w	$ff09
	addq.w	#4,sp
	
	rts


VDPregsLine:
	.dc.b	$0d,$0a
	.dc.b	'PatGen:'
PatGenValue:
	.dc.b	'xxxxx'
	.dc.b	' PatName:'
PatNameValue:
	.dc.b	'xxxxx'
	.dc.b	' Color:'
ColorValue:
	.dc.b	'xxxxx'
	.dc.b	' SprPatGen:'
SprPatGenValue:
	.dc.b	'xxxxx'
	.dc.b	' SprAtr:'
SprAtrValue:
	.dc.b	'xxxxx'
	.dc.b	$0d,$0a

	.dc.b	' 0:'
VDPValue_0:
	.dc.b	'xx'
	.dc.b	'  1:'
VDPValue_1:
	.dc.b	'xx'
	.dc.b	'  2:'
VDPValue_2:
	.dc.b	'xx'
	.dc.b	'  3:'
VDPValue_3:
	.dc.b	'xx'
	.dc.b	'  4:'
VDPValue_4:
	.dc.b	'xx'
	.dc.b	'  5:'
VDPValue_5:
	.dc.b	'xx'
	.dc.b	'  6:'
VDPValue_6:
	.dc.b	'xx'
	.dc.b	'  7:'
VDPValue_7:
	.dc.b	'xx'
	.dc.b	'  8:'
VDPValue_8:
	.dc.b	'xx'
	.dc.b	'  9:'
VDPValue_9:
	.dc.b	'xx' 
	.dc.b	$0d,$0a
	
	.dc.b	'10:'
VDPValue_10:
	.dc.b	'xx'
	.dc.b	' 11:'
VDPValue_11:
	.dc.b	'xx'
	.dc.b	' 12:'
VDPValue_12:
	.dc.b	'xx'
	.dc.b	' 13:'
VDPValue_13:
	.dc.b	'xx'
	.dc.b	' 14:'
VDPValue_14:
	.dc.b	'xx'
	.dc.b	' 15:'
VDPValue_15:
	.dc.b	'xx'
	.dc.b	' 16:'
VDPValue_16:
	.dc.b	'xx'
	.dc.b	' 17:'
VDPValue_17:
	.dc.b	'xx'
	.dc.b	' 18:'
VDPValue_18:
	.dc.b	'xx'
	.dc.b	' 19:'
VDPValue_19:
	.dc.b	'xx'
	.dc.b	$0d,$0a
	
	.dc.b	'20:'
VDPValue_20:
	.dc.b	'xx'
	.dc.b	' 21:'
VDPValue_21:
	.dc.b	'xx'
	.dc.b	' 22:'
VDPValue_22:
	.dc.b	'xx'
	.dc.b	' 23:'
VDPValue_23:
	.dc.b	'xx'
	.dc.b	$0d,$0a

	.dc.b	'            32:'
VDPValue_32:
	.dc.b	'xx'
	.dc.b	' 33:'
VDPValue_33:
	.dc.b	'xx'
	.dc.b	' 34:'
VDPValue_34:
	.dc.b	'xx'
	.dc.b	' 35:'
VDPValue_35:
	.dc.b	'xx'
	.dc.b	' 36:'
VDPValue_36:
	.dc.b	'xx'
	.dc.b	' 37:'
VDPValue_37:
	.dc.b	'xx'
	.dc.b	' 38:'
VDPValue_38:
	.dc.b	'xx'
	.dc.b	' 39:'
VDPValue_39:
	.dc.b	'xx'
	.dc.b	$0d,$0a
	
	.dc.b	'40:'
VDPValue_40:
	.dc.b	'xx'
	.dc.b	' 41:'
VDPValue_41:
	.dc.b	'xx'
	.dc.b	' 42:'
VDPValue_42:
	.dc.b	'xx'
	.dc.b	' 43:'
VDPValue_43:
	.dc.b	'xx'
	.dc.b	' 44:'
VDPValue_44:
	.dc.b	'xx'
	.dc.b	' 45:'
VDPValue_45:
	.dc.b	'xx'
	.dc.b	' 46:'
VDPValue_46:
	.dc.b	'xx'
	.dc.b	$0d,$0a
	.dc.b	$0d,$0a
	.dc.b	0
	
	.even



*------------------------------------------------------
* a0 ... memory
* a3 ... string buf
*
* ret:
*	d2 ... instruction size
disasm:
	moveq.l	#0,d0
	move.b	(a0),d0
	
	cmpi.b	#$CB,d0
	beq	_CB
	
	cmpi.b	#$DD,d0
	beq	_DD
	
	cmpi.b	#$ED,d0
	beq	_ED
	
	cmpi.b	#$FD,d0
	beq	_FD
	
	lea	Xoptab,a1
	bra	1f
	
_CB:	move.b	1(a0),d0
	lea	CBoptab,a1
	bra	1f

_DD:	move.b	1(a0),d0
	cmpi.b	#$CB,d0
	beq	_DDCB
	
	lea	DDoptab,a1
	bra	1f

_DDCB:	move.b	3(a0),d0
	lea	DDCBoptab,a1
	bra	1f

_ED:	move.b	1(a0),d0
	lea	EDoptab,a1
	bra	1f

_FD:	move.b	1(a0),d0
	cmpi.b	#$CB,d0
	beq	_FDCB
	
	lea	FDoptab,a1
	bra	1f

_FDCB:	move.b	3(a0),d0
	lea	FDCBoptab,a1
**	bra	1f
	
1:	
	add.w	d0,d0
	add.w	d0,d0
	
	move.w	2(a1,d0.w),d2			* size
	
	move.w	(a1,d0.w),d0			* nimonic, operand string
	
	
	lea	STRtab,a1
	
	* nimonic
	move.w	d0,d1
	andi.w	#%1111_1110_0000_0000,d1
	lsr.w	#6,d1
	lea	(a1,d1.w),a2
	jsr	strcpy(pc)
	
	* operand
	andi.w	#%0000_0001_1111_1111,d0
	lsl.w	#3,d0
	lea	(a1,d0.w),a2
	jsr	strcpy(pc)
	
	clr.b	(a3)				* EOS
	
	rts

*---
	* a0 ... Z80 memory
	* a2 ... source
	* a3 ... destination
	* d0 ... 要保存
strcpy:
	move.l	d0,-(sp)
	
	moveq.l	#8-1,d1				* loop counter
@@:
	move.b	(a2)+,d0
	
	cmpi.b	#'!',d0
	beq	o1b
	
	cmpi.b	#'#',d0
	beq	o2b
	
	cmpi.b	#'$',d0
	beq	o3b
	
	cmpi.b	#'%',d0
	beq	o1w
	
	cmpi.b	#'&',d0
	beq	o2w
	
	cmpi.b	#'@',d0
	beq	reladd
	
	move.b	d0,(a3)+
1:
	dbra	d1,@b
	
	move.l	(sp)+,d0
	rts

o1b:	* +1 [byte]
	move.b	#'$',(a3)+
	
	move.b	1(a0),d0
	jsr	hex2chr(pc)
	bra	1b

o2b:	* +2 [byte]
	move.b	#'$',(a3)+
	
	move.b	2(a0),d0
	jsr	hex2chr(pc)
	bra	1b

o3b:	* +3 [byte]
	move.b	#'$',(a3)+
	
	move.b	3(a0),d0
	jsr	hex2chr(pc)
	bra	1b

o1w:	* +1 [word]
	move.b	#'$',(a3)+
	
	move.b	2(a0),d0
	jsr	hex2chr(pc)
	move.b	1(a0),d0
	jsr	hex2chr(pc)
	bra	1b

o2w:	* +2 [word]
	move.b	#'$',(a3)+
	
	move.b	3(a0),d0
	jsr	hex2chr(pc)
	move.b	2(a0),d0
	jsr	hex2chr(pc)
	bra	1b

reladd:
	move.l	d1,-(sp)
	
	move.b	#'$',(a3)+
	
	move.w	a0,d1
	move.b	1(a0),d0
	ext.w	d0
	add.w	d0,d1
	addq.w	#2,d1
	
	move.w	d1,-(sp)
	move.b	(sp)+,d0
	jsr	hex2chr(pc)
	move.b	d1,d0
	jsr	hex2chr(pc)
	
	move.l	(sp)+,d1
	
	bra	1b


*--------
* d0.b ... data
; a3.l ... buf
hex2chr:
	move.l	d1,-(sp)
	
	moveq.l	#0,d1
	move.b	d0,d1
	lsr.b	#4,d1
	move.b	hexstr(pc,d1.w),(a3)+
	
	move.b	d0,d1
	andi.b	#%0000_1111,d1
	move.b	hexstr(pc,d1.w),(a3)+
	
	move.l	(sp)+,d1
	rts
	
hexstr:
	.dc.b	'0123456789ABCDEF'


*----------
* d0.w ... data
; a3.l ... buf
word2chr:
	move.w	d0,-(sp)
	
	move.b	(sp),d0
	jsr	hex2chr(pc)
	
	move.w	(sp)+,d0
	jsr	hex2chr(pc)
	
	rts
	
	
;----------
; d0.l ... data
; a3.l ... buf
vdp2chr:
	btst.l	#16,d0
	bne	1f
	move.b	#'0',(a3)+
	bra	2f
1:
	move.b	#'1',(a3)+
2:
	bra	word2chr


*------------------
* Dump Memory
*
* IN :	a0   ... Z80 memory
*	a3   ... destination
*	d2.w ... size
*
dumpMemory:
	*-- address
	move.w	a0,d0
	jsr	word2chr(pc)
	
	subq.w	#1,d2
@@:
	addq.l	#1,a3
	move.b	(a0)+,d0
	jsr	hex2chr(pc)
	dbra	d2,@b
	
	rts


*--------------------------
* F register to character (?)
*
* IN :	a3   ... destination
*	d0.b ... data
flag2chr:
	.irpc	ch,"SZcH_VNC"
	move.b	#'&ch',d1
	jsr	chkFlag(pc)
	.endm
	
	rts

chkFlag:
	add.b	d0,d0
	bcc	@f
	move.b	d1,(a3)+
	rts
@@:
	move.b	#'_',(a3)+
	rts



*----------------------------------------------------------------------------------------------
* インタラプトスイッチ処理
* Z80デバッガの起動
NMIroutine:
	move.b	#IN_Z80DEBUGGER,_Z80INTREQ
	move.b	#IN_Z80DEBUGGER,_Z80intrFlag
	
	st.b	_traceFlag
	
	bset.b	#2,(SYSPORT_4)
	
	rte



*----------------------------------------------------------------------------------------------
* コピーキーインタラプト処理
* Z80デバッガの起動
COPYinterrupt:
	move.b	#IN_Z80DEBUGGER,_Z80INTREQ
	move.b	#IN_Z80DEBUGGER,_Z80intrFlag
	
	st.b	_traceFlag
	
	rte



_interrupt_inDebugger:	.dc.w	-1
_traceFlag:		.dc.w	0
_keywaitFlag:		.dc.w	-1

_Org_NMIintr:		.dc.l	0
_Org_COPYintr:		.dc.l	0
