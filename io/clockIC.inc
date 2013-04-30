;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;



_readClockIC:
	
	move.w	ClockIC_block(regZjmp),d0		* 12
	beq.s	@f					* 10/ 8 (T/F)
	
	*-- Block 1,2,3 読み込み
	lea	_ClockIC_memory(pc),a0			*  8
	adda.w	d0,a0					*  8
	adda.w	ClockIC_adr(regZjmp),a0			* 
	
	move.b	(a0),d1					*  8
;	ori.b	#%1111_0000,d1				*  8
	
	rts						* 16
	
@@:	*-- Block 0 読み込み
	*
	*	   +-------------+
	*	 0 | 秒（１の位）|
	*	   +-------------+
	*	 1 | 秒（10の位）|
	*	   +-------------+
	*	 2 | 分（１の位）|
	*	   +-------------+
	*	 3 | 分（10の位）|
	*	   +-------------+
	*	 4 | 時（１の位）|
	*	   +-------------+
	*	 5 | 時（10の位）|
	*	   +-------------+
	*	 6 | 曜日        |
	*	   +-------------+
	*	 7 | 日（１の位）|
	*	   +-------------+
	*	 8 | 日（10の位）|
	*	   +-------------+
	*	 9 | 月（１の位）|
	*	   +-------------+
	*	10 | 月（10の位）|
	*	   +-------------+
	*	11 | 年（１の位）|
	*	   +-------------+
	*	12 | 年（10の位）|
	*	   +-------------+
	
	move.w	ClockIC_adr(regZjmp),d0			* 12
	
	cmpi.w	#6,d0					* 
	bcc.s	@f					* 10/ 8 (T/F)
	
	*-- 時分秒
	cmpi.w	#5,d0					* 
	bne	1f					* 
	
	*-- 時（10の位）
	moveq.l	#$54,d0					*  4
	trap	#15					* 	_TIMEGET
	
	lsr.w	#8,d0					* 
	lsr.w	#4,d0					* 
	move.b	d0,d1					* 
	ori.b	#%1111_0000,d1				*  8
	
	rts						* 16
	
1:	*
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	move.w	d0,d1					*  4
	
	moveq.l	#$56,d0					*  4
	trap	#15					* 	_TIMEGET
	
	lsr.l	d1,d0					* 
	move.b	d0,d1					*  4
	ori.b	#%1111_0000,d1				*  8
	
	rts						* 16
	
@@:	
	cmpi.w	#6,d0					* 
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- 曜日
	moveq.l	#$54,d0					*  4
	trap	#15					* 	_DATEGET
	
	swap.w	d0					*  4
	lsr.w	#8,d0					* 
	move.b	d0,d1					*  4
	
	ori.b	#%1111_0000,d1				*  8
	
	rts						* 16
	
@@:	
	cmpi.w	#13,d0					* 
	bcc.s	@f					* 10/ 8 (T/F)
	
	*-- 年月日
	subi.w	#7,d0					* 
	add.w	d0,d0					* 
	add.w	d0,d0					* 
	move.w	d0,d1					* 
	
	moveq.l	#$54,d0					*  4
	trap	#15					* 	_DATEGET
	
	lsr.l	d1,d0					* 
	move.b	d0,d1					*  4
	ori.b	#%1111_0000,d1				*  8
	
	rts						* 16
	
@@:	
	cmpi.w	#13,d0					* 
	bne.s	@f					* 10/ 8 (T/F)
	
	*-- モードレジスタ
	move.w	ClockIC_modeReg(regZjmp),d1		* 12
	ori.b	#%1111_0000,d1				*  8
	
	rts						* 16
	
@@:	
	*-- 不正なアドレス
	move.b	#$ff,d1					* 
	
	rts						* 16


*----------------------------------------------------------------------------------------------
_ClockIC_memory:
	*--- BLOCK 0
	.dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	
	*--- BLOCK 1
	.dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	
	*--- BLOCK 2
	.dc.b	0	*  0: ID
	.dc.b	0	*  1: Adjust X (-8 ~ +7)
	.dc.b	0	*  2: Adjust Y (-8 ~ +7)
	.dc.b	0	*  3: Interlace Mode, Screen Mode
	.dc.b	0	*  4: WIDTHの値 (Lo)
	.dc.b	0	*  5: WIDTHの値 (Hi)
	.dc.b	15	*  6: 前景色
	.dc.b	4	*  7: 背景色
	.dc.b	4	*  8: 周辺色
	.dc.b	0	*  9: Cassette Speed, Printer Mode, Key Click, Key ON/OFF
	.dc.b	0	* 10: BEEP 音色, BEEP 音量
	.dc.b	0	* 11: タイトルカラー
	.dc.b	0	* 12: 国別コード
	.dc.b	0	* 13:
	.dc.b	0	* 14:
	.dc.b	0	* 15:
	
	*-- BLOCK 3
	.dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

