;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 teknobow
;

	.xdef	__changeCRTmode,_CRTmode

	.xref	TXT1_VDISP_interrupt
	.xref	TXT2_VDISP_interrupt
	.xref	GRA1_VDISP_interrupt
	.xref	GRA2_VDISP_interrupt
	.xref	GRA2_VDISP_interrupt_r
	.xref	GRA4_VDISP_interrupt
	.xref	GRA5_VDISP_interrupt
	.xref	GRA6_VDISP_interrupt
	.xref	GRA7_VDISP_interrupt

	.xref	GRA2_BL1_RASTER_interrupt



	SET_OFFSET_IO	CountVDISP
	SET_OFFSET_IO	cnt_CountVDISP

	SET_OFFSET_IO	CountUpdate
	SET_OFFSET_IO	cnt_CountUpdate

	SET_OFFSET_IO	VDPreg_1
	SET_OFFSET_IO	VDPreg_2
	SET_OFFSET_IO	VDPreg_3
	SET_OFFSET_IO	VDPreg_4
	SET_OFFSET_IO	VDPreg_5
	SET_OFFSET_IO	VDPreg_6
	SET_OFFSET_IO	VDPreg_7
	SET_OFFSET_IO	VDPreg_10
	SET_OFFSET_IO	VDPreg_11

	SET_OFFSET_IO	GRA2_backDropColor

	SET_OFFSET_IO	StatusReg_0

	SET_OFFSET_IO	ScreenRefresh
	SET_OFFSET_IO	PatNameTblUpdate
	SET_OFFSET_IO	PatGenTblUpdate

	SET_OFFSET_IO	SprPatGenTblUpdate
	SET_OFFSET_IO	SprAtrTblUpdate

	SET_OFFSET_IO	PatGenTbl
	SET_OFFSET_IO	PatGenTbl_HL

	SET_OFFSET_IO	PatNameTbl
	SET_OFFSET_IO	PatNameTbl_HL

	SET_OFFSET_IO	ColorTbl
	SET_OFFSET_IO	ColorTbl_HL

	SET_OFFSET_IO	SprPatGenTbl
	SET_OFFSET_IO	SprPatGenTbl_HL

	SET_OFFSET_IO	SprAtrTbl
	SET_OFFSET_IO	SprAtrTbl_HL

;;;	SET_OFFSET_IO	PatNameUpdateTbl
;;;	SET_OFFSET_IO	PatGenUpdateTbl

	SET_OFFSET_IO	paletteTbl

	SET_OFFSET_IO	inScrUpdate

*----------------------------------------------------------------------------------------------
*--- TEXT 1 screen initialize
TXT1_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	;-- VRAM 判別バイトのクリア
	jsr	_clearVRAM_BLOCK(pc)			* 18
	
	
	;--
	move.l	regVadr,d0				*  4
	
	
	;-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	;    +---+---+---+---+---+---+---+---+
	;  #4| 0 | 0 |A16|A15|A14|A13|A12|A11|
	;    +---+---+---+---+---+---+---+---+
	
	move.w	VDPreg_4(regFtbl),d0			* 12
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.l	d0,PatGenTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_LOW,PatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.l	d0,PatGenTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_HIGH,PatGenTbl_HL(regFtbl)	* 16
	
1:
	
	
	;-- パターンネームテーブル (1 Kbytes, 4 blocks)
	;    +---+---+---+---+---+---+---+---+
	;  #2| 0 |A16|A15|A14|A13|A12|A11|A10|
	;    +---+---+---+---+---+---+---+---+
	move.w	VDPreg_2(regFtbl),d0			* 12
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.l	d0,PatNameTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_LOW,PatNameTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10

@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.l	d0,PatNameTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_HIGH,PatNameTbl_HL(regFtbl)	* 16
	
1:
	
	;-- 画面モード設定
	moveq.l	#1,d0					*  4	high 384x240  16/16 512x512
	jsr	_changeCRTmode(pc)			* 
	
	;-- GR > TX > SP/BG
	;           SP TXGR G3G2 G1G0
	move.w	#%0010_0100_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp OFF, Txt ON, GR ON
	move.w	#%0000_0000_0010_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 OFF
	move.w	#%0000_0010_0000_0000,(BGcontrol)	* 
	
	;-- テキスト画面同時アクセス設定
	move.w	#%0000_0001_1111_0000,(CRTC_R21)	* 
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	;-- ボーダーカラー領域設定
	; 画面モード 384x240 16/16 512x512
	;
	;      0     32       352 384
	;     +-----+--------+-----+
	;    0|     |        |     |
	;     +-----+--------+-----+
	;   14|     |        |     |
	;     |     |        |     |
	;     |     |        |     |
	;     +-----+--------+-----+
	;  206|     |        |     |
	;     +-----+--------+-----+
	;  240
	
	lea	GVRAM_16_P0,a0				* 12
	move.l	#$00010001,d0				* 
	suba.l	a1,a1					* 
	
	;-- 上マスク
	move.w	#14-1,d1				* 
@@:
	move.w	#384/2-1,d2				* 
1:
	move.l	d0,(a0)+				* 
	dbra	d2,1b					* 
	lea	(512-384)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 左右マスク
	move.w	#192-1,d1				* 
@@:
	.rept 32/2
	move.l	d0,(a0)+				* 
	.endm
	
	move.w	#320/2/4-1,d2				* 
1:
	move.l	a1,(a0)+				* 
	move.l	a1,(a0)+				* 
	move.l	a1,(a0)+				* 
	move.l	a1,(a0)+				* 
	dbra	d2,1b					* 
	
	.rept 32/2
	move.l	d0,(a0)+
	.endm
	lea	(512-384)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 下マスク
	move.w	#34-1,d1				* 
@@:
	move.w	#384/2-1,d2				* 
1:
	move.l	d0,(a0)+				* 
	dbra	d2,1b					* 
	lea	(512-384)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;--
	move.w	#0,(GPALETTE+0)				* 
	
	
	;-- テキスト画面クリア
	jsr	_clearText(pc)				* 
	
	
	;-- テキストパレット初期化
	lea	TPALETTE,a0				* 
	lea	paletteTbl(regFtbl),a1			*  8
	
	; Text color
	move.b	VDPreg_7(regFtbl),d1			* 12
	move.w	#%1111_0000,d0				*  8
	and.w	d1,d0					*  4
	lsr.w	#3,d0					* 
	move.w	(a1,d0.w),2*15(a0)			* 
	
	; Back drop color
	andi.w	#%0000_1111,d1				*  8
	add.w	d1,d1					*  4
	move.w	(a1,d1.w),(a0)				* 
	move.w	(a1,d1.w),(GPALETTE+2)			* 
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	TXT1_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	TXT1_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	TXT1_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;-- VRAM_BLOCK 設定
	jmp	_TXT1_SetTable(pc)			* 10




