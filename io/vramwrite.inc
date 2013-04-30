;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;



;----------------------------------------------------------------------------------------------
; (NOP の処理ルーチンに入れる)
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite 00
	;-- スクリーンモードでルーチンをコピーする
	
	move.b	d0,(regVadr)+				*  8
	
.if 1
;.if MSX1screen
	move.w	regVadr,d0				*  4
	andi.w	#%0011_1111_1111_1111,d0		*  8
	beq.s	@f					* 10/ 8 (T/F)
	
	rts						* 16
@@:
	lea	-$4000(regVadr),regVadr			*  8
	rts						* 16
.else
	move.w	regVadr,d0				*  4
	bne.s	@f					* 10/ 8 (T/F)
	
	move.l	regVadr,d0				*  4
	btst	#16,d0					* 10
	beq.s	@f					* 10/ 8 (T/F)
	
	movea.l	VRAM(regFtbl),regVadr			* 16
@@:
	;-- 256バイト単位で VRAM アドレスの設定処理を行う
	; (regVadr の最上位ﾋﾞｯﾄは無視して良い)
	tst.b	d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	rts						* 16
@@:
	;-- VRAM_{LOW,HIGH}
	move.b	(regIO),d0				*  8
	move.w	VRAM_BLOCK(regZjmp,d0.w),d1		* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
.endif
;.endif
	EndVRAMwrite 00


;---
; (DDCB00 の処理ルーチンに入れる)
	VRAMsetAdr 00
	
	rts						* 16
	EndVRAMsetAdr 00



;----------------------------------------------------------------------------------------------
; TEXT 1,2 (SCREEN 0, 40/80字モード)
; Pattern generator table - 2Kbyte単位, 2Kbytes
; 	画面更新は垂直帰線割り込みで行う
;	d0.b      ... data
;	regVadr.l ... VRAM address
	
	VRAMwrite 80
	;-- pat.gen.tabのオフセットをテーブルに設定
	move.w	regVadr,d2				*  4
	andi.w	#%0000_0111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl(regFtbl),a0		*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d0				*  4
	tst.b	d0					*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	rts						* 16
@@:
	andi.w	#%0011_1111_1111_1111,d0		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	rts						* 16
	EndVRAMwrite 80


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 80
	
	rts						* 16
	EndVRAMsetAdr 80



