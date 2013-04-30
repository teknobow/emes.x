;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 teknobow
;


; d0.w ... proc offset

;----------------------------------------------------------------------------------------------
; write addr : regBC
; write data : regA

_RomWrite_BC_A:
	move.w	regBC,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regA,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regDE
; write data : regA

_RomWrite_DE_A:
	move.w	regDE,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regA,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regHL

_RomWrite_mn_HL:
	;-- MegaROM & SCC [L]
	move.w	(regZjmp,d0.w),d1
	move.b	regHL,d0
	
	move.w	regAdr,SAVE_regAdr(regZjmp)
	jsr	(regFtbl,d1.l)
	move.w	SAVE_regAdr(regZjmp),regAdr
	
	;-- 
	addq.w	#1,regAdr
	move.w	regAdr,d0
	clr.b	d0
	move.w	O_CHKROM(regZjmp,d0.w),d0
	beq	@f
	
	;-- MegaROM & SCC [H]
	move.w	(regZjmp,d0.w),d1
	move.w	regHL,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
@@:
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regA

_RomWrite_mn_A:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regA,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : (regPC)

_RomWrite_HL_n:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	(regPC)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regB

_RomWrite_HL_B:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.w	regBC,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regC

_RomWrite_HL_C:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regBC,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regD

_RomWrite_HL_D:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.w	regDE,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regE

_RomWrite_HL_E:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regDE,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regH

_RomWrite_HL_H:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.w	regHL,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regL

_RomWrite_HL_L:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM
	move.w	(regZjmp,d0.w),d1
	move.b	regHL,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regHL
; write data : regA

_RomWrite_HL_A:
	move.w	regHL,regAdr		; SCC Reg No.
	
	;-- MegaROM
	move.w	(regZjmp,d0.w),d1
	move.b	regA,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : (regIXY)

_RomWrite_mn_Ixy:
	;-- MegaROM & SCC [L]
	move.w	(regZjmp,d0.w),d1
	move.b	(regIXY)+,d0
	
	move.w	regAdr,SAVE_regAdr(regZjmp)
	move.b	(regIXY)+,SAVE_IXYh(regZjmp)
	jsr	(regFtbl,d0.w)
	move.w	SAVE_regAdr(regZjmp),regAdr
	
	;--
	addq.w	#1,regAdr
	move.w	regAdr,d0
	clr.b	d0
	move.w	O_CHKROM(regZjmp,d0.w),d0
	beq.s	@f
	
	;-- MegaROM [H]
	move.w	(regZjmp,d0.w),d1
	move.b	SAVE_IXYh(regZjmp),d0
	jsr	(regFtbl,d1.l)
@@:
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : (regPC)

_RomWrite_Ixy_n:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	(regPC)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regB

_RomWrite_Ixy_B:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.w	regBC,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regC

_RomWrite_Ixy_C:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regBC,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regD

_RomWrite_Ixy_D:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.w	regDE,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regE

_RomWrite_Ixy_E:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regDE,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regH

_RomWrite_Ixy_H:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.w	regHL,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regL

_RomWrite_Ixy_L:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regHL,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regA

_RomWrite_Ixy_A:
	;-- MegaROM & SCC
	move.w	(regZjmp,d0.w),d1
	move.b	regA,d0
	jsr	(regFtbl,d1.l)
	
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regBC

_RomWrite_mn_BC:
	;-- MegaROM & SCC [L]
	move.w	(regZjmp,d0.w),d1
	move.b	regBC,d0
	
	move.w	regAdr,SAVE_regAdr(regZjmp)
	jsr	(regFtbl,d1.w)
	move.w	SAVE_regAdr(regZjmp),regAdr
	
	;--
	addq.w	#1,regAdr
	move.w	regAdr,d0
	clr.b	d0
	move.w	O_CHKROM(regZjmp,d0.w),d0
	beq.s	@f
	
	;-- MegaROM & SCC[H]
	move.w	(regZjmp,d0.w),d1
	move.w	regBC,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
@@:
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regDE

_RomWrite_mn_DE:
	;-- MegaROM & SCC [L]
	move.w	(regZjmp,d0.w),d1
	move.b	regDE,d0
	
	move.w	regAdr,SAVE_regAdr(regZjmp)
	jsr	(regFtbl,d1.l)
	move.w	SAVE_regAdr(regZjmp),regAdr
	
	;--
	addq.w	#1,regAdr
	move.w	regAdr,d0
	clr.b	d0
	move.w	O_CHKROM(regZjmp,d0.w),d0
	beq.s	@f
	
	;-- MegaROM & SCC [H]
	move.w	(regZjmp,d0.w),d1
	move.w	regDE,-(sp)
	move.b	(sp)+,d0
	jsr	(regFtbl,d1.l)
@@:
	FETCH


;----------------------------------------------------------------------------------------------
; write addr : regAdr
; write data : regSP

_RomWrite_mn_SP:
	;-- MegaROM & SCC [L]
	move.w	(regZjmp,d0.w),d1
	move.w	regSP,-(sp)
	move.b	(sp)+,d0
	
	move.w	regAdr,SAVE_regAdr(regZjmp)
	jsr	(regFtbl,d1.l)
	move.w	SAVE_regAdr(regZjmp),regAdr
	
	;--
	addq.w	#1,regAdr
	move.w	regAdr,d0
	clr.b	d0
	move.w	O_CHKROM(regZjmp,d0.w),d0
	beq.s	@f
	
	;-- MegaROM & SCC [L]
	move.w	(regZjmp,d0.w),d1
	move.w	regSP,d0
	jsr	(regFtbl,d1.l)
@@:
	FETCH