*----------------------------------------------------------------------------------------------
*--- TEXT 2 screen initialize
TXT2_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	
	;-- VRAM 判別バイトのクリア
	jsr	_clearVRAM_BLOCK(pc)			* 18
	
	
	;--
	move.l	regVadr,d0				*  4
	
	
	;-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #4| 0 | 0 |A16|A15|A14|A13|A12|A11|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_4(regFtbl),d0			* 12
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.l	d0,PatGenTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_LOW,PatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.l	d0,PatGenTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_HIGH,PatGenTbl_HL(regFtbl)	* 16
	
1:
	
	
	;-- パターンネームテーブル (4 Kbytes, 16 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #2| 0 |A16|A15|A14|A13|A12|   |   |
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_2(regFtbl),d0			* 12
	andi.w	#%0111_1100_0000_0000,d0		*  8
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.l	d0,PatNameTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_LOW,PatNameTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.l	d0,PatNameTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_HIGH,PatNameTbl_HL(regFtbl)	* 16
1:
	
	
	;-- カラーテーブル (512 bytes, 2 blocks)
	
	
	;-- 画面モード設定
	moveq.l	#2,d0					*  4	high 640x231  16/16 1024x1024
	jsr	_changeCRTmode(pc)			* 
	
	;-- GR > TX > SP/BG
	;           SP TXGR G3G2 G1G0
	move.w	#%0010_0100_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp OFF, Txt ON, GR ON
	move.w	#%0000_0000_0011_0000,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 OFF
	move.w	#%0000_0010_0000_0000,(BGcontrol)	* 
	
	;-- テキスト画面同時アクセス設定
	move.w	#%0000_0001_1111_0000,(CRTC_R21)	* 
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	
	;-- ボーダーカラー領域設定
	; 画面モード 640x231 16/16 1024x1024
	;
	;      0                  640
	;     +------------------+
	;    0|                  |
	;     |                  |
	;     |                  |
	;     |                  |
	;     +------------------+
	;  192|                  |
	;     +------------------+
	;  231
	
	lea	GVRAM_16_P0,a0				* 12
	
	;-- マスク
	moveq.l	#0,d0					*  4
	move.w	#192-1,d1				* 
@@:
	move.w	#640/2/4-1,d2				* 
1:
	move.l	d0,(a0)+				* 
	move.l	d0,(a0)+				* 
	move.l	d0,(a0)+				* 
	move.l	d0,(a0)+				* 
	dbra	d2,1b					* 
	
	lea	(1024-640)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;--
	move.l	#$00010001,d0				* 
	move.w	#39-1,d1				* 
@@:
	move.w	#640/2/4-1,d2				* 
1:
	move.l	d0,(a0)+				* 
	move.l	d0,(a0)+				* 
	move.l	d0,(a0)+				* 
	move.l	d0,(a0)+				* 
	dbra	d2,1b					* 
	
	lea	(1024-640)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 
	move.w	#0,(GPALETTE+0)				* 
	move.w	#1,(GPALETTE+2)				* 
	
	;-- テキスト画面クリア
	jsr	_clearText(pc)				* 
	
	
	;-- テキストパレット初期化
	lea	TPALETTE,a0				* 
	lea	paletteTbl(regFtbl),a1			*  8
	
	; Text color
	move.b	VDPreg_7(regFtbl),d1			* 12
	move.w	#%1111_0000,d0				*  8
	and.w	d1,d0					*  4
	lsr.w	#3,d0					* 
	move.w	(a1,d0.w),2*15(a0)			* 
	
	; Back drop color
	andi.w	#%0000_1111,d1				*  8
	add.w	d1,d1					*  4
	move.w	(a1,d1.w),(a0)				* 
	
	
	*-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	TXT2_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	TXT2_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	TXT2_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;-- VRAM_BLOCK 設定
	jmp	_TXT2_SetTable(pc)			* 10





*----------------------------------------------------------------------------------------------
*--- MULTI COLOR screen initialize
MLT_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	
	;-- VRAM 判別バイトのクリア
	jsr	_clearVRAM_BLOCK(pc)			* 18
	
	
	;--
	move.l	regVadr,d0				*  4
	
	
	;-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #4| 0 | 0 |A16|A15|A14|A13|A12|A11|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_4(regFtbl),d0			* 12
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.l	d0,PatGenTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_LOW,PatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.l	d0,PatGenTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_HIGH,PatGenTbl_HL(regFtbl)	* 16
	
1:
	
	
	;-- パターンネームテーブル (1 Kbytes, 4 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #2| 0 |A16|A15|A14|A13|A12|A11|A10|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_2(regFtbl),d0			* 12
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.l	d0,PatNameTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_LOW,PatNameTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10

@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.l	d0,PatNameTbl(regFtbl)			* 16
	move.b	#MARK_VRAM_HIGH,PatNameTbl_HL(regFtbl)	* 16
1:
	
	
	*-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	MLT_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	MLT_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
;!!	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
;	move.w	CountUpdate(regFtbl),d0			* 12
;	cmpi.w	#1,d0					* 
;	beq.s	@f					* 
;	move.w	d0,d1					; 
;	asr.w	#1,d1					; 
;	add.w	d1,d0					* 
;@@:
;	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
;!!	lea	MLT_VDISP_interrupt,a1			* 12
;	moveq.l	#1,d1					*  4
;	moveq.l	#$6c,d0					*  4
;	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	jmp	_MLT_SetTable(pc)			* 10



*----------------------------------------------------------------------------------------------
*--- GRAPHIC 1 screen initialize
GRA1_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	;-- VRAM 判別バイトのクリア
	jsr	_clearVRAM_BLOCK(pc)			* 18
	
	
	;--
	move.l	regVadr,d0				*  4
	
	
	;-- パターンジェネレータテーブル (2 K bytes, 8 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #4| 0 | 0 |A16|A15|A14|A13|A12|A11|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_4(regFtbl),d0			* 12
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,PatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,PatGenTbl_HL(regFtbl)	* 16
1:
	move.l	d0,PatGenTbl(regFtbl)			* 16
	
	
	;-- パターンネームテーブル (1 Kbytes, 4 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #2| 0 |A16|A15|A14|A13|A12|A11|A10|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_2(regFtbl),d0			* 12
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,PatNameTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,PatNameTbl_HL(regFtbl)	* 16
1:
	move.l	d0,PatNameTbl(regFtbl)			* 16
	
	
	;-- カラーテーブル (64 bytes, 1/4 blocks)
	;   +---+---+---+---+---+---+---+---+
	;#10| 0 | 0 | 0 | 0 | 0 |A16|A15|A14|
	;   +---+---+---+---+---+---+---+---+
	;
	;   +---+---+---+---+---+---+---+---+
	; #3|A13|A12|A11|A10| A9| A8| A7| A6|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_10(regFtbl),d0			* 12
	move.b	VDPreg_3(regFtbl),d0			* 12
	lsl.w	#6,d0					* 18
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,ColorTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,ColorTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,ColorTbl(regFtbl)			* 16
	
	
	*-- スプライトアトリビュートテーブル(mode 1) 64 bytes (1/4 blocks)
	;   +---+---+---+---+---+---+---+---+
	;#11| 0 | 0 | 0 | 0 | 0 | 0 |A16|A15|
	;   +---+---+---+---+---+---+---+---+
	;
	;   +---+---+---+---+---+---+---+---+
	; #5|A14|A13|A12|A11|A10| A9| A8| A7|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_11(regFtbl),d0			* 12
	move.b	VDPreg_5(regFtbl),d0			* 12
	lsl.w	#7,d0					* 20
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,SprAtrTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,SprAtrTbl_HL(regFtbl)	* 16
1:
	move.l	d0,SprAtrTbl(regFtbl)			* 16
	
	
	*-- スプライトパターンジェネレータテーブル
	;   +---+---+---+---+---+---+---+---+
	; #6| 0 | 0 |A16|A15|A14|A13|A12|A11|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_6(regFtbl),d0			* 12
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,SprPatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,SprPatGenTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,SprPatGenTbl(regFtbl)		* 16
	
	
	;--
	moveq.l	#0,d0					*  4	high 320x248  16/16 512x512
	jsr	_changeCRTmode(pc)			* 
	
	;-- スプライト初期化
	moveq.l	#$c0,d0					*  4
	trap	#15					* 	IOCS	_SP_INIT
	
	moveq.l	#$c1,d0					*  4
	trap	#15					* 	IOCS	_SP_ON
	
	;--  TX > GR > SP/BG
	;           SP TXGR G3G2 G1G0
	move.w	#%0010_0001_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 ON
	move.w	#%0000_0010_0000_0001,BGcontrol		* 
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	;-- ボーダーカラー領域設定
	; 画面モード 320x248 65536/16 512x512
	;
	;      0     32       288 319
	;     +-----+--------+-----+
	;    0|     |        |     |
	;     +-----+--------+-----+
	;   24|     |        |     |
	;     |     |        |     |
	;     |     |        |     |
	;     +-----+--------+-----+
	;  216|     |        |     |
	;     +-----+--------+-----+
	;  248
	
	lea	GVRAM_16_P0,a0				* 12
	move.l	#$00010001,d0				* 
	suba.l	a1,a1					* 
	
	;-- 上マスク
	move.w	#24-1,d1				* 
@@:
	move.w	#320/2-1,d2				* 
1:
	move.l	d0,(a0)+				* 
	dbra	d2,1b					* 
	lea	(512-320)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 左右マスク
	move.w	#192-1,d1				* 
@@:
	.rept 32/2
	move.l	d0,(a0)+				* 
	.endm
	
	move.w	#256/2/4-1,d2				* 
1:
	move.l	a1,(a0)+				* 
	move.l	a1,(a0)+				* 
	move.l	a1,(a0)+				* 
	move.l	a1,(a0)+				* 
	dbra	d2,1b					* 
	
	.rept 32/2
	move.l	d0,(a0)+
	.endm
	lea	(512-320)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 下マスク
	move.w	#32-1,d1				* 
@@:
	move.w	#320/2-1,d2				* 
1:
	move.l	d0,(a0)+				* 
	dbra	d2,1b					* 
	lea	(512-320)*2(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 
	lea	paletteTbl(regFtbl),a0			*  8
	
	*-- Back drop color
	moveq.l	#%0000_1111,d1				*  4
	and.w	d0,d1					*  4
	add.w	d1,d1					*  4
	move.w	(a0,d1.w),d0				* 
	
	move.w	d0,(TPALETTE)				* 16
	move.w	#0,(GPALETTE+0)				* 
	move.w	d0,(GPALETTE+2)				* 16
	
	
	;--
	jsr	_clearText(pc)				* 
	
	
	;-- カラーテーブルアップデート
	movea.l	ColorTbl(regFtbl),a0			* 16
	lea	paletteTbl(regFtbl),a1			*  8
	
	move.l	a2,-(sp)				* 12
	lea	TPALETTE,a2				* 12
	
	moveq.l	#32-1,d1				*  4
@@:
	move.b	(a0)+,d2				*  8
	
	*-- パターンの０の部分の色コード
	moveq.l	#%0000_1111,d0				*  4
	and.w	d2,d0					*  4
	add.w	d0,d0					*  4
	move.w	(a1,d0.w),(a2)+				* 
	
	*-- パターンの１の部分の色コード
	andi.w	#%0000_0000_1111_0000,d2		*  8
	lsr.w	#3,d2					* 12
	move.w	(a1,d2.w),(a2)+				* 
	
	dbra	d1,@b					* 10/14 (T/F)
	
	move.l	(sp)+,a2				* 12
	
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA1_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA1_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	GRA1_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;--
	jmp	_GRA1_SetTable(pc)			* 10



*----------------------------------------------------------------------------------------------
*--- GRAPHIC 2 screen initialize
GRA2_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	;-- VRAM 判別バイトのクリア
	jsr	_clearVRAM_BLOCK(pc)			* 18
	
	
	;--
	move.l	regVadr,d0				*  4
	
	
	;-- パターンジェネレータテーブル (8 Kbytes, 32 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #4| 0 | 0 |A16|A15|A14|A13|   |   |
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_4(regFtbl),d0			* 12
	andi.w	#%0011_1100_0000_0000,d0		*  8
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,PatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,PatGenTbl_HL(regFtbl)	* 16
1:
	move.l	d0,PatGenTbl(regFtbl)			* 16
	
	
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #2| 0 |A16|A15|A14|A13|A12|A11|A10|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_2(regFtbl),d0			* 12
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,PatNameTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,PatNameTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,PatNameTbl(regFtbl)			* 16
	
	
	*-- カラーテーブル (8 Kbytes, 32 blocks)
	;   +---+---+---+---+---+---+---+---+
	;#11| 0 | 0 | 0 | 0 | 0 |A16|A15|A14|
	;   +---+---+---+---+---+---+---+---+
	;
	;   +---+---+---+---+---+---+---+---+
	; #3|A13|   |   |   |   |   |   |   |
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_11(regFtbl),d0			* 12
	move.b	VDPreg_3(regFtbl),d0			* 12
	andi.w	#%0000_0111_1000_0000,d0		*  8
	lsl.w	#6,d0					* 18
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,ColorTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,ColorTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,ColorTbl(regFtbl)			* 16
	
	
	*-- スプライトアトリビュートテーブル(mode 1) 64 bytes (1/4 blocks)
	;   +---+---+---+---+---+---+---+---+
	;#11| 0 | 0 | 0 | 0 | 0 | 0 |A16|A15|
	;   +---+---+---+---+---+---+---+---+
	;
	;   +---+---+---+---+---+---+---+---+
	; #5|A14|A13|A12|A11|A10| A9| A8| A7|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_11(regFtbl),d0			* 12
	move.b	VDPreg_5(regFtbl),d0			* 12
	lsl.w	#7,d0					* 20
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,SprAtrTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,SprAtrTbl_HL(regFtbl)	* 16
1:
	move.l	d0,SprAtrTbl(regFtbl)			* 16
	
	
	*-- スプライトパターンジェネレータテーブル
	;   +---+---+---+---+---+---+---+---+
	; #6| 0 | 0 |A16|A15|A14|A13|A12|A11|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_6(regFtbl),d0			* 12
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,SprPatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,SprPatGenTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,SprPatGenTbl(regFtbl)		* 16
	
	
	;--
.ifndef GRA2_LOWRESO
	moveq.l	#3,d0					*  4	high 320x248 65536/16 512x512
.else
	moveq.l	#4,d0					*  4	high 320x248 65536/16 512x512
.endif
	jsr	_changeCRTmode(pc)			* 
	
	;-- スプライト初期化
	moveq.l	#$c0,d0					*  4
	trap	#15					* 	IOCS	_SP_INIT
	
	moveq.l	#$c1,d0					*  4
	trap	#15					* 	IOCS	_SP_ON
	
	;-- SP/BG > TX > GR
	;           SP TXGR G3G2 G1G0
;	move.w	#%0000_0110_1110_0100,(VideoCtrl_R1)	* 
	;-- TX > SP/BG > GR
	;           SP TXGR G3G2 G1G0
	move.w	#%0001_0010_1110_0100,(VideoCtrl_R1)	* 
	;-- SP/BG > GR > TX
	;           SP TXGR G3G2 G1G0
;	move.w	#%0000_1001_1110_0100,(VideoCtrl_R1)	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_1111,(VideoCtrl_R2)	* 
	
	;-- Disp ON, BG0 ON
	move.w	#%0000_0010_0000_0001,(BGcontrol)	* 
	
	;-- テキスト画面同時アクセス設定
	move.w	#%0000_0001_1111_0000,(CRTC_R21)	* 
	
	;--
	jsr	_clearText(pc)				* 
	
	;-- グラフィック消去
	jsr	_clearGraphic_P0(pc)			* 
	
	;-- スクロールレジスタ設定
	move.w	#-6,(GScroll_X0)			* 
	move.w	#-4,(GScroll_X1)			* 
	move.w	#-2,(GScroll_X2)			* 
	move.w	#0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	
	;-- ボーダーカラー領域設定
	; 画面モード 320x248 65536/16 512x512
	;
	;      0     32       288 319
	;     +-----+--------+-----+
	;    0|     |        |     |
	;     +-----+--------+-----+
	;   24|     |        |     |
	;     |     |        |     |
	;     |     |        |     |
	;     +-----+--------+-----+
	;  216|     |        |     |
	;     +-----+--------+-----+
	;  248
	
	;-- テキスト画面同時アクセス設定
	move.w	#%0000_0001_1111_0000,(CRTC_R21)	* 
	
	moveq.l	#-1,d0					* 
	
	;-- 上マスク
	lea	TVRAM,a0				* 
	moveq.l	#24-1,d1				* 
@@:
	.rept (320)/32
	move.l	d0,(a0)+				*  
	.endm
	lea	(1024-(320))/8(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 左右マスク
	move.w	#192-1,d1				* 
@@:
	move.l	d0,(a0)+				* 
	lea	256/8(a0),a0				* 
	move.l	d0,(a0)+				* 
	lea	(1024-(320))/8(a0),a0			* 
	dbra	d1,@b					* 
	
	;-- 下マスク
	moveq.l	#32-1,d1				* 
@@:
	.rept (320)/32
	move.l	d0,(a0)+				*  
	.endm
	lea	(1024-(320))/8(a0),a0			* 
	dbra	d1,@b					* 
	
	
	;-- テキストパレット初期化
	lea	TPALETTE,a0				* 12
	lea	paletteTbl(regFtbl),a1			*  8
	
	move.l	(a1),(a0)				* 
	lea	16*2(a0),a0				* 12
	
	moveq.l	#16-1,d1				*  4
@@:
	move.w	(a1)+,(a0)+				* 
	dbra	d1,@b					* 
	
	lea	TPALETTE+32+2,a0			* 
	lea	paletteTbl+2(regFtbl),a1		* 
	moveq.l	#15-1,d1				*  4
@@:
	move.w	(a1)+,(a0)				* 12
	lea	32(a0),a0				*  8
	dbra	d1,@b					* 
	
	
	lea	GPALETTE,a0				* 12
	lea	paletteTbl(regFtbl),a1			*  8
	
	moveq.l	#16-1,d1				*  4
@@:
	move.w	(a1)+,(a0)+				* 
	dbra	d1,@b					* 
	
	;-- バックドロップカラーの設定
	lea	paletteTbl(regFtbl),a0			*  8
	move.b	VDPreg_7(regFtbl),d0			* 12
	andi.w	#%0000_0000_0000_1111,d0		*  8
	add.w	d0,d0					*  4
	move.w	(a0,d0.w),d0				* 
	
	;-- GRAPHIC2の場合、背景色は割り込みで設定する
	move.w	d0,GRA2_backDropColor(regFtbl)		* 
	
	;-- 周辺色
	move.w	d0,(TPALETTE+15*2)			* 
	
	;-- ブロック 1 バックドロップカラー
	move.w	d0,(TPALETTE+16*2)			* 
	
	;-- ブロック 2,3 バックドロップカラー
	move.w	d0,(GPALETTE)				* 16
	
	;-- TEXT Palette 0 をクリア
;;;	move.w	#0,(TPALETTE)				* 
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA2_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA2_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
;	lea	GRA2_VDISP_interrupt,a1			* 12
	lea	GRA2_VDISP_interrupt_r,a1		* 12

	suba.l	a1,a1					* 


	moveq.l	#1,d1					*  4
.if 0 *DEBUG
	ori.w	#$100,d1
.endif
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込み設定
	lea	GRA2_BL1_RASTER_interrupt,a1		* 12
.ifndef GRA2_LOWRESO
	move.w	#$13+(24+0)*2+1,d1			*  8
.else
	move.w	#$11+(24+0)+1,d1			*  8
.endif
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	;-- USP 設定
	lea.l	PCGarea,a0				* 12
	move.l	a0,usp					*  4
	
	;--
	jmp	_GRA2_SetTable(pc)			* 10





*----------------------------------------------------------------------------------------------
*--- GRAPHIC 3 screen initialize
GRA3_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	;-- VRAM 判別バイトのクリア
	jsr	_clearVRAM_BLOCK(pc)			* 18
	
	
	;--
	move.l	regVadr,d0				*  4
	
	
	;-- パターンジェネレータテーブル (8 Kbytes, 32 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #4| 0 | 0 |A16|A15|A14|A13|   |   |
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_4(regFtbl),d0			* 12
	andi.w	#%0011_1100_0000_0000,d0		*  8
	lsl.w	#3,d0					* 12
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,PatGenTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,PatGenTbl_HL(regFtbl)	* 16
1:
	move.l	d0,PatGenTbl(regFtbl)			* 16
	
	
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	;   +---+---+---+---+---+---+---+---+
	; #2| 0 |A16|A15|A14|A13|A12|A11|A10|
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_2(regFtbl),d0			* 12
	add.w	d0,d0					*  4
	add.w	d0,d0					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM_LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,PatNameTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,PatNameTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,PatNameTbl(regFtbl)			* 16
	
	
	*-- カラーテーブル (8 Kbytes, 32 blocks)
	;   +---+---+---+---+---+---+---+---+
	;#11| 0 | 0 | 0 | 0 | 0 |A16|A15|A14|
	;   +---+---+---+---+---+---+---+---+
	;
	;   +---+---+---+---+---+---+---+---+
	; #3|A13|   |   |   |   |   |   |   |
	;   +---+---+---+---+---+---+---+---+
	move.w	VDPreg_11(regFtbl),d0			* 12
	move.b	VDPreg_3(regFtbl),d0			* 12
	andi.w	#%0000_0111_1000_0000,d0		*  8
	lsl.w	#6,d0					* 18
	bcs.s	@f					* 10/ 8 (T/F)
	
	;-- VRAM LOW
	bclr.l	#16,d0					*<14
	move.b	#MARK_VRAM_LOW,ColorTbl_HL(regFtbl)	* 16
	
	bra	1f					* 10
	
@@:
	;-- VRAM_HIGH
	bset.l	#16,d0					*<12
	move.b	#MARK_VRAM_HIGH,ColorTbl_HL(regFtbl)	* 16
1:	
	move.l	d0,ColorTbl(regFtbl)			* 16
	
	
	;--
	moveq.l	#0,d0					*  4	high 320x248  16/16 512x512
	jsr	_changeCRTmode(pc)			* 
	
	;-- TX > GR > SP > BG
	;           SP TXGR G3G2 G1G0
	move.w	#%0010_0001_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 ON
	move.w	#%0000_0010_0000_0001,BGcontrol		* 
	
	;--
	jsr	_clearText(pc)				* 
	
	;-- グラフィック消去
	jsr	_clearGraphic_P0(pc)			* 
	
	
	;-- ボーダーカラー領域設定
	lea	GRA3_border_U(pc),a1			* 
	moveq.l	#$ba,d0					*  4
	trap	#15					* 	IOCS	_FILL
	
	lea	GRA3_border_R(pc),a1			* 
	moveq.l	#$ba,d0					*  4
	trap	#15					* 	IOCS	_FILL
	
	lea	GRA3_border_L(pc),a1			* 
	moveq.l	#$ba,d0					*  4
	trap	#15					* 	IOCS	_FILL
	
	lea	GRA3_border_D(pc),a1			* 
	moveq.l	#$ba,d0					*  4
	trap	#15					* 	IOCS	_FILL
	
	move.w	#1,(GPALETTE+2)				* 
	
	;-- テキストパレット初期化
	lea	TPALETTE,a0				* 
	lea	paletteTbl(regFtbl),a1			*  8
	
	moveq.l	#16-1,d1				*  4
@@:
	move.w	(a1)+,(a0)+				* 12
	dbra	d1,@b					* 
	
	lea	TPALETTE+32+2,a0			* 
	lea	paletteTbl+2(regFtbl),a1		* 
	moveq.l	#15-1,d1				*  4
@@:
	move.w	(a1)+,(a0)				* 12
	lea	32(a0),a0				*  8
	dbra	d1,@b					* 
	
	
	*-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA3_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA3_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
*!!	lea	GRA3_VDISP_interrupt,a1			* 12
	lea	GRA2_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;--
	jmp	_GRA2_SetTable(pc)			* 10
*	jmp	_GRA3_SetTable(pc)			* 10


;320x248  16/16 512x512
;(320-256)/2 = 32
;(248-212)/2 = 18 + 20
GRA3_border_U:
	.dc.w	0,0
	.dc.w	319,17+6
	.dc.w	1

GRA3_border_L:
	.dc.w	0,18+6
	.dc.w	31,248-18-20-1+6
	.dc.w	1

GRA3_border_R:
	.dc.w	320-32,18+6
	.dc.w	319,248-18-20-1+6
	.dc.w	1

GRA3_border_D:
	.dc.w	0,248-18-20+6
	.dc.w	319,247
	.dc.w	1



*----------------------------------------------------------------------------------------------
*--- GRAPHIC 4 screen initialize
GRA4_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	
	;-- パターンネームテーブル (32 Kbytes, 128 blocks)
	lea	VRAM_BLOCK_LOW(regZjmp),a0		*  8
	lea	$8000(a0),a0				*  8
	
	move.w	#VDP_GRA4_PATNAME_P1*256,d0		*  8
	swap.w	d0					*  4
	move.w	#VDP_GRA4_PATNAME_P3*256,d0		*  8
	moveq.l	#128-1,d1				*  4
@@:
	move.l	d0,(a0)					*  8
	lea	$100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	move.w	#VDP_GRA4_PATNAME_P0*256,d0		*  8
	swap.w	d0					*  4
	move.w	#VDP_GRA4_PATNAME_P2*256,d0		*  8
	moveq.l	#128-1,d1				*  4
@@:
	move.l	d0,(a0)					*  8
	lea	$100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	
	;--
	moveq.l	#$06,d1					*  4	high 256*256  16/16 512*512
**	moveq.l	#$34,d1					*  4	high 384x256  16/16 512x512
***	moveq.l	#$44,d1					*  4	mid  256x212  16/16 512x512
	moveq.l	#$10,d0					*  4
	trap	#15					* 	IOCS	_CRTMOD
	
	moveq.l	#0,d1					*  4
	
	;-- グラフィック消去
;;;	jsr	_clearGraphic_P0(pc)			* 
	
	moveq.l	#$90,d0					*  4
	trap	#15					* 	IOCS _G_CLR_ON
	
	;-- TX > SP > BG >  GR
	;           SP TXGR G3G2 G1G0
	move.w	#%0001_0010_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 OFF
	move.w	#%0000_0010_0000_0000,(BGcontrol)	* 
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA4_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA4_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	GRA4_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;-- VRAM書き込みアドレスの設定
	move.l	regVadr,d0				*  4
	move.b	(regIO),d0				*  8
	move.w	VRAM_BLOCK(regZjmp,d0.w),d1		* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14




*----------------------------------------------------------------------------------------------
*--- GRAPHIC 5 screen initialize
GRA5_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	
	*-- パターンネームテーブル (32 Kbytes, 128 blocks)
	lea	VRAM_BLOCK_LOW(regZjmp),a0		*  8
	lea	$8000(a0),a0				*  8
	
	move.w	#VDP_GRA5_PATNAME_P1*256,d0		*  8
	swap.w	d0					*  4
	move.w	#VDP_GRA5_PATNAME_P3*256,d0		*  8
	moveq.l	#128-1,d1				*  4
@@:
	move.l	d0,(a0)					*  8
	lea	$100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	move.w	#VDP_GRA5_PATNAME_P0*256,d0		*  8
	swap.w	d0					*  4
	move.w	#VDP_GRA5_PATNAME_P2*256,d0		*  8
	moveq.l	#128-1,d1				*  4
@@:
	move.l	d0,(a0)					*  8
	lea	$100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	
	;--
	moveq.l	#$04,d1					*  4	high 512*512  16/16 512*512
**	moveq.l	#$24,d1					*  4	high 512x256  16/16 512x512
***	moveq.l	#$45,d1					*  4	mid  512x212  16/16 512x512
	moveq.l	#$10,d0					*  4
	trap	#15					* 	IOCS	_CRTMOD
	
	moveq.l	#0,d1					*  4
	
	;-- グラフィック消去
;	jsr	_clearGraphic_P0(pc)			* 
	
	moveq.l	#$90,d0					*  4
	trap	#15					* 	IOCS _G_CLR_ON
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	;-- TX > SP > BG >  GR
	;           SP TXGR G3G2 G1G0
	move.w	#%0001_0010_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 OFF
	move.w	#%0000_0010_0000_0000,(BGcontrol)	* 
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA5_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA5_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	GRA5_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;-- VRAM書き込みアドレスの設定
	move.l	regVadr,d0				*  4
	move.b	(regIO),d0				*  8
	move.w	VRAM_BLOCK(regZjmp,d0.w),d1		* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14




;----------------------------------------------------------------------------------------------
;--- GRAPHIC 6 screen initialize
GRA6_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	
	;-- パターンネームテーブル (64 Kbytes, 256 blocks)
	lea	VRAM_BLOCK_LOW(regZjmp),a0		*  8
	lea	$8000(a0),a0				*  8
	
	move.w	#VDP_GRA6_PATNAME_P0*256,d0		*  8
	swap.w	d0					*  4
	move.w	#VDP_GRA6_PATNAME_P1*256,d0		*  8
	move.w	#256-1,d1				*  4
@@:
	move.l	d0,(a0)					*  8
	lea	$100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	
	;--
	moveq.l	#$04,d1					*  4	high 512*512  16/16 512*512
**	moveq.l	#$24,d1					*  4	high 512x256  16/16 512x512
***	moveq.l	#$45,d1					*  4	mid  512x212  16/16 512x512
	moveq.l	#$10,d0					*  4
	trap	#15					* 	IOCS	_CRTMOD
	
	moveq.l	#0,d1					*  4
	
	;-- グラフィック消去
;	jsr	_clearGraphic_P0(pc)			* 
	
	moveq.l	#$90,d0					*  4
	trap	#15					* 	IOCS _G_CLR_ON
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	;-- TX > SP > BG >  GR
	;           SP TXGR G3G2 G1G0
	move.w	#%0001_0010_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 OFF
	move.w	#%0000_0010_0000_0000,(BGcontrol)	* 
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA6_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA6_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	GRA6_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;-- VRAM書き込みアドレスの設定
	move.l	regVadr,d0				*  4
	move.b	(regIO),d0				*  8
	move.w	VRAM_BLOCK(regZjmp,d0.w),d1		* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14



;----------------------------------------------------------------------------------------------
;--- GRAPHIC 7 screen initialize
GRA7_ScreenInitialize:
	
	;-- VDISP 割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	;-- ラスター割り込みルーチン解除
	suba.l	a1,a1					*  8
	moveq.l	#$6d,d0					*  4
	trap	#15					* 	IOCS	_CRTCRAS
	
	
	;-- パターンネームテーブル (64 Kbytes, 256 blocks)
	lea	VRAM_BLOCK_LOW(regZjmp),a0		*  8
	lea	$8000(a0),a0				*  8
	
	move.w	#VDP_GRA7_PATNAME_P0*256,d0		*  8
	swap.w	d0					*  4
	move.w	#VDP_GRA7_PATNAME_P1*256,d0		*  8
	move.w	#256-1,d1				*  4
@@:
	move.l	d0,(a0)					*  8
	lea	$100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	
	;--
	moveq.l	#$0A,d1					*  4	high 256*256  16/256 512*512
**	moveq.l	#$38,d1					*  4	high 384x256  16/256 512x512
***	moveq.l	#$48,d1					*  4	mid  256x212  16/256 512x512
	moveq.l	#$10,d0					*  4
	trap	#15					* 	IOCS	_CRTMOD
	
	moveq.l	#0,d1					*  4
	
	;-- グラフィック消去
;	jsr	_clearGraphic_P0(pc)			* 
	
	moveq.l	#$90,d0					*  4
	trap	#15					* 	IOCS _G_CLR_ON
	
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_X3)				* 
	
	move.w	#0,(TScroll_X)				* 
	
	;-- TX > SP > BG >  GR
	;           SP TXGR G3G2 G1G0
	move.w	#%0001_0010_1110_0100,VideoCtrl_R1	* 
	
	;-- Sp ON, Txt ON, GR ON
	move.w	#%0000_0000_0110_0001,VideoCtrl_R2	* 
	
	;-- Disp ON, BG0 OFF
	move.w	#%0000_0010_0000_0000,(BGcontrol)	* 
	
	
	;-- I/O ルーチン
	move.l	regVadr,d0				*  4
	btst.l	#16,d0					* 10
	bne.s	@f					* 10/ 8 (T/F)
	
	lea	GRA7_L_IOroutine,regIO			* 12
	bra	1f					* 10
@@:
	lea	GRA7_H_IOroutine,regIO			* 12
1:
	
	
	;-- 画面再表示フラグ
	move.b	#0,ScreenRefresh(regFtbl)		* 16
	
	
	;--
	clr.b	inScrUpdate(regFtbl)			* 
	
	;-- VDISP割り込みルーチン設定
	move.w	CountVDISP(regFtbl),cnt_CountVDISP(regFtbl)	* 20
	move.w	CountUpdate(regFtbl),d0			* 12
	cmpi.w	#1,d0					* 
	beq.s	@f					* 
	move.w	d0,d1					; 
	asr.w	#1,d1					; 
	add.w	d1,d0					* 
@@:
	move.w	d0,cnt_CountUpdate(regFtbl)		* 20
	lea	GRA7_VDISP_interrupt,a1			* 12
	moveq.l	#1,d1					*  4
	moveq.l	#$6c,d0					*  4
	trap	#15					* 	IOCS	_VDISPST				* 
	
	
	;-- VRAM書き込みアドレスの設定
	move.l	regVadr,d0				*  4
	move.b	(regIO),d0				*  8
	move.w	VRAM_BLOCK(regZjmp,d0.w),d1		* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14



*----------------------------------------------------------------------------------------------
* テキスト画面のクリア

_clearText:
	movem.l	d2-d7,-(sp)				* 
	
	move.w	(CRTC_R21),-(sp)			* 
	
	*-- テキスト画面同時アクセス設定
	move.w	#%0000_0001_1111_0000,(CRTC_R21)	* 
	
	lea	TVRAM+1024/8*512,a0			* 
	
	moveq.l	#0,d0					*  4
	move.l	d0,d1					*  4
	move.l	d0,d2					*  4
	move.l	d0,d3					*  4
	move.l	d0,d4					*  4
	move.l	d0,d5					*  4
	move.l	d0,d6					*  4
	move.l	d0,a1					*  4
	
	move.w	#(1024/8*512)/(4*8)-1,d7		* 
@@:	movem.l	d0-d6/a1,-(a0)				* 
	dbra	d7,@b					* 10/14 (T/F)
	
	move.w	(sp)+,(CRTC_R21)			* 
	
	movem.l	(sp)+,d2-d7				* 
	
	rts						* 16


;-- グラフィック消去
_clearGraphic_P0:
	movem.l	d2-d7,-(sp)				* 
	
	lea	GVRAM_16_P0+512*2*512,a0		* 
	
	moveq.l	#0,d0					*  4
	move.l	d0,d1					*  4
	move.l	d0,d2					*  4
	move.l	d0,d3					*  4
	move.l	d0,d4					*  4
	move.l	d0,d5					*  4
	move.l	d0,d6					*  4
	move.l	d0,a1					*  4
	
	move.w	#(512*2*512)/(4*8)-1,d7			* 
@@:	movem.l	d0-d6/a1,-(a0)				* 
	dbra	d7,@b					* 10/14 (T/F)
	
	movem.l	(sp)+,d2-d7				* 
	
	rts						* 16



*----------------------------------------------------------------------------------------------
* 画面モード切り替え
*
*	d0.w : 画面モード番号

	.offset 0
O_CRTMOD_TBL_R0:	.ds.w	1
O_CRTMOD_TBL_R1:	.ds.w	1
O_CRTMOD_TBL_R2:	.ds.w	1
O_CRTMOD_TBL_R3:	.ds.w	1
O_CRTMOD_TBL_R4:	.ds.w	1
O_CRTMOD_TBL_R5:	.ds.w	1
O_CRTMOD_TBL_R6:	.ds.w	1
O_CRTMOD_TBL_R7:	.ds.w	1
O_CRTMOD_TBL_R20:	.ds.w	1
O_CRTMOD_TBL_HRL:	.ds.w	1
O_CRTMOD_TBL_H_TOTAL:	.ds.w	1
O_CRTMOD_TBL_H_DISP:	.ds.w	1
O_CRTMOD_TBL_V_DISP:	.ds.w	1
O_CRTMOD_TBL_SP_RES:	.ds.w	1
O_CRTMOD_TBL_VCNT:	.ds.w	1

	.text

;-- C interface
__changeCRTmode:
	move.w	6(sp),d0				;

;-- ASM interface
_changeCRTmode:
	;-- 画面モード
	move.w	d0,(_CRTmode)				; 
	
	;--
	lsl.w	#5,d0					* 
	lea	_CRTmodeTable(pc),a0			*  8
	adda.w	d0,a0					*  8
	
	lea	CRTC_R0,a1				* 12
	
	move.w	O_CRTC_R20(a1),d0			* 12
	andi.w	#%0000_0000_0001_011,d0			*  8
	
	cmp.w	O_CRTMOD_TBL_R20(a0),d0			* 
	bhi.s	@f					* 10/ 8 (T/F)
	
	*-- 高い画面モードへの切り替え
	move.w	O_CRTMOD_TBL_R0(a0),O_CRTC_R0(a1)	* 
	move.w	O_CRTMOD_TBL_R1(a0),O_CRTC_R1(a1)	* 
	move.w	O_CRTMOD_TBL_R2(a0),O_CRTC_R2(a1)	* 
	move.w	O_CRTMOD_TBL_R3(a0),O_CRTC_R3(a1)	* 
	move.w	O_CRTMOD_TBL_R4(a0),O_CRTC_R4(a1)	* 
	move.w	O_CRTMOD_TBL_R5(a0),O_CRTC_R5(a1)	* 
	move.w	O_CRTMOD_TBL_R6(a0),O_CRTC_R6(a1)	* 
	move.w	O_CRTMOD_TBL_R7(a0),O_CRTC_R7(a1)	* 
	
	move.w	O_CRTMOD_TBL_R20(a0),O_CRTC_R20(a1)	* 
	
	bra	1f					* 10
	
	*-- 低い画面モードへの切り替え
@@:
	move.w	O_CRTMOD_TBL_R20(a0),O_CRTC_R20(a1)	* 
	
	move.w	O_CRTMOD_TBL_R0(a0),O_CRTC_R0(a1)	* 
	move.w	O_CRTMOD_TBL_R1(a0),O_CRTC_R1(a1)	* 
	move.w	O_CRTMOD_TBL_R2(a0),O_CRTC_R2(a1)	* 
	move.w	O_CRTMOD_TBL_R3(a0),O_CRTC_R3(a1)	* 
	move.w	O_CRTMOD_TBL_R4(a0),O_CRTC_R4(a1)	* 
	move.w	O_CRTMOD_TBL_R5(a0),O_CRTC_R5(a1)	* 
	move.w	O_CRTMOD_TBL_R6(a0),O_CRTC_R6(a1)	* 
	move.w	O_CRTMOD_TBL_R7(a0),O_CRTC_R7(a1)	* 
	
	*-- HRL 設定
1:	
	move.b	SYSPORT_4,d0				* 
	andi.b	#%1111_1101,d0				* 
	or.w	O_CRTMOD_TBL_HRL(a0),d0			* 
	move.b	d0,SYSPORT_4				* 
	
	*-- ビデオコントローラ設定
;	move.w	O_CRTMOD_TBL_R20(a0),d0			* 12
;	lsr.w	#8,d0					* 
;	andi.w	#%0000_0000_0000_0111,d0		*  8
;	move.w	d0,VideoCtrl_R0				* 
	
	move.w	O_CRTMOD_TBL_VCNT(a0),(VideoCtrl_R0)	* 
	
	*-- スプライトコントローラ H-DISP 設定
	move.w	O_CRTMOD_TBL_H_DISP(a0),SpCtrl_H_DISP	* 
	
	*-- 130μ秒のウェイト
	move.w	#10-1,d0				* 
1:
	;水平表示期間まで待つ
@@:	tst.b	(MFP_GPIP)				* 
	bmi.s	@b					* 
	
	;水平同期期間を待つ
@@:	tst.b	(MFP_GPIP)				* 
	bpl.s	@b					* 
	
	dbra	d0,@b					* 
	
	*-- スプライトコントローラ H-TOTAL 設定
	move.w	O_CRTMOD_TBL_H_TOTAL(a0),SpCtrl_H_TOTAL	* 
	
	*-- スプライトコントローラ V-DISP 設定
	move.w	O_CRTMOD_TBL_V_DISP(a0),SpCtrl_V_DISP	* 
	
	*-- スプライトコントローラ L/H freq, V/H Res 設定
	move.w	O_CRTMOD_TBL_SP_RES(a0),SpCtrl_RES	* 
	
	rts						* 16



_CRTmodeTable:
	*-- No. 0  : 320x248, 16color - GRAPHIC 1 用
	.dc.w	$0045			* CRTC R0
	.dc.w	$0009			* CRTC R1
	.dc.w	$000f			* CRTC R2
	.dc.w	$0037			* CRTC R3
	.dc.w	$0204			* CRTC R4
	.dc.w	$0004			* CRTC R5
	.dc.w	$0013			* CRTC R6
	.dc.w	$0203			* CRTC R7
	.dc.w	$0011			* CRTC R20
	.dc.w	%0000_0000_0000_0010	* HRL
	.dc.w	$00ff			* SP CTRL H-TOTAL
	.dc.w	$000f+4+4		* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$0013+16*3		* SP CTRL V_DISP	(CRTC R6)
	.dc.w	%0000_0000_0001_0000	* SP CTRL L/H freq, V/H Res
	.dc.w	$0000			; Video Controler
	.dc.w	0

	*-- No. 1  : 384x248, 16color - TEXT 1 用
	.dc.w	$0045			* CRTC R0
	.dc.w	$0007			* CRTC R1
	.dc.w	$000b			* CRTC R2
	.dc.w	$003b			* CRTC R3
	.dc.w	$0204			* CRTC R4
	.dc.w	$0004			* CRTC R5
	.dc.w	$0014			* CRTC R6
	.dc.w	$01f4			* CRTC R7
	.dc.w	$0011			* CRTC R20
	.dc.w	%0000_0000_0000_0010	* HRL
	.dc.w	$00ff			* SP CTRL H-TOTAL
	.dc.w	$000b+4			* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$0014			* SP CTRL V_DISP	(CRTC R6)
	.dc.w	%0000_0000_0001_0000	* SP CTRL L/H freq, V/H Res
	.dc.w	$0000			; Video Controler
	.dc.w	0

	*-- No. 2  : 640x231, 16color - TEXT 2 用
	.dc.w	$008b			* CRTC R0
	.dc.w	$0013			* CRTC R1
	.dc.w	$0023			* CRTC R2
	.dc.w	$0073			* CRTC R3
	.dc.w	$0205			* CRTC R4
	.dc.w	$0005			* CRTC R5
	.dc.w	$0028			* CRTC R6
	.dc.w	$01f6			* CRTC R7
	.dc.w	$0412			* CRTC R20
	.dc.w	%0000_0000_0000_0000	* HRL
	.dc.w	$00ff			* SP CTRL H-TOTAL
	.dc.w	$0023+4			* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$0028			* SP CTRL V_DISP	(CRTC R6)
	.dc.w	%0000_0000_0001_0000	* SP CTRL L/H freq, V/H Res
	.dc.w	$0004			; Video Controler
	.dc.w	0

	*-- No. 3  : 320x248, 65536color - GRAPHIC 2 用
	.dc.w	$0045			* CRTC R0
	.dc.w	$0009			* CRTC R1
	.dc.w	$000f			* CRTC R2
	.dc.w	$0037			* CRTC R3
	.dc.w	$0204			* CRTC R4
	.dc.w	$0004			* CRTC R5
	.dc.w	$0013			* CRTC R6
	.dc.w	$0203			* CRTC R7
	.dc.w	$0311			* CRTC R20
	.dc.w	%0000_0000_0000_0010	* HRL
	.dc.w	$00ff			* SP CTRL H-TOTAL
;;;	.dc.w	$000f+4+5		* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$000f+4+4		* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$0013+16*3		* SP CTRL V_DISP	(CRTC R6)
	.dc.w	%0000_0000_0001_0000	* SP CTRL L/H freq, V/H Res
	.dc.w	$0000			; Video Controler
	.dc.w	0

	*-- No. 4  : 320x248, 65536color - GRAPHIC 2 用 (15kHz)
	.dc.w	$0041			* CRTC R0
	.dc.w	$0004			* CRTC R1
	.dc.w	$000c			* CRTC R2
	.dc.w	$0034			* CRTC R3
	.dc.w	$0111			* CRTC R4
	.dc.w	$0008			* CRTC R5
	.dc.w	$0011			* CRTC R6
	.dc.w	$0109			* CRTC R7
	.dc.w	$0314			* CRTC R20
	.dc.w	%0000_0011_0000_0010	* HRL
	.dc.w	$00ff			* SP CTRL H-TOTAL
;;;	.dc.w	$000f+4+5		* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$000c+4+4		* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$0011+8*3		* SP CTRL V_DISP	(CRTC R6)
	.dc.w	%0000_0000_0000_0000	* SP CTRL L/H freq, V/H Res
	.dc.w	$0000			; Video Controler
	.dc.w	0

	*-- No. 5  : 768x512, 16color - Z80 Debugger 用
	.dc.w	$0089			* CRTC R0
	.dc.w	$000e			* CRTC R1
	.dc.w	$001c			* CRTC R2
	.dc.w	$0011			* CRTC R3
	.dc.w	$0237			* CRTC R4
	.dc.w	$0005			* CRTC R5
	.dc.w	$0028			* CRTC R6
	.dc.w	$0228			* CRTC R7
	.dc.w	$0416			* CRTC R20
	.dc.w	%0000_0000_0000_0000	* HRL
	.dc.w	$00ff			* SP CTRL H-TOTAL
	.dc.w	$001c+4			* SP CTRL H-DISP	(CRTC R2 + 4)
	.dc.w	$0028			* SP CTRL V_DISP	(CRTC R6)
	.dc.w	%0000_0000_0001_0000	* SP CTRL L/H freq, V/H Res
	.dc.w	$0004			; Video Controler
	.dc.w	0



;-- 現在の画面モード
_CRTmode:	.dc.w	0


