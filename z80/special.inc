;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;
;
; 特殊命令



	SET_OFFSET	SlotTable
	SET_OFFSET	SlotTablePage0
	SET_OFFSET	SlotTablePage1
	SET_OFFSET	SlotTablePage2
	SET_OFFSET	SlotTablePage3

	SET_OFFSET	regSave
	SET_OFFSET	Z80_DIEI



;----------------------------------------------------------------------------------------------
; 指定ｽﾛｯﾄから１ﾊﾞｲﾄ読み出す
;
; IN:	A:  ｽﾛｯﾄ指定
;	HL: ｱﾄﾞﾚｽ
; OUT:	A:  読み出した値
; BREAK:AF,BC,DE

_Special_RDSLT:
	;-- 読み込みﾍﾟｰｼﾞ
	lea	SlotTable(regFtbl),a0			*  8
	move.w	regHL,d0				*  4
	andi.w	#%1100_0000_0000_0000,d0		*  8
	lsr.w	#6,d0					* 
	adda.w	d0,a0					*  8
	
	;--
	cmpi.w	#%0000_0000_0000_0000,d0		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	SlotPage0(regZjmp),a1			*  8
	bra	1f					* 10
	
@@:	cmpi.w	#%0000_0001_0000_0000,d0		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	SlotPage1(regZjmp),a1			*  8
	bra	1f					* 10

@@:	cmpi.w	#%0000_0010_0000_0000,d0		*  8
	bne.s	@f					* 10/ 8 (T/F)
	lea	SlotPage2(regZjmp),a1			*  8
	bra	1f					* 10
@@:
	lea	SlotPage3(regZjmp),a1			*  8
1:
	
	;-- 読み込み基本ｽﾛｯﾄ
	moveq.l	#%0000_0011,d0				*  4
	and.b	regA,d0					*  4
	ror.b	#2,d0					* 
	
	;-- 読み込み拡張ｽﾛｯﾄ
	moveq.l	#%0000_1100,d1				*  4
	and.b	regA,d1					*  4
	lsl.b	#2,d1					* 
	
	;-- 指定ｽﾛｯﾄ(基本)は表に出ているか?
	cmp.w	(a1),d0					* 
	bne.s	@f					* 
	
	;-- 指定ｽﾛｯﾄ(拡張)は表に出ているか?
	cmp.w	(a0,d0.w),d1				* 
	bne.s	@f					* 
	
	;-- 表のﾃﾞｰﾀを読み込む
	movea.l	regHL,a0				* 
	move.b	(a0),regA				* 
	
	FETCH						* 
	
	
	;-- 裏のｽﾛｯﾄから読み込む
@@:
	;-- ｽﾛｯﾄ管理ﾃｰﾌﾞﾙのｱﾄﾞﾚｽ
	or.b	d1,d0					*  4
	adda.w	d0,a0					*  8
	
	;-- 未実装ﾒﾓﾘの場合は読み込まない
	tst.b	SLOTINFO_MEMTYPE(a0)			* 
	bmi.s	@f					* 
	

;!!!!! 8KB 単位に分割したため、そのチェックが必要

	;-- ﾒﾓﾘのｱﾄﾞﾚｽ
	movea.l	SLOTINFO_MEMADR_Low(a0),a1		* 
	
	;-- ｵﾌｾｯﾄ計算
	move.w	regHL,d0				*  4
	andi.w	#%0011_1111_1111_1111,d0		*  8
	
	;-- 読み込む
	move.b	(a1,d0.w),regA				* 
	
@@:
	FETCH						* 


