;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 teknobow
;


	SET_OFFSET_IO	JoyConvTable


;--
; d0.b ... data
_procPSGwrite_15:
	move.b	d0,PSG_reg15(regZjmp)		* 
	
	;-- LED [ÉJÉi]
	bmi.s	@f				* 10/ 8 (T/F)
	
	; ì_ìî
	bclr.b	#0,keyboard_LED(regFtbl)	* 16
	
	rts					* 16
	
@@:	; è¡ìî
	bset.b	#0,keyboard_LED(regFtbl)	* 16
	
	rts					* 16


;--
; d1.b ... data
; regFtbl ... inIO
_procPSGread_15:
	move.b	PSG_reg15(regZjmp),d1		* 
	
	rts					* 16


;--
; d1.b ... data
; regFtbl ... inIO
_procPSGread_14:
	moveq.l	#%0100_0000,d0			*  4
	and.b	PSG_reg15(regZjmp),d0		* 
	bne.s	@f
	
	;-- Joy 1
	move.b	(Joystick1),d0
	bra	1f
	
@@:	;-- Joy 2
	move.b	(Joystick2),d0
1:
	lea	JoyConvTable(regFtbl),a0
**	ext.w	d0
	move.b	(a0,d0.w),d1
	
	rts