;------
; TEXT 1 (SCREEN 0, 40字モード)
; Pattern name table - 40x24 : 1Kbyte単位, 960bytes
;	画面更新は垂直帰線割り込み処理で行う
;	d0.b      ... data
;	regVadr.l ... VRAM address
	
	VRAMwrite 81
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
	;-- pat.gen.tabのオフセットを計算
	lsl.w	#3,d0					* 12
	andi.w	#%0000_0111_1111_1000,d0		*  8
	
	;-- 更新位置
	andi.w	#%0000_0011_1111_1111,d2		*  8
	
	;-- テーブルに設定
	lea	_PatNameUpdateTbl(pc),a0		*  8
	add.w	d2,d2					*  4
	move.w	d0,(a0,d2.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatNameTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
	rts						* 16
	
	;--
1:
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
	EndVRAMwrite 81


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 81
	
	rts						* 16
	EndVRAMsetAdr 81


;------
; TEXT 2 (SCREEN 0, 80字モード)
; Pattern name table - 80x24 : 4Kbyte単位, 2159bytes
;	画面更新は垂直帰線割り込み処理で行う
;	d0.b      ... data
;	regVadr.l ... VRAM address
	
	VRAMwrite 82
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/f)
2:
	;-- pat.gen.tabのオフセットを計算
	lsl.w	#3,d0					* 12
	andi.w	#%0000_0111_1111_1000,d0		*  8
	
	;-- 更新位置
	andi.w	#%0000_1111_1111_1111,d2		*  8
	
	;-- テーブルに設定
	lea	_PatNameUpdateTbl(pc),a0		*  8
	add.w	d2,d2					*  4
	move.w	d0,(a0,d2.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatNameTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
	rts						* 16
	
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
	EndVRAMwrite 82


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 82
	
	rts						* 16
	EndVRAMsetAdr 82


;------
; TEXT 2 (SCREEN 0, 80字モード)
; Color table - 512byte単位, 269bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite 83
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	beq.s	@f					* 10/ 8 (T/F)
	
	rts						* 16
	
@@:	;--
	lea	-$4000(regVadr),regVadr			*  8
	
	rts						* 16
	EndVRAMwrite 83


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 83
	
	rts						* 16
	EndVRAMsetAdr 83


;----------------------------------------------------------------------------------------------
; MULTI COLOR (SCREEN 3)
; Pattern generator table - 2Kbyte単位, 2Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite 84
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	
	rts						* 16
	EndVRAMwrite 84


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 84
	
	rts						* 16
	EndVRAMsetAdr 84


;------
; MULTI COLOR (SCREEN 3)
; Pattern name table - 1Kbyte単位, 767bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite 85
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	
	rts						* 16
	EndVRAMwrite 85


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 85
	
	rts						* 16
	EndVRAMsetAdr 85


;----------------------------------------------------------------------------------------------
; GRAPHIC 1 (SCREEN 1) - 32x24
; Pattern generator table - 2Kbyte単位, 2Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite 86
.if 0

;	trap	#9

	
	andi.w	#%0000_0111_1111_1111,d2		*  8
	
	* PCGのアドレス
	lea.l	PCGarea,a0				* 12
	add.w	d2,d2					*  4
	add.w	d2,d2					*  4
*	lea	(a0,d2.w),a0				* 12
	adda.w	d2,a0					*  8
	
	lea	PCGdatatbl,a1				* 12
	
	* パターンの１の部分のデータ (カラーコードは１５)
	move.b	d0,(regZeroSwap)			*  8	(swap.b, 下位バイトは 0)
	move.w	(regZeroSwap),d0			*  8
	move.l	15*4(a1,d0.w),(a0)			* 18
	
	*-- d2.l 復元
	move.l	regHL,d2				*  4
	
	rts						* 16 = 114
.else
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl(regFtbl),a0		*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
	rts						* 16
	
1:	;--
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 86


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 86
	
	rts						* 16
	EndVRAMsetAdr 86


;------
; GRAPHIC 1 (SCREEN 1)
; Pattern name table - 1Kbyte単位, 767bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite 87
.if 1
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
	lea	BGDATAarea,a0				* 12
	
	*-- 桁
	moveq.l	#%0001_1111,d1				*  4
	and.w	d2,d1					*  4
	
	*-- 行
	add.w	d2,d2					*  4
	andi.w	#%0000_0111_1100_0000,d2		*  8
	
	or.w	d1,d2					*  4
	add.w	d2,d2					*  4
	
**	moveq.l	#0,d1					*  4
	move.b	d0,d1					*  4
	add.w	d1,d1					*  4
	add.w	d1,d1					*  4
	move.b	d0,d1					*  4
	
	move.w	d1,(a0,d2.w)				* 14
	
	rts						* 16
	
1:	;--
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.else
	andi.w	#%0000_0011_1111_1111,d2		*  8
	add.w	d2,d2					*  4
	
	andi.w	#$00ff,d0				*  8
***	lsl.w	#3,d0					* 12
	
	lea	_PatNameUpdateTbl(pc),a0		*  8
	move.w	d0,(a0,d2.w)				* 14
	
	*-- 更新フラグセット
	move.w	d2,PatNameTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
	*-- d2.l 復元
	move.l	regHL,d2				*  4
	
	rts						* 16
.endif
	EndVRAMwrite 87


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 87
	
	rts						* 16
	EndVRAMsetAdr 87


;------
; GRAPHIC 1 (SCREEN 1)
; Color table - 64byte単位, 32bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address
;
;	  +0 +--------+
;	     |        | <- ここにある場合
;	 +64 +--------+
;	     |        |
;	+128 +--------+
;	     |        |
;	+192 +--------+
;	     |        |
;*	     +--------+

	VRAMwrite 88
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	
	add.b	d2,d2					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	add.b	d2,d2					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	bmi.s	@f					* 10/ 8 (T/F)
	
	lea	TPALETTE,a0				* 12
	ext.w	d2					*  4
	add.w	d2,a0					*  8
	
	lea	paletteTbl(regFtbl),a1			*  8
	
	;-- パターンの０の部分の色コード
	moveq.l	#%0000_1111,d1				*  4
	and.w	d0,d1					*  4
	add.w	d1,d1					*  4
	move.w	(a1,d1.w),(a0)+				* 
	
	;-- パターンの１の部分の色コード
	andi.w	#%0000_0000_1111_0000,d0		*  8
	lsr.w	#3,d0					* 12
	move.w	(a1,d0.w),(a0)				* 
	
@@:
	
	rts						* 16
	EndVRAMwrite 88


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 88
	
	rts						* 16
	EndVRAMsetAdr 88


;------
; GRAPHIC 1 (SCREEN 1)
; Color table - 64byte単位, 32bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address
;
;	  +0 +--------+
;	     |        |
;	 +64 +--------+
;	     |        | <- ここにある場合
;	+128 +--------+
;	     |        |
;	+192 +--------+
;	     |        |
;	     +--------+

	VRAMwrite A7
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	
	add.b	d2,d2					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	add.b	d2,d2					*  4
	bcc.s	@f					* 10/ 8 (T/F)
	
	bmi.s	@f					* 10/ 8 (T/F)
	
	lea	TPALETTE,a0				* 12
	ext.w	d2					*  4
	add.w	d2,a0					*  8
	
	lea	paletteTbl(regFtbl),a1			*  8
	
	;-- パターンの０の部分の色コード
	moveq.l	#%0000_1111,d1				*  4
	and.w	d0,d1					*  4
	add.w	d1,d1					*  4
	move.w	(a1,d1.w),(a0)+				* 
	
	;-- パターンの１の部分の色コード
	andi.w	#%0000_0000_1111_0000,d0		*  8
	lsr.w	#3,d0					* 12
	move.w	(a1,d0.w),(a0)				* 
	
@@:
	
	rts						* 16
	EndVRAMwrite A7


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A7
	
	rts						* 16
	EndVRAMsetAdr A7


;------
; GRAPHIC 1 (SCREEN 1)
; Color table - 64byte単位, 32bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address
;
;	  +0 +--------+
;	     |        |
;	 +64 +--------+
;	     |        |
;	+128 +--------+
;	     |        | <- ここにある場合
;	+192 +--------+
;	     |        |
;	     +--------+

	VRAMwrite A8
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	
	add.b	d2,d2					*  4
	bcc.s	@f					* 10/ 8 (T/F)
	
	add.b	d2,d2					*  4
	bcs.s	@f					* 10/ 8 (T/F)
	
	bmi.s	@f					* 10/ 8 (T/F)
	
	lea	TPALETTE,a0				* 12
	ext.w	d2					*  4
	add.w	d2,a0					*  8
	
	lea	paletteTbl(regFtbl),a1			*  8
	
	;-- パターンの０の部分の色コード
	moveq.l	#%0000_1111,d1				*  4
	and.w	d0,d1					*  4
	add.w	d1,d1					*  4
	move.w	(a1,d1.w),(a0)+				* 
	
	;-- パターンの１の部分の色コード
	andi.w	#%0000_0000_1111_0000,d0		*  8
	lsr.w	#3,d0					* 12
	move.w	(a1,d0.w),(a0)				* 
	
@@:
	rts						* 16
	EndVRAMwrite A8


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A8
	
	rts						* 16
	EndVRAMsetAdr A8


;------
; GRAPHIC 1 (SCREEN 1)
; Color table - 64byte単位, 32bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address
;
;	  +0 +--------+ 00xx xxxx
;	     |        |
;	 +64 +--------+ 01xx xxxx
;	     |        |
;	+128 +--------+ 10xx xxxx
;	     |        |
;	+192 +--------+ 11xx xxxx
;	     |        | <- ここにある場合
;	     +--------+

	VRAMwrite A9
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
@@:
	
	add.b	d2,d2					*  4
	bcc.s	@f					* 10/ 8 (T/F)
	
	add.b	d2,d2					*  4
	bcc.s	@f					* 10/ 8 (T/F)
	
	bmi.s	@f					* 10/ 8 (T/F)
	
	lea	TPALETTE,a0				* 12
	ext.w	d2					*  4
	add.w	d2,a0					*  8
	
	lea	paletteTbl(regFtbl),a1			*  8
	
	;-- パターンの０の部分の色コード
	moveq.l	#%0000_1111,d1				*  4
	and.w	d0,d1					*  4
	add.w	d1,d1					*  4
	move.w	(a1,d1.w),(a0)+				* 
	
	;-- パターンの１の部分の色コード
	andi.w	#%0000_0000_1111_0000,d0		*  8
	lsr.w	#3,d0					* 12
	move.w	(a1,d0.w),(a0)				* 
	
@@:
	rts						* 16
	EndVRAMwrite A9


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A9
	
	rts						* 16
	EndVRAMsetAdr A9




;----------------------------------------------------------------------------------------------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern generator table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address
; BLOCK 1
	VRAMwrite 89
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック1はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
.endif
	andi.w	#%0000_0111_1111_1111,d2		*  8
	
.if 0
	;-- カラーテーブルのデータを取得
	movea.l	ColorTbl(regFtbl),a0			* 16
	move.b	(a0,d2.w),d1				* 14
	
	
	;-- PCGのアドレス
	lea.l	PCGarea,a0				* 12
	add.w	d2,d2					*  4
	add.w	d2,d2					*  4
	adda.w	d2,a0					*  8
	
	lea	PCGdatatbl,a1				* 12
	
	move.b	d0,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d2			*  8
	
	;-- パターンの１の部分のデータ
	move.b	d1,d2					*  4
	andi.b	#%1111_0000,d2				*  8
	move.l	(a1,d2.w),d0				* 18
	
	;-- パターンの０の部分のデータ
	move.b	d1,d2					*  4
	lsl.b	#4,d2					* 14
	or.l	4(a1,d2.w),d0				* 20
	
	;-- PCG設定
	move.l	d0,(a0)					* 12 = 166
.else
	;--
	move.b	d0,-(sp)				*  8
	moveq.l	#0,d0					*  4
	move.w	(sp)+,d0				*  8
	
	;-- カラーテーブルのデータを取得
	movea.l	ColorTbl(regFtbl),a0			* 16
	move.b	(a0,d2.w),d0				* 14
	
	;-- 変換テーブル
	lea	table_PCGtoXPCG,a0			* 12
	lsl.l	#2,d0					* 12
	
	;-- 
	move.l	usp,a1					*  4	PCGarea
	add.w	d2,d2					*  4
	add.w	d2,d2					*  4
	move.l	(a0,d0.l),(a1,d2.w)			* 32 = 118
.endif
	
	
	rts						* 16
	
	;-----
.if 0
1:
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra.s	2b					* 10
.endif
	EndVRAMwrite 89


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 89
	
	rts						* 16
	EndVRAMsetAdr 89


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern generator table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 2
	VRAMwrite 8A
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック2はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
.endif
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
;;	andi.w	#%0001_1111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl+256*2(regFtbl),a0	*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
	;---
.if 0
1:
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra.s	2b					* 10
.endif
	EndVRAMwrite 8A


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 8A
	
	rts						* 16
	EndVRAMsetAdr 8A


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern generator table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 3
	VRAMwrite 8B
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック3はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
.endif
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
;	andi.w	#%0001_1111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl+256*2*2(regFtbl),a0	*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
	;---
.if 0
1:
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 8B


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 8B
	
	rts						* 16
	EndVRAMsetAdr 8B


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern generator table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 4
	VRAMwrite 8C
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:	
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
;	andi.w	#%0001_1111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl+256*2*3(regFtbl),a0	*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts
	
							* 16
	;---
1:
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10/ 8 (T/F)
	EndVRAMwrite 8C


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 8C
	
	rts						* 16
	EndVRAMsetAdr 8C


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Color table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

* BLOCK 1
	VRAMwrite 8D
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック1はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:	
.endif
	;--
	andi.w	#%0000_0111_1111_1111,d2		*  8
	
.if 0
	;-- パターンジェネレータテーブルのデータを取得
	movea.l	PatGenTbl(regFtbl),a0			* 16
	move.b	(a0,d2.w),(regZeroSwap)			* 18
	
	;-- PCGのアドレス
	lea.l	PCGarea,a0				* 12
	add.w	d2,d2					*  4
	add.w	d2,d2					*  4
	adda.w	d2,a0					*  8
	
	lea	PCGdatatbl,a1				* 12
	
	move.w	(regZeroSwap),d2			*  8
	
	move.b	d0,d1					*  4
	
	;-- パターンの１の部分のデータ
	move.b	d1,d2					*  4
	andi.b	#%1111_0000,d2				*  8
	move.l	(a1,d2.w),d0				* 18
	
	;-- パターンの０の部分のデータ
	move.b	d1,d2					*  4
	lsl.b	#4,d2					* 14
	or.l	4(a1,d2.w),d0				* 20
	
	;-- PCG設定
	move.l	d0,(a0)					* 12
.else
	;---
	;-- パターンジェネレータテーブルのデータを取得
	movea.l	PatGenTbl(regFtbl),a0			* 16
	move.b	(a0,d2.w),-(sp)				* 18
	move.w	(sp)+,d1				*  8
	
	move.b	d0,d1					*  4
	
	;-- 変換テーブル
	lea	table_PCGtoXPCG,a0			* 12
	lsl.l	#2,d1					* 12
	
	;--
	move.l	usp,a1					*  4	PCGarea
	add.w	d2,d2					*  4
	add.w	d2,d2					*  4
	move.l	(a0,d1.l),(a1,d2.w)			* 32
	
	moveq.l	#0,d1					*  4 = 118
.endif
	
	
	rts						* 16
	
	;---
.if 0
1:
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 8D


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 8D
	
	rts						* 16
	EndVRAMsetAdr 8D


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Color table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 2
	VRAMwrite 8E
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック2はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
.endif
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
;	andi.w	#%0001_1111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl+256*2(regFtbl),a0	*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
	
.if 0
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 8E


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 8E
	
	rts						* 16
	EndVRAMsetAdr 8E


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Color table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 3
	VRAMwrite 8F
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック3はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
.endif
	
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
;	andi.w	#%0001_1111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl+256*2*2(regFtbl),a0	*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
	
.if 0
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 8F


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 8F
	
	rts						* 16
	EndVRAMsetAdr 8F


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Color table - 8Kbyte単位, 8Kbytes(6136bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 4
	VRAMwrite 90
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:	
	;-- pat.gen.tabのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
;	andi.w	#%0001_1111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	PatGenUpdateTbl+256*2*3(regFtbl),a0	*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- 
	move.w	d2,PatGenTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
	
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10/ 8 (T/F)
	EndVRAMwrite 90


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 90
	
	rts						* 16
	EndVRAMsetAdr 90


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern name table - 1Kbyte単位, 1Kbytes(768bytes)
;	+---------+
;	| BLOCK 1 | 256 bytes
;	+---------+
;	| BLOCK 2 | 256 bytes
;	+---------+
;	| BLOCK 3 | 256 bytes
;	+---------+
;	| BLOCK 4 | 256 bytes
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 1
	VRAMwrite 91
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック1はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:	
.endif
	;-- BLOCK 1 なので、d2は 0 - 255 の範囲
	move.l	usp,a0					*  4	PCGarea
	lea	convXY_Block1(regFtbl),a1		*  8
	ext.w	d2					*  4
	add.w	d2,d2					*  4
	adda.w	(a1,d2.w),a0				* 18
	;- color code
	move.w	regZjmp,d1				*  4
	move.b	d0,d1					*  4
	move.w	d1,(a0)					*  8 = 54
	
	
	
.if 0
	;--
	lea	BGDATAarea,a0				* 12
	
	;-- 桁
	moveq.l	#%0001_1111,d1				*  4
	and.w	d2,d1					*  4
	
	;-- 行
	add.w	d2,d2					*  4
	andi.w	#%0000_0111_1100_0000,d2		*  8
	
	or.w	d1,d2					*  4
	add.w	d2,d2					*  4
	
	move.w	regZjmp,d1				*  4
	move.b	d0,d1					*  4
	
	move.w	d1,(a0,d2.w)				* 14 = 62
.endif
	
	rts						* 16
	
.if 0
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 91


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 91
	
	rts						* 16
	EndVRAMsetAdr 91


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern name table - 1Kbyte単位, 1Kbytes(768bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 2
	VRAMwrite 92
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック2はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:	
.endif
	;-- PatGenTab のオフセット計算
	lsl.w	#3,d0					* 12
	andi.w	#%0000_0111_1111_1000,d0		*  8
	
	;-- 更新位置
	andi.w	#%0000_0011_1111_1111,d2		*  8
	
	;-- テーブルに設定
	lea	_PatNameUpdateTbl(pc),a0		*  8	pc相対なのでﾘﾛｹｰﾄ後もOK
	add.w	d2,d2					*  4
	move.w	d0,(a0,d2.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatNameTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
.if 0
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10/ 8 (T/F)
.endif
	EndVRAMwrite 92


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 92
	
	rts						* 16
	EndVRAMsetAdr 92


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern name table - 1Kbyte単位, 1Kbytes(768bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 3
	VRAMwrite 93
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
;-- ブロック3はVRAM境界にない
.if 0
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
.endif
	;-- PatGenTab のオフセット計算
	lsl.w	#3,d0					* 12
	andi.w	#%0000_0111_1111_1000,d0		*  8
	
	;-- 更新位置
	andi.w	#%0000_0011_1111_1111,d2		*  8
	
	;-- テーブルに設定
	lea	_PatNameUpdateTbl(pc),a0		*  8	pc相対なのでﾘﾛｹｰﾄ後もOK
	add.w	d2,d2					*  4
	move.w	d0,(a0,d2.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatNameTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
.if 0
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
.endif
	EndVRAMwrite 93


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 93
	
	rts						* 16
	EndVRAMsetAdr 93


;------
; GRAPHIC 2,3 (SCREEN 2,4)
; Pattern name table - 1Kbyte単位, 1Kbytes(768bytes)
;	+---------+
;	| BLOCK 1 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 2 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 3 | 2Kbytes ($800bytes)
;	+---------+
;	| BLOCK 4 | 2Kbytes ($800bytes)
;	+---------+
;
;	d0.b      ... data
;	regVadr.l ... VRAM address

; BLOCK 4
	VRAMwrite 94
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	move.w	regVadr,d1				*  4
	tst.b	d1					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:	
	;-- PatGenTab のオフセット計算
	lsl.w	#3,d0					* 12
	andi.w	#%0000_0111_1111_1000,d0		*  8
	
	;-- 更新位置
	andi.w	#%0000_0011_1111_1111,d2		*  8
	
	;-- テーブルに設定
	lea	_PatNameUpdateTbl(pc),a0		*  8	pc相対なのでﾘﾛｹｰﾄ後もOK
	add.w	d2,d2					*  4
	move.w	d0,(a0,d2.w)				* 14
	
	;-- 更新フラグセット
	move.w	d2,PatNameTblUpdate(regFtbl)		* 12	d2.w はﾌﾟﾗｽなのでOK
	
	rts						* 16
	
	
1:	;---
	andi.w	#%0011_1111_1111_1111,d1		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
	EndVRAMwrite 94


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 94
	
	rts						* 16
	EndVRAMsetAdr 94


;----------------------------------------------------------------------------------------------
; GRAPHIC 4 (SCREEN 5) - 256x212, 16color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

* PAGE 0
	VRAMwrite 95
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	move.w	regVadr,d2				*  4
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	move.b	d0,d1					*  4
	lsr.w	#4,d0					* 14
	swap.w	d0					*  4
	move.b	d1,d0					*  4
	
	
	;-- グラフィックデータ書き込み
	move.l	d0,(a0)+				* 12
	
	;-- 書き込んだX座標が255の時、次のラインへ
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	lea	256*2(a0),a0				*  8
	
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	;-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_LOW(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 95




;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 95
	
	rts						* 16
	EndVRAMsetAdr 95


;------
; GRAPHIC 4 (SCREEN 5) - 256x212, 16color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 1
	VRAMwrite 96
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	move.w	regVadr,d2				*  4
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	move.b	d0,d1					*  4
	lsr.w	#4,d0					* 14
	swap.w	d0					*  4
	move.b	d1,d0					*  4
	
	
	;-- グラフィックデータ書き込み
	move.l	d0,(a0)+				* 12
	
	;-- 書き込んだX座標が255の時、次のラインへ
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	lea	256*2(a0),a0				*  8
	
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	;-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_HIGH(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 96


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 96
	
	rts						* 16
	EndVRAMsetAdr 96


;------
; GRAPHIC 4 (SCREEN 5) - 256x212, 16color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 2
	VRAMwrite 97
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	move.w	regVadr,d2				*  4
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	move.b	d0,d1					*  4
	lsr.w	#4,d0					* 14
	swap.w	d0					*  4
	move.b	d1,d0					*  4
	
	
	;-- グラフィックデータ書き込み
	move.l	d0,(a0)+				* 12
	
	;-- 書き込んだX座標が255の時、次のラインへ
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	lea	256*2(a0),a0				*  8
	
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	;-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_HIGH(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 97


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 97
	
	rts						* 16
	EndVRAMsetAdr 97


;------
; GRAPHIC 4 (SCREEN 5) - 256x212, 16color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 3
	VRAMwrite 98
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	move.w	regVadr,d2				*  4
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	move.b	d0,d1					*  4
	lsr.w	#4,d0					* 14
	swap.w	d0					*  4
	move.b	d1,d0					*  4
	
	
	;-- グラフィックデータ書き込み
	move.l	d0,(a0)+				* 12
	
	;-- 書き込んだX座標が255の時、次のラインへ
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	lea	256*2(a0),a0				*  8
	
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	;-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	movea.l	VRAM(regFtbl),regVadr			* 16
	move.w	VRAM_BLOCK_LOW(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 98


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 98
	
	rts						* 16
	EndVRAMsetAdr 98



;----------------------------------------------------------------------------------------------
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 0	(VDP_GRA5_PATNAME_P0)
	VRAMwrite 99
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	rol.b	#2,d0					* 10	No.1
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.2
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	rol.b	#2,d0					* 10	No.3
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.4
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	moveq.l	#0,d1					*  4
	
	;-- 書き込んだX座標が511の時、次のラインへ -> 実画面512x512モードのため必要なし
	
	;-- ページ境界を越えた?
	; ３２Ｋバイト単位のため
	move.w	regVadr,d2				*  4
	bmi.s	@f					* 10/ 8 (T/F)
	
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16
@@:
	*-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_LOW(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 99


;---
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	regVadr.l ... VRAM address

; PAGE 0	(VDP_GRA5_PATNAME_P0)
	VRAMsetAdr 99
	lea	GVRAM_16_P0,a0				* 12
	moveq.l	#0,d0					*  4
	move.w	regVadr,d0				*  4
	lsl.l	#3,d0					* 14
	
	adda.l	d0,a0					*  8
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16
	EndVRAMsetAdr 99


;------
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 1
	VRAMwrite 9A
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	rol.b	#2,d0					* 10	No.1
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.2
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	rol.b	#2,d0					* 10	No.3
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.4
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	moveq.l	#0,d1					*  4
	
	;-- 書き込んだX座標が511の時、次のラインへ -> 実画面512x512モードのため必要なし
	
	;-- ページ境界を越えた?
	move.w	regVadr,d2				*  4
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	*-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_HIGH(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 9A


;---
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	regVadr.l ... VRAM address

; PAGE 1
	VRAMsetAdr 9A
	lea	GVRAM_16_P1,a0				* 12
	moveq.l	#0,d0					*  4
	move.w	regVadr,d0				*  4
	lsl.l	#3,d0					* 14
	
	adda.l	d0,a0					*  8
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16
	EndVRAMsetAdr 9A


;------
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 2
	VRAMwrite 9B
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	rol.b	#2,d0					* 10	No.1
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.2
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	rol.b	#2,d0					* 10	No.3
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.4
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	moveq.l	#0,d1					*  4
	
	;-- 書き込んだX座標が511の時、次のラインへ -> 実画面512x512モードのため必要なし
	
	;-- ページ境界を越えた?
	move.w	regVadr,d2				*  4
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	*-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_HIGH(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 9B


;---
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	regVadr.l ... VRAM address

; PAGE 2
	VRAMsetAdr 9B
	lea	GVRAM_16_P2,a0				* 12
	moveq.l	#0,d0					*  4
	move.w	regVadr,d0				*  4
	lsl.l	#3,d0					* 14
	
	adda.l	d0,a0					*  8
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16
	EndVRAMsetAdr 9B


;------
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

; PAGE 3
	VRAMwrite 9C
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	rol.b	#2,d0					* 10	No.1
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.2
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	rol.b	#2,d0					* 10	No.3
	move.b	d0,d1					*  4
	swap.w	d1					*  4
	
	rol.b	#2,d0					* 10	No.4
	move.b	d0,d1					*  4
	
	move.l	d1,(a0)+				* 12
	
	moveq.l	#0,d1					*  4
	
	;-- 書き込んだX座標が511の時、次のラインへ -> 実画面512x512モードのため必要なし
	
	;-- ページ境界を越えた?
	move.w	regVadr,d2				*  4
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	andi.w	#%0111_1111_1111_1111,d2		*  8
	bne	1b
	
	*-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	movea.l	VRAM(regFtbl),regVadr			* 16
	move.w	VRAM_BLOCK_LOW(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 9C


;---
; GRAPHIC 5 (SCREEN 6) - 512x212, 4color
; Pattern name table - 32Kbyte単位, 32Kbytes
;	regVadr.l ... VRAM address

; PAGE 3
	VRAMsetAdr 9C
	lea	GVRAM_16_P3,a0				* 12
	moveq.l	#0,d0					*  4
	move.w	regVadr,d0				*  4
	lsl.l	#3,d0					* 14
	
	adda.l	d0,a0					*  8
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16
	EndVRAMsetAdr 9C



;----------------------------------------------------------------------------------------------
; GRAPHIC 6 (SCREEN 7) - 512x212, 16color
; Pattern name table - 64Kbyte単位, 64Kbytes
;	d0      ... data
;	regVadr ... VRAM address

; PAGE 0
	VRAMwrite 9D
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ作成
	move.b	d0,d1					*  4
	lsr.w	#4,d0					* 14
	swap.w	d0					*  4
	move.b	d1,d0					*  4
	
	;-- グラフィックデータ書き込み
	move.l	d0,(a0)+				* 12
	
	;-- 書き込んだX座標が511の時、次のラインへ -> 実画面512x512モードのため必要なし
	
	;-- ページ境界を越えた?
	move.w	regVadr,d2				*  4
	beq.s	@f					* 10/ 8 (T/F)
	
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

@@:	*-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_HIGH(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 9D


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 9D
	
	rts						* 16
	EndVRAMsetAdr 9D


;------
; PAGE 1
	VRAMwrite 9E
	
	rts						* 16
	EndVRAMwrite 9E


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 9E
	
	rts						* 16
	EndVRAMsetAdr 9E


;----------------------------------------------------------------------------------------------
; GRAPHIC 7 (SCREEN 8) - 256x212, 256color
; Pattern name table - 64Kbyte単位, 64Kbytes
;	d0      ... data
;	regVadr ... VRAM address

; PAGE 0
	VRAMwrite 9F
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- X68kのGRAMアドレス
	movea.l	GRAMadr(regFtbl),a0			* 16
	
	;-- グラフィックデータ書き込み
	move.w	d0,(a0)+				*  8
	
	;-- 次のライン?
	move.w	regVadr,d2				*  4
	tst.b	d2					*  4
	beq.s	2f					* 10/ 8 (T/F)
1:
	;-- X68kのGRAMアドレス保存
	move.l	a0,GRAMadr(regFtbl)			* 16
	
	rts						* 16

2:
	;-- 次のラインへ
	lea	256*2(a0),a0				*  8
	
	;-- ページ境界を越えた?
	tst.w	d2					*  4
	bne.s	1b					* 10/ 8 (T/F)
	
	*-- 次ページへ (regVadr の下位バイトは 0のためそのまま使用)
	move.w	VRAM_BLOCK_HIGH(regZjmp,regVadr.w),d1	* 14
	jmp	VRAM_SET_ADR(regZjmp,d1.l)		* 14
	EndVRAMwrite 9F


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr 9F
	
	rts						* 16
	EndVRAMsetAdr 9F


;------
; PAGE 1
	VRAMwrite A0
	
	rts						* 16
	EndVRAMwrite A0


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A0
	
	rts						* 16
	EndVRAMsetAdr A0


;----------------------------------------------------------------------------------------------
; Sprite mode 1 Attribute table - 128byte単位, 128bytes
;	    +----+
;	  +0|    | <-- ここにある場合の処理
;	    +----+
;	+128|    |
;	    +----+
;	d1.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite A1
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	tst.b	d2					*  4
	bmi.s	@f					* 10/ 8 (T/F)
	
	lea	SprAtrUpdateTbl(regFtbl),a0		*  8
	
	;-- スプライトアトリビュートテーブルのオフセット
	andi.w	#%0000_0000_0111_1100,d2		*  8
	move.w	d2,d0					*  4
	
	lsr.w	#1,d2					*  8
	move.w	d0,(a0,d2.w)				* 
	
	;-- 更新フラグセット
	move.w	d2,SprAtrTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
@@:
	rts						* 16
	EndVRAMwrite A1


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A1
	
	rts						* 16
	EndVRAMsetAdr A1


;------
; Sprite mode 1 Attribute table - 128byte単位, 128bytes
;	    +----+
;	  +0|    |
;	    +----+
;	+128|    | <-- ここにある場合の処理
;	    +----+
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite A2
	move.w	regVadr,d2				*  4
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	tst.b	d2					*  4
	
	;!! 要境界チェック
	bpl.s	@f					* 10/ 8 (T/F)
	
	lea	SprAtrUpdateTbl(regFtbl),a0		*  8
	
	;-- スプライトアトリビュートテーブルのオフセット
	andi.w	#%0000_0000_0111_1100,d2		*  8
	move.w	d2,d0					*  4
	
	lsr.w	#1,d2					*  8
	move.w	d0,(a0,d2.w)				* 
	
	*-- 更新フラグセット
	move.w	d2,SprAtrTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
@@:
	
	rts						* 16
	EndVRAMwrite A2


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A2
	
	rts						* 16
	EndVRAMsetAdr A2


;------
; Sprite mode 1 Pattern generator table - 2Kbyte単位, 2Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite A3
	move.w	regVadr,d2				*  4
	
	;-- SprPatGenTblのオフセットをテーブルに設定
	andi.w	#%0000_0111_1111_1000,d2		*  8
	
	move.w	d2,d1					*  4
	lsr.w	#2,d1					* 10
	
	lea	SprPatGenUpdateTbl(regFtbl),a0		*  8
	move.w	d2,(a0,d1.w)				* 14
	
	;-- データ書き込み
	move.b	d0,(regVadr)+				*  8
	
	;-- 更新フラグセット
	move.w	d2,SprPatGenTblUpdate(regFtbl)		* 12	d2.wはﾌﾟﾗｽ
	
	move.w	regVadr,d0				*  4
	tst.b	d0					*  4
	beq.s	1f					* 10/ 8 (T/F)
2:
	rts
1:
	andi.w	#%0011_1111_1111_1111,d0		*  8
	bne.s	2b					* 10/ 8 (T/F)
	lea	-$4000(regVadr),regVadr			*  8
	bra	2b					* 10
	EndVRAMwrite A3


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A3
	
	rts						* 16
	EndVRAMsetAdr A3


;----------------------------------------------------------------------------------------------
; Sprite mode 2 Attribute table - 1Kbyte単位, 128bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite A4
	
	rts						* 16
	EndVRAMwrite A4


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A4
	
	rts						* 16
	EndVRAMsetAdr A4


;------
; Sprite mode 2 Pattern generator table - 2Kbyte単位, 2Kbytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite A5
	* スプライト用PCGエリア
	lea	(regFtbl),a0				* 12
	
	andi.w	#%0000_0111_1111_1111,d2		*  8
	add.w	d2,d2					*  4
	add.w	d2,d2					*  4
	
	move.b	d1,(regZeroSwap)			*  8
	move.w	(regZeroSwap),d1			*  8
	lea	PCGdatatbl,a1				* 12
	
	move.l	4(a1,d1.w),(a0,d2.w)			* 32
	
	rts						* 16
	EndVRAMwrite A5


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A5
	
	rts						* 16
	EndVRAMsetAdr A5


;------
; Sprite mode 2 Color table - 2Kbyte単位, 512bytes
;	d0.b      ... data
;	regVadr.l ... VRAM address

	VRAMwrite A6
	
	rts						* 16
	EndVRAMwrite A6


;---
;	regVadr.l ... VRAM address
	VRAMsetAdr A6
	
	rts						* 16
	EndVRAMsetAdr A6






*!!! A7 : VDP_GRA1_COLOR_64 で使用
*!!! A8 : VDP_GRA1_COLOR_128 で使用
*!!! A9 : VDP_GRA1_COLOR_192 で使用






*------
	VRAMwrite AA
	
	rts						* 16
	EndVRAMwrite AA


*---
*	d2.l ... VRAM address
	VRAMsetAdr AA
	
	rts						* 16
	EndVRAMsetAdr AA

*------
	VRAMwrite AB
	
	rts						* 16
	EndVRAMwrite AB


*---
*	d2.l ... VRAM address
	VRAMsetAdr AB
	
	rts						* 16
	EndVRAMsetAdr AB

*------
	VRAMwrite AC
	
	rts						* 16
	EndVRAMwrite AC


*---
*	d2.l ... VRAM address
	VRAMsetAdr AC
	
	rts						* 16
	EndVRAMsetAdr AC

*------
	VRAMwrite AD
	
	rts						* 16
	EndVRAMwrite AD


*---
*	d2.l ... VRAM address
	VRAMsetAdr AD
	
	rts						* 16
	EndVRAMsetAdr AD

*------
	VRAMwrite AE
	
	rts						* 16
	EndVRAMwrite AE


*---
*	d2.l ... VRAM address
	VRAMsetAdr AE
	
	rts						* 16
	EndVRAMsetAdr AE

*------
	VRAMwrite AF
	
	rts						* 16
	EndVRAMwrite AF


*---
*	d2.l ... VRAM address
	VRAMsetAdr AF
	
	rts						* 16
	EndVRAMsetAdr AF

*------
	VRAMwrite B0
	
	rts						* 16
	EndVRAMwrite B0


*---
*	d2.l ... VRAM address
	VRAMsetAdr B0
	
	rts						* 16
	EndVRAMsetAdr B0

*------
	VRAMwrite B1
	
	rts						* 16
	EndVRAMwrite B1


*---
*	d2.l ... VRAM address
	VRAMsetAdr B1
	
	rts						* 16
	EndVRAMsetAdr B1

*------
	VRAMwrite B2
	
	rts						* 16
	EndVRAMwrite B2


*---
*	d2.l ... VRAM address
	VRAMsetAdr B2
	
	rts						* 16
	EndVRAMsetAdr B2

*------
	VRAMwrite B3
	
	rts						* 16
	EndVRAMwrite B3


*---
*	d2.l ... VRAM address
	VRAMsetAdr B3
	
	rts						* 16
	EndVRAMsetAdr B3

*------
	VRAMwrite B4
	
	rts						* 16
	EndVRAMwrite B4


*---
*	d2.l ... VRAM address
	VRAMsetAdr B4
	
	rts						* 16
	EndVRAMsetAdr B4

*------
	VRAMwrite B5
	
	rts						* 16
	EndVRAMwrite B5


*---
*	d2.l ... VRAM address
	VRAMsetAdr B5
	
	rts						* 16
	EndVRAMsetAdr B5

*------
	VRAMwrite B6
	
	rts						* 16
	EndVRAMwrite B6


*---
*	d2.l ... VRAM address
	VRAMsetAdr B6
	
	rts						* 16
	EndVRAMsetAdr B6

*------
	VRAMwrite B7
	
	rts						* 16
	EndVRAMwrite B7


*---
*	d2.l ... VRAM address
	VRAMsetAdr B7
	
	rts						* 16
	EndVRAMsetAdr B7

*------
	VRAMwrite B8
	
	rts						* 16
	EndVRAMwrite B8


*---
*	d2.l ... VRAM address
	VRAMsetAdr B8
	
	rts						* 16
	EndVRAMsetAdr B8

*------
	VRAMwrite B9
	
	rts						* 16
	EndVRAMwrite B9


*---
*	d2.l ... VRAM address
	VRAMsetAdr B9
	
	rts						* 16
	EndVRAMsetAdr B9

*------
	VRAMwrite BA
	
	rts						* 16
	EndVRAMwrite BA


*---
*	d2.l ... VRAM address
	VRAMsetAdr BA
	
	rts						* 16
	EndVRAMsetAdr BA

*------
	VRAMwrite BB
	
	rts						* 16
	EndVRAMwrite BB


*---
*	d2.l ... VRAM address
	VRAMsetAdr BB
	
	rts						* 16
	EndVRAMsetAdr BB

*------
	VRAMwrite BC
	
	rts						* 16
	EndVRAMwrite BC


*---
*	d2.l ... VRAM address
	VRAMsetAdr BC
	
	rts						* 16
	EndVRAMsetAdr BC

*------
	VRAMwrite BD
	
	rts						* 16
	EndVRAMwrite BD


*---
*	d2.l ... VRAM address
	VRAMsetAdr BD
	
	rts						* 16
	EndVRAMsetAdr BD

*------
	VRAMwrite BE
	
	rts						* 16
	EndVRAMwrite BE


*---
*	d2.l ... VRAM address
	VRAMsetAdr BE
	
	rts						* 16
	EndVRAMsetAdr BE

*------
	VRAMwrite BF
	
	rts						* 16
	EndVRAMwrite BF


*---
*	d2.l ... VRAM address
	VRAMsetAdr BF
	
	rts						* 16
	EndVRAMsetAdr BF

*------
	VRAMwrite C0
	
	rts						* 16
	EndVRAMwrite C0


*---
*	d2.l ... VRAM address
	VRAMsetAdr C0
	
	rts						* 16
	EndVRAMsetAdr C0

*------
	VRAMwrite C1
	
	rts						* 16
	EndVRAMwrite C1


*---
*	d2.l ... VRAM address
	VRAMsetAdr C1
	
	rts						* 16
	EndVRAMsetAdr C1

*------
	VRAMwrite C2
	
	rts						* 16
	EndVRAMwrite C2


*---
*	d2.l ... VRAM address
	VRAMsetAdr C2
	
	rts						* 16
	EndVRAMsetAdr C2

*------
	VRAMwrite C3
	
	rts						* 16
	EndVRAMwrite C3


*---
*	d2.l ... VRAM address
	VRAMsetAdr C3
	
	rts						* 16
	EndVRAMsetAdr C3

*------
	VRAMwrite C4
	
	rts						* 16
	EndVRAMwrite C4


*---
*	d2.l ... VRAM address
	VRAMsetAdr C4
	
	rts						* 16
	EndVRAMsetAdr C4

*------
	VRAMwrite C5
	
	rts						* 16
	EndVRAMwrite C5


*---
*	d2.l ... VRAM address
	VRAMsetAdr C5
	
	rts						* 16
	EndVRAMsetAdr C5

*------
	VRAMwrite C6
	
	rts						* 16
	EndVRAMwrite C6


*---
*	d2.l ... VRAM address
	VRAMsetAdr C6
	
	rts						* 16
	EndVRAMsetAdr C6

*------
	VRAMwrite C7
	
	rts						* 16
	EndVRAMwrite C7


*---
*	d2.l ... VRAM address
	VRAMsetAdr C7
	
	rts						* 16
	EndVRAMsetAdr C7

*------
	VRAMwrite C8
	
	rts						* 16
	EndVRAMwrite C8


*---
*	d2.l ... VRAM address
	VRAMsetAdr C8
	
	rts						* 16
	EndVRAMsetAdr C8

*------
	VRAMwrite C9
	
	rts						* 16
	EndVRAMwrite C9


*---
*	d2.l ... VRAM address
	VRAMsetAdr C9
	
	rts						* 16
	EndVRAMsetAdr C9

*------
	VRAMwrite CA
	
	rts						* 16
	EndVRAMwrite CA


*---
*	d2.l ... VRAM address
	VRAMsetAdr CA
	
	rts						* 16
	EndVRAMsetAdr CA

*------
	VRAMwrite CB
	
	rts						* 16
	EndVRAMwrite CB


*---
*	d2.l ... VRAM address
	VRAMsetAdr CB
	
	rts						* 16
	EndVRAMsetAdr CB

*------
	VRAMwrite CC
	
	rts						* 16
	EndVRAMwrite CC


*---
*	d2.l ... VRAM address
	VRAMsetAdr CC
	
	rts						* 16
	EndVRAMsetAdr CC

*------
	VRAMwrite CD
	
	rts						* 16
	EndVRAMwrite CD


*---
*	d2.l ... VRAM address
	VRAMsetAdr CD
	
	rts						* 16
	EndVRAMsetAdr CD

*------
	VRAMwrite CE
	
	rts						* 16
	EndVRAMwrite CE


*---
*	d2.l ... VRAM address
	VRAMsetAdr CE
	
	rts						* 16
	EndVRAMsetAdr CE

*------
	VRAMwrite CF
	
	rts						* 16
	EndVRAMwrite CF


*---
*	d2.l ... VRAM address
	VRAMsetAdr CF
	
	rts						* 16
	EndVRAMsetAdr CF

*------
	VRAMwrite D0
	
	rts						* 16
	EndVRAMwrite D0


*---
*	d2.l ... VRAM address
	VRAMsetAdr D0
	
	rts						* 16
	EndVRAMsetAdr D0

*------
	VRAMwrite D1
	
	rts						* 16
	EndVRAMwrite D1


*---
*	d2.l ... VRAM address
	VRAMsetAdr D1
	
	rts						* 16
	EndVRAMsetAdr D1

*------
	VRAMwrite D2
	
	rts						* 16
	EndVRAMwrite D2


*---
*	d2.l ... VRAM address
	VRAMsetAdr D2
	
	rts						* 16
	EndVRAMsetAdr D2

*------
	VRAMwrite D3
	
	rts						* 16
	EndVRAMwrite D3


*---
*	d2.l ... VRAM address
	VRAMsetAdr D3
	
	rts						* 16
	EndVRAMsetAdr D3

*------
	VRAMwrite D4
	
	rts						* 16
	EndVRAMwrite D4


*---
*	d2.l ... VRAM address
	VRAMsetAdr D4
	
	rts						* 16
	EndVRAMsetAdr D4

*------
	VRAMwrite D5
	
	rts						* 16
	EndVRAMwrite D5


*---
*	d2.l ... VRAM address
	VRAMsetAdr D5
	
	rts						* 16
	EndVRAMsetAdr D5

*------
	VRAMwrite D6
	
	rts						* 16
	EndVRAMwrite D6


*---
*	d2.l ... VRAM address
	VRAMsetAdr D6
	
	rts						* 16
	EndVRAMsetAdr D6

*------
	VRAMwrite D7
	
	rts						* 16
	EndVRAMwrite D7


*---
*	d2.l ... VRAM address
	VRAMsetAdr D7
	
	rts						* 16
	EndVRAMsetAdr D7

*------
	VRAMwrite D8
	
	rts						* 16
	EndVRAMwrite D8


*---
*	d2.l ... VRAM address
	VRAMsetAdr D8
	
	rts						* 16
	EndVRAMsetAdr D8

*------
	VRAMwrite D9
	
	rts						* 16
	EndVRAMwrite D9


*---
*	d2.l ... VRAM address
	VRAMsetAdr D9
	
	rts						* 16
	EndVRAMsetAdr D9

*------
	VRAMwrite DA
	
	rts						* 16
	EndVRAMwrite DA


*---
*	d2.l ... VRAM address
	VRAMsetAdr DA
	
	rts						* 16
	EndVRAMsetAdr DA

*------
	VRAMwrite DB
	
	rts						* 16
	EndVRAMwrite DB


*---
*	d2.l ... VRAM address
	VRAMsetAdr DB
	
	rts						* 16
	EndVRAMsetAdr DB

*------
	VRAMwrite DC
	
	rts						* 16
	EndVRAMwrite DC


*---
*	d2.l ... VRAM address
	VRAMsetAdr DC
	
	rts						* 16
	EndVRAMsetAdr DC

*------
	VRAMwrite DD
	
	rts						* 16
	EndVRAMwrite DD


*---
*	d2.l ... VRAM address
	VRAMsetAdr DD
	
	rts						* 16
	EndVRAMsetAdr DD

*------
	VRAMwrite DE
	
	rts						* 16
	EndVRAMwrite DE


*---
*	d2.l ... VRAM address
	VRAMsetAdr DE
	
	rts						* 16
	EndVRAMsetAdr DE

*------
	VRAMwrite DF
	
	rts						* 16
	EndVRAMwrite DF


*---
*	d2.l ... VRAM address
	VRAMsetAdr DF
	
	rts						* 16
	EndVRAMsetAdr DF

*------
	VRAMwrite E0
	
	rts						* 16
	EndVRAMwrite E0


*---
*	d2.l ... VRAM address
	VRAMsetAdr E0
	
	rts						* 16
	EndVRAMsetAdr E0

*------
	VRAMwrite E1
	
	rts						* 16
	EndVRAMwrite E1


*---
*	d2.l ... VRAM address
	VRAMsetAdr E1
	
	rts						* 16
	EndVRAMsetAdr E1

*------
	VRAMwrite E2
	
	rts						* 16
	EndVRAMwrite E2


*---
*	d2.l ... VRAM address
	VRAMsetAdr E2
	
	rts						* 16
	EndVRAMsetAdr E2

*------
	VRAMwrite E3
	
	rts						* 16
	EndVRAMwrite E3


*---
*	d2.l ... VRAM address
	VRAMsetAdr E3
	
	rts						* 16
	EndVRAMsetAdr E3

*------
	VRAMwrite E4
	
	rts						* 16
	EndVRAMwrite E4


*---
*	d2.l ... VRAM address
	VRAMsetAdr E4
	
	rts						* 16
	EndVRAMsetAdr E4

*------
	VRAMwrite E5
	
	rts						* 16
	EndVRAMwrite E5


*---
*	d2.l ... VRAM address
	VRAMsetAdr E5
	
	rts						* 16
	EndVRAMsetAdr E5

*------
	VRAMwrite E6
	
	rts						* 16
	EndVRAMwrite E6


*---
*	d2.l ... VRAM address
	VRAMsetAdr E6
	
	rts						* 16
	EndVRAMsetAdr E6

*------
	VRAMwrite E7
	
	rts						* 16
	EndVRAMwrite E7


*---
*	d2.l ... VRAM address
	VRAMsetAdr E7
	
	rts						* 16
	EndVRAMsetAdr E7

*------
	VRAMwrite E8
	
	rts						* 16
	EndVRAMwrite E8


*---
*	d2.l ... VRAM address
	VRAMsetAdr E8
	
	rts						* 16
	EndVRAMsetAdr E8

*------
	VRAMwrite E9
	
	rts						* 16
	EndVRAMwrite E9


*---
*	d2.l ... VRAM address
	VRAMsetAdr E9
	
	rts						* 16
	EndVRAMsetAdr E9

*------
	VRAMwrite EA
	
	rts						* 16
	EndVRAMwrite EA


*---
*	d2.l ... VRAM address
	VRAMsetAdr EA
	
	rts						* 16
	EndVRAMsetAdr EA

*------
	VRAMwrite EB
	
	rts						* 16
	EndVRAMwrite EB


*---
*	d2.l ... VRAM address
	VRAMsetAdr EB
	
	rts						* 16
	EndVRAMsetAdr EB

*------
	VRAMwrite EC
	
	rts						* 16
	EndVRAMwrite EC


*---
*	d2.l ... VRAM address
	VRAMsetAdr EC
	
	rts						* 16
	EndVRAMsetAdr EC

*------
	VRAMwrite ED
	
	rts						* 16
	EndVRAMwrite ED


*---
*	d2.l ... VRAM address
	VRAMsetAdr ED
	
	rts						* 16
	EndVRAMsetAdr ED

*------
	VRAMwrite EE
	
	rts						* 16
	EndVRAMwrite EE


*---
*	d2.l ... VRAM address
	VRAMsetAdr EE
	
	rts						* 16
	EndVRAMsetAdr EE

*------
	VRAMwrite EF
	
	rts						* 16
	EndVRAMwrite EF


*---
*	d2.l ... VRAM address
	VRAMsetAdr EF
	
	rts						* 16
	EndVRAMsetAdr EF

*------
	VRAMwrite F0
	
	rts						* 16
	EndVRAMwrite F0


*---
*	d2.l ... VRAM address
	VRAMsetAdr F0
	
	rts						* 16
	EndVRAMsetAdr F0

*------
	VRAMwrite F1
	
	rts						* 16
	EndVRAMwrite F1


*---
*	d2.l ... VRAM address
	VRAMsetAdr F1
	
	rts						* 16
	EndVRAMsetAdr F1

*------
	VRAMwrite F2
	
	rts						* 16
	EndVRAMwrite F2


*---
*	d2.l ... VRAM address
	VRAMsetAdr F2
	
	rts						* 16
	EndVRAMsetAdr F2

*------
	VRAMwrite F3
	
	rts						* 16
	EndVRAMwrite F3


*---
*	d2.l ... VRAM address
	VRAMsetAdr F3
	
	rts						* 16
	EndVRAMsetAdr F3

*------
	VRAMwrite F4
	
	rts						* 16
	EndVRAMwrite F4


*---
*	d2.l ... VRAM address
	VRAMsetAdr F4
	
	rts						* 16
	EndVRAMsetAdr F4

*------
	VRAMwrite F5
	
	rts						* 16
	EndVRAMwrite F5


*---
*	d2.l ... VRAM address
	VRAMsetAdr F5
	
	rts						* 16
	EndVRAMsetAdr F5

*------
	VRAMwrite F6
	
	rts						* 16
	EndVRAMwrite F6


*---
*	d2.l ... VRAM address
	VRAMsetAdr F6
	
	rts						* 16
	EndVRAMsetAdr F6

*------
	VRAMwrite F7
	
	rts						* 16
	EndVRAMwrite F7


*---
*	d2.l ... VRAM address
	VRAMsetAdr F7
	
	rts						* 16
	EndVRAMsetAdr F7

*------
	VRAMwrite F8
	
	rts						* 16
	EndVRAMwrite F8


*---
*	d2.l ... VRAM address
	VRAMsetAdr F8
	
	rts						* 16
	EndVRAMsetAdr F8

*------
	VRAMwrite F9
	
	rts						* 16
	EndVRAMwrite F9


*---
*	d2.l ... VRAM address
	VRAMsetAdr F9
	
	rts						* 16
	EndVRAMsetAdr F9

*------
	VRAMwrite FA
	
	rts						* 16
	EndVRAMwrite FA


*---
*	d2.l ... VRAM address
	VRAMsetAdr FA
	
	rts						* 16
	EndVRAMsetAdr FA

*------
	VRAMwrite FB
	
	rts						* 16
	EndVRAMwrite FB


*---
*	d2.l ... VRAM address
	VRAMsetAdr FB
	
	rts						* 16
	EndVRAMsetAdr FB

*------
	VRAMwrite FC
	
	rts						* 16
	EndVRAMwrite FC


*---
*	d2.l ... VRAM address
	VRAMsetAdr FC
	
	rts						* 16
	EndVRAMsetAdr FC

*------
	VRAMwrite FD
	
	rts						* 16
	EndVRAMwrite FD


*---
*	d2.l ... VRAM address
	VRAMsetAdr FD
	
	rts						* 16
	EndVRAMsetAdr FD

*------
	VRAMwrite FE
	
	rts						* 16
	EndVRAMwrite FE


*---
*	d2.l ... VRAM address
	VRAMsetAdr FE
	
	rts						* 16
	EndVRAMsetAdr FE

*------
	VRAMwrite FF
	
	rts						* 16
	EndVRAMwrite FF


*---
*	d2.l ... VRAM address
	VRAMsetAdr FF
	
	rts						* 16
	EndVRAMsetAdr FF

