;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;


	.xdef	_VRAM
	.xdef	_CountVDISP,_CountUpdate
	.xdef	_cnt_CountVDISP,_cnt_CountUpdate

_clearVRAM_BLOCK:
	moveq.l	#0,d0					*  4
	lea	VRAM_BLOCK_LOW(regZjmp),a0		*  8
	lea	$8000(a0),a0				*  8
	
	move.w	#256/2-1,d1				*  8
@@:
	move.l	d0,(a0)					* 12
	move.l	d0,$100(a0)				* 16
	lea	$0200(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	rts						* 16



_SetVRAM_512:
	move.w	#512-1,d1				*  4
@@:	move.b	d0,(a0)					*  8
	lea	$0100(a0),a0				*  8
	dbra	d1,@b					* 10/14 (T/F)
	
	rts						* 16

_SetVRAM_256:
	jsr	_SetVRAM_128(pc)			* 18
	jmp	_SetVRAM_128(pc)			* 10

_SetVRAM_128:
	jsr	_SetVRAM_32(pc)				* 18
	lea	$2000(a0),a0				*  8
	jsr	_SetVRAM_32(pc)				* 18
	lea	$2000(a0),a0				*  8
	jsr	_SetVRAM_32(pc)				* 18
	lea	$2000(a0),a0				*  8
	jsr	_SetVRAM_32(pc)				* 18
	lea	$2000(a0),a0				*  8
	
	rts						* 16

_SetVRAM_32:
	move.b	d0,$1F00(a0)				* 12
	move.b	d0,$1E00(a0)				* 12
	move.b	d0,$1D00(a0)				* 12
	move.b	d0,$1C00(a0)				* 12
	move.b	d0,$1B00(a0)				* 12
	move.b	d0,$1A00(a0)				* 12
	move.b	d0,$1900(a0)				* 12
	move.b	d0,$1800(a0)				* 12
	move.b	d0,$1700(a0)				* 12
	move.b	d0,$1600(a0)				* 12
	move.b	d0,$1500(a0)				* 12
	move.b	d0,$1400(a0)				* 12
	move.b	d0,$1300(a0)				* 12
	move.b	d0,$1200(a0)				* 12
	move.b	d0,$1100(a0)				* 12
	move.b	d0,$1000(a0)				* 12
_SetVRAM_16:
	move.b	d0,$0F00(a0)				* 12
	move.b	d0,$0E00(a0)				* 12
	move.b	d0,$0D00(a0)				* 12
	move.b	d0,$0C00(a0)				* 12
	move.b	d0,$0B00(a0)				* 12
	move.b	d0,$0A00(a0)				* 12
	move.b	d0,$0900(a0)				* 12
	move.b	d0,$0800(a0)				* 12
_SetVRAM_8:
	move.b	d0,$0700(a0)				* 12
	move.b	d0,$0600(a0)				* 12
	move.b	d0,$0500(a0)				* 12
	move.b	d0,$0400(a0)				* 12
_SetVRAM_4:
	move.b	d0,$0300(a0)				* 12
_SetVRAM_3:
	move.b	d0,$0200(a0)				* 12
	move.b	d0,$0100(a0)				* 12
	move.b	d0,(a0)					*  8
	
	rts						* 16



*----------------------------------------------------------------------------------------------
_initVDP:
	;-- スクロールレジスタ設定
	moveq.l	#0,d0					*  4
	move.w	d0,(GScroll_X0)				* 
	move.w	d0,(GScroll_Y0)				* 
	move.w	d0,(GScroll_X1)				* 
	move.w	d0,(GScroll_Y1)				* 
	move.w	d0,(GScroll_X2)				* 
	move.w	d0,(GScroll_Y2)				* 
	move.w	d0,(GScroll_X3)				* 
	move.w	d0,(GScroll_Y3)				* 
	
	move.w	d0,(TScroll_X)				* 
	move.w	d0,(TScroll_Y)				* 
	
	;-- テキスト画面アクセスマスク
	move.w	d0,(CRTC_R23)				* 
	
	;-- I/O ルーチンの設定
	lea	TXT1_L_IOroutine,regIO			* 
	
	movea.l	_VRAM(pc),regVadr			* 16
	
	move.l	regVadr,PatGenTbl(regFtbl)		* 16
	move.l	regVadr,PatNameTbl(regFtbl)		* 16
	move.l	regVadr,ColorTbl(regFtbl)		* 16
	move.l	regVadr,SprPatGenTbl(regFtbl)		* 16
	move.l	regVadr,SprAtrTbl(regFtbl)		* 16
	
	bra	_TXT1_SetTable				* 10





*----------------------------------------------------------------------------------------------
* TEXT1 テーブル設定
_TXT1_SetTable:
	*-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	move.w	PatGenTbl+2(regFtbl),d0			* 12
	move.b	PatGenTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_TEXT_PATGEN,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	move.w	PatNameTbl+2(regFtbl),d0		* 12
	move.b	PatNameTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_TEXT1_PATNAME,d0			*  4
	jsr	_SetVRAM_4(pc)				* 18
	
	
	;-- 画面再描画
;;	clr.b	ScreenRefresh(regFtbl)			* 16
	
	
	rts						* 16



*----------
* TEXT2 テーブル設定
_TXT2_SetTable:
	*-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	move.w	PatGenTbl+2(regFtbl),d0			* 12
	move.b	PatGenTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_TEXT_PATGEN,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	*-- パターンネームテーブル (4 Kbytes, 16 blocks)
	move.w	PatNameTbl+2(regFtbl),d0		* 12
	move.b	PatNameTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_TEXT2_PATNAME,d0			*  4
	jsr	_SetVRAM_16(pc)				* 18
	
	
	*-- カラーテーブル (512 bytes, 2 blocks)
	
	
	;-- 画面再描画
;;	clr.b	ScreenRefresh(regFtbl)			* 16
	
	
	rts						* 16



*----------
* MULTI COLOR テーブル設定
_MLT_SetTable:
	*-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	move.w	PatGenTbl+2(regFtbl),d0			* 12
	move.b	PatGenTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_MULTI_PATGEN,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	move.w	PatNameTbl+2(regFtbl),d0		* 12
	move.b	PatNameTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_MULTI_PATNAME,d0			*  4
	jsr	_SetVRAM_4(pc)				* 18
	
	
	;-- 画面再描画
;;	clr.b	ScreenRefresh(regFtbl)			* 16
	
	
	rts						* 16




_GRA1_SetTable_withColor:
	;-- カラーテーブルアップデート
	movea.l	d0,a0					*  4
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
	
	;-- Fall through
	
*----------
* GRAPHIC 1 テーブル設定
_GRA1_SetTable:
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	move.w	PatNameTbl+2(regFtbl),d0		* 12
	move.b	PatNameTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_GRA1_PATNAME,d0			*  4
	jsr	_SetVRAM_3(pc)				* 18
	
	
	*-- パターンジェネレータテーブル (2 Kbytes, 8 blocks)
	move.w	PatGenTbl+2(regFtbl),d0			* 12
	move.b	PatGenTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_GRA1_PATGEN,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	* カラーテーブル (64 bytes, 1/4 blocks)
	move.w	ColorTbl+2(regFtbl),d0				* 12
	
	add.b	d0,d0						*  4
	bcs.s	2f						* 10/ 8 (T/F)
	
	add.b	d0,d0						*  4
	bcs.s	1f						* 10/ 8 (T/F)
	
	move.b	ColorTbl_HL(regFtbl),d0				* 12
	move.b	#VDP_GRA1_COLOR_0,VRAM_BLOCK(regZjmp,d0.w)	* 18
	
	bra	@f						* 10
	
1:
	move.b	ColorTbl_HL(regFtbl),d0				* 12
	move.b	#VDP_GRA1_COLOR_64,VRAM_BLOCK(regZjmp,d0.w)	* 18
	
	bra	@f						* 10

2:	add.b	d0,d0						*  4
	bcs.s	3f						* 10/ 8 (T/F)
	
	move.b	ColorTbl_HL(regFtbl),d0				* 12
	move.b	#VDP_GRA1_COLOR_128,VRAM_BLOCK(regZjmp,d0.w)	* 18
	
	bra	@f						* 10

3:
	move.b	ColorTbl_HL(regFtbl),d0				* 12
	move.b	#VDP_GRA1_COLOR_192,VRAM_BLOCK(regZjmp,d0.w)	* 18
	
@@:	
	
	
	*-- スプライトパターンジェネレータテーブル
	move.w	SprPatGenTbl+2(regFtbl),d0			* 12
	move.b	SprPatGenTbl_HL(regFtbl),d0			* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0			* 12
	moveq.l	#VDP_SP1_PATGEN,d0				*  4
	jsr	_SetVRAM_8(pc)					* 18
	
	*-- スプライト更新フラグ設定
;	moveq.l	#0,d0						*  4
;	move.w	d0,SprAtrTblUpdate(regFtbl)			* 12
;	move.w	d0,SprPatGenTblUpdate(regFtbl)			* 12
	
	
	
	*-- スプライトアトリビュートテーブル
	move.w	SprAtrTbl+2(regFtbl),d0				* 12
	add.b	d0,d0						*  4
	bcs.s	@f						* 10/ 8 (T/F)
	
	move.b	SprAtrTbl_HL(regFtbl),d0			* 12
	tst.b	VRAM_BLOCK(regZjmp,d0.w)			* 
	beq	2f						* 
	
	
	
	
2:
	move.b	#VDP_SP1_ATTR_U,VRAM_BLOCK(regZjmp,d0.w)	* 18
	bra	1f						* 10
@@:	
	move.b	SprAtrTbl_HL(regFtbl),d0			* 12
	tst.b	VRAM_BLOCK(regZjmp,d0.w)			* 
	beq	2f						* 
	
	
	
	
2:
	move.b	#VDP_SP1_ATTR_B,VRAM_BLOCK(regZjmp,d0.w)	* 18
1:
	
	;-- 画面再描画
;;	clr.b	ScreenRefresh(regFtbl)			* 16
	
	
	rts							* 16



*----------
* GRAPHIC 2 テーブル設定
_GRA2_SetTable:
	*-- パターンジェネレータテーブル (8 Kbytes, 32 blocks)
	move.w	PatGenTbl+2(regFtbl),d0			* 12
	move.b	PatGenTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	
	moveq.l	#VDP_GRA23_PATGEN_B1,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATGEN_B2,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATGEN_B3,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
;	lea	$800(a0),a0				*  8
;	moveq.l	#VDP_GRA23_PATGEN_B4,d0			*  4
;	jsr	_SetVRAM_8(pc)				* 18
	
	
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	move.w	PatNameTbl+2(regFtbl),d0		* 12
	move.b	PatNameTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	
	moveq.l	#VDP_GRA23_PATNAME_B1,d0		*  4
	move.b	d0,(a0)					*  8
	
	move.b	#VDP_GRA23_PATNAME_B2,$100(a0)		* 16
;	move.b	#VDP_GRA23_PATNAME_B1,$100(a0)		* 16
	
	move.b	#VDP_GRA23_PATNAME_B3,$200(a0)		* 16
;	move.b	#VDP_GRA23_PATNAME_B1,$200(a0)		* 16
	
*!!	move.b	#VDP_GRA23_PATNAME_B4,$300(a0)		* 16
	
	
	* カラーテーブル (8 Kbytes, 32 blocks)
	move.w	ColorTbl+2(regFtbl),d0			* 12
	move.b	ColorTbl_HL(regFtbl),d0			* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	
	moveq.l	#VDP_GRA23_COLOR_B1,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_COLOR_B2,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_COLOR_B3,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	*-- スプライトパターンジェネレータテーブル
	move.w	SprPatGenTbl+2(regFtbl),d0			* 12
	move.b	SprPatGenTbl_HL(regFtbl),d0			* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0			* 12
	moveq.l	#VDP_SP1_PATGEN,d0				*  4
	jsr	_SetVRAM_8(pc)					* 18
	
	*-- スプライト更新フラグ設定
;;	moveq.l	#0,d0						*  4
;;	move.w	d0,SprAtrTblUpdate(regFtbl)			* 12
;;	move.w	d0,SprPatGenTblUpdate(regFtbl)			* 12
	
	
	*-- スプライトアトリビュートテーブル
	move.w	SprAtrTbl+2(regFtbl),d0				* 12
	add.b	d0,d0						*  4
	bcs.s	@f						* 10/ 8 (T/F)
	
	move.b	SprAtrTbl_HL(regFtbl),d0			* 12
	move.b	#VDP_SP1_ATTR_U,VRAM_BLOCK(regZjmp,d0.w)	* 18
	bra	1f						* 10
@@:	
	move.b	SprAtrTbl_HL(regFtbl),d0			* 12
	move.b	#VDP_SP1_ATTR_B,VRAM_BLOCK(regZjmp,d0.w)	* 18
1:
	
	
	;-- 画面再描画
;;	clr.b	ScreenRefresh(regFtbl)			* 16
	
	
	rts							* 16



*----------
* GRAPHIC 3 テーブル設定
_GRA3_SetTable:
	*-- パターンジェネレータテーブル (8 Kbytes, 32 blocks)
	move.w	PatGenTbl+2(regFtbl),d0			* 12
	move.b	PatGenTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_GRA23_PATGEN_B1,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATGEN_B2,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATGEN_B3,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATGEN_B4,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	*-- パターンネームテーブル (1 Kbytes, 4 blocks)
	move.w	PatNameTbl+2(regFtbl),d0		* 12
	move.b	PatNameTbl_HL(regFtbl),d0		* 12
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	
	moveq.l	#VDP_GRA23_PATNAME_B1,d0		*  4
	move.b	d0,(a0)					*  8
	
	lea	$100(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATNAME_B2,d0		*  4
	move.b	d0,(a0)					*  8
	
	lea	$100(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATNAME_B3,d0		*  4
	move.b	d0,(a0)					*  8
	
	lea	$100(a0),a0				*  8
	moveq.l	#VDP_GRA23_PATNAME_B4,d0		*  4
	move.b	d0,(a0)					*  8
	
	
	*-- カラーテーブル (8 Kbytes, 32 blocks)
	move.w	ColorTbl+2(regFtbl),d0			* 12
	move.b	ColorTbl_HL(regFtbl),d0			* 12
	
	lea	VRAM_BLOCK(regZjmp,d0.w),a0		* 12
	moveq.l	#VDP_GRA23_COLOR_B1,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_COLOR_B2,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	lea	$800(a0),a0				*  8
	moveq.l	#VDP_GRA23_COLOR_B3,d0			*  4
	jsr	_SetVRAM_8(pc)				* 18
	
	
	;-- 画面再描画
;;	clr.b	ScreenRefresh(regFtbl)			* 16
	
	
	rts						* 16



*----------------------------------------------------------------------------------------------
_TXT1_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	0					* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_TXT2_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	0					* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_MLT_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	0					*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA1_ScreenInitTbl:
	.dc.w	0					*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA2_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	0					*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA3_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	0					*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA4_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	0					*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA5_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	0					*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA6_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	0					*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	GRA7_ScreenInitialize-FtblinIO		*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


_GRA7_ScreenInitTbl:
	.dc.w	GRA1_ScreenInitialize-FtblinIO		*  0	GRAPHIC 1
	.dc.w	GRA2_ScreenInitialize-FtblinIO		*  1	GRAPHIC 2
	.dc.w	GRA3_ScreenInitialize-FtblinIO		*  2	GRAPHIC 3
	.dc.w	GRA4_ScreenInitialize-FtblinIO		*  3	GRAPHIC 4
	.dc.w	GRA5_ScreenInitialize-FtblinIO		*  4	GRAPHIC 5
	.dc.w	GRA6_ScreenInitialize-FtblinIO		*  5	GRAPHIC 6
	.dc.w	0					*  6
	.dc.w	0					*  7	GRAPHIC 7
	.dc.w	MLT_ScreenInitialize-FtblinIO		*  8	MULTI COLOR
	.dcb.w	7,0					*  9 - 15
	.dc.w	TXT1_ScreenInitialize-FtblinIO		* 16	TEXT 1
	.dc.w	0					* 17
	.dc.w	TXT2_ScreenInitialize-FtblinIO		* 18	TEXT 2
	.dcb.w	31-18,0					* 19 - 31


*----------------------------------------------------------------------------------------------
_VRAM:			.dc.l	0

			.even
			.dc.b	0
_VDPport1_data:		.dc.b	0
_VDPport1_dataset:	.dc.b	0

_VDPpalette_dataset:	.dc.b	0
			.even
_VDPpalette_data:	.dc.w	0

_ScreenRefresh:		.dc.w	-1
_PatNameTblUpdate:	.dc.w	-1
_PatGenTblUpdate:	.dc.w	-1
_ColorTblUpdate:	.dc.w	-1

_SprPatGenTblUpdate:	.dc.w	-1
_SprAtrTblUpdate:	.dc.w	-1

_inScrUpdate:		.dc.b	0
			.even

;-- パターンジェネレータテーブル
_PatGenTbl:		.dc.l	0
_PatGenTbl_HL:		.dc.w	0

;-- パターンネームテーブル
_PatNameTbl:		.dc.l	0
_PatNameTbl_HL:		.dc.w	0

;-- カラーテーブル
_ColorTbl:		.dc.l	0
_ColorTbl_HL:		.dc.w	0

;-- スプライトパターンジェネレータテーブル
_SprPatGenTbl:		.dc.l	0
_SprPatGenTbl_HL:	.dc.w	0

;-- スプライトアトリビュートテーブル
_SprAtrTbl:		.dc.l	0
_SprAtrTbl_HL:		.dc.w	0


_CountVDISP:		.dc.w	1
_cnt_CountVDISP:	.dc.w	1

_CountUpdate:		.dc.w	1
_cnt_CountUpdate:	.dc.w	1


_GRAMadr:		.dc.l	0

_VDPreg_0:		.dc.w	0
_VDPreg_1:		.dc.w	0
_VDPreg_2:		.dc.w	0
_VDPreg_3:		.dc.w	0
_VDPreg_4:		.dc.w	0
_VDPreg_5:		.dc.w	0
_VDPreg_6:		.dc.w	0

*-- Text color / Back drop color register
_VDPreg_7:		.dc.w	$f400		;上位バイトを使用
_GRA2_backDropColor:	.dc.w	0

_VDPreg_8:		.dc.w	0
_VDPreg_9:		.dc.w	0
_VDPreg_10:		.dc.w	0
_VDPreg_11:		.dc.w	0
_VDPreg_12:		.dc.w	0
_VDPreg_13:		.dc.w	0
_VDPreg_14:		.dc.w	0
_VDPreg_15:		.dc.w	0

;-- Color palette address
_VDPreg_16:		.dc.w	0

_VDPreg_17:		.dc.w	0
_VDPreg_18:		.dc.w	0
_VDPreg_19:		.dc.w	0
_VDPreg_20:		.dc.w	0
_VDPreg_21:		.dc.w	0
_VDPreg_22:		.dc.w	0
_VDPreg_23:		.dc.w	0

*-- Source X register
			.even
_VDPcom_SX:
_VDPreg_33:		.dc.b	0	* high
_VDPreg_32:		.dc.b	0	* low

*-- Source Y register
			.even
_VDPcom_SY:
_VDPreg_35:		.dc.b	0	* high
_VDPreg_34:		.dc.b	0	* low

*-- Destination X register
			.even
_VDPcom_DX:
_VDPreg_37:		.dc.b	0	* high
_VDPreg_36:		.dc.b	0	* low

*-- Destination Y register
			.even
_VDPcom_DY:
_VDPreg_39:		.dc.b	0	* high
_VDPreg_38:		.dc.b	0	* low

*-- Number of dot X register
			.even
_VDPcom_NX:
_VDPreg_41:		.dc.b	0	* high
_VDPreg_40:		.dc.b	0	* low

*-- Number of dot Y register
			.even
_VDPcom_NY:
_VDPreg_43:		.dc.b	0	* high
_VDPreg_42:		.dc.b	0	* low

*-- Color register
			.even
_VDPcom_CLR:
_VDPreg_44:		.dc.b	0

*-- Argument register
			.even
_VDPcom_ARG:
_VDPreg_45:		.dc.b	0

*-- Command register
			.even
_VDPcom_CMR:
_VDPreg_46:		.dc.w	0


_StatusReg_0:		.dc.b	0
_StatusReg_1:		.dc.b	0
_StatusReg_2:		.dc.b	%0000_1100	* TR VR HR BD 1 1 EO CE
_StatusReg_3:		.dc.b	0
_StatusReg_4:		.dc.b	0
_StatusReg_5:		.dc.b	0
_StatusReg_6:		.dc.b	0
_StatusReg_7:		.dc.b	0
_StatusReg_8:		.dc.b	0
_StatusReg_9:		.dc.b	0
_StatusReg_10:		.dc.b	0
_StatusReg_11:		.dc.b	0
_StatusReg_12:		.dc.b	0
_StatusReg_13:		.dc.b	0
_StatusReg_14:		.dc.b	0
_StatusReg_15:		.dc.b	0


			.even
_paletteTbl:		*	 Blue  Red  Green
			.dc.w	%00000_00000_00000_1
			.dc.w	%00000_00000_00000_1
			.dc.w	%11011_00100_00100_1
			.dc.w	%11111_01101_01101_1
			.dc.w	%00100_00100_11111_1
			.dc.w	%01101_01001_11111_1
			.dc.w	%00100_10110_00100_1
			.dc.w	%11011_01001_11111_1
			.dc.w	%00100_11111_00100_1
			.dc.w	%01101_11111_01101_1
			.dc.w	%11011_11011_00100_1
			.dc.w	%11011_11011_10010_1
			.dc.w	%10010_00100_00100_1
			.dc.w	%01001_11011_10110_1
			.dc.w	%10110_10110_10110_1
			.dc.w	%11111_11111_11111_1


;-- パターンジェネレータ変更テーブル
_PatGenUpdateTbl:	.dcb.w	256*4,-1

;-- スプライトパターンジェネレータ変更テーブル
_SprPatGenUpdateTbl:	.dcb.w	256,-1

;-- スプライトアトリビュート変更テーブル
_SprAtrUpdateTbl:	.dcb.w	32,-1

tail_signed:	.equ *+2
