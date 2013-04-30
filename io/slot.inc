;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 teknobow
;


	.xdef _SegmentsMemMap,_MemoryMapperTbl
	.xdef _IndexMemoryMapper
	
	SET_OFFSET_IO	IndexMemoryMapper

	.even


*----------------------------------------------------------------------------------------------

*-- メモリマッパのセグメント数
_SegmentsMemMap:
	.dc.w	0

	.rept 128
	.dc.l	0
	.endm
_MemoryMapperTbl:
	.rept 128
	.dc.l	0
	.endm

;-- スロット管理テーブルのメモリマッパの位置
_IndexMemoryMapper:
	.dc.w	0


