;----------------------------------------------------------------------------------------------
; MSX Emulator for X680x0 - emes.x
;
;    Copyright 1997-1998 nir
;



	.xdef	NMIroutine,COPYinterrupt
	.xdef	_interrupt_inDebugger
	.xdef	_Org_NMIintr,_Org_COPYintr
	.xdef	_breakPoint,_breakPoint_1,_breakPoint_2

	.xref	_Z80debugger
	.xref	exitMSXemu



;----------------------------------------------------------------------------------------------
	SET_OFFSET	breakPoint
	SET_OFFSET	w_breakPoint
	SET_OFFSET	interrupt_inDebugger
	SET_OFFSET	Z80intrFlag
	SET_OFFSET	terminateFlag
	SET_OFFSET	Z80INTREQ



;----------------------------------------------------------------------------------------------
; �EregPC �͖��߂̑��݂���A�h���X���� +1 ����Ă����ԂŌĂ΂��.
; �E�u���C�N�|�C���g�� $0000 �` $fffe �͈̔͂Őݒ�\.
; �E�u���C�N�|�C���g�����ݒ�̏ꍇ�́A�X�e�b�v���s.
; �EMSXemu ���I��������ɂ́Aw_breakPoint �𖢐ݒ�ɂ��AtermineteFlag ���O�ȊO�ɐݒ�.
; �E�X�e�b�v���s�̏ꍇ�͊��荞�݂͔������Ȃ�.�i�ۗ������j

_exception:
	;-- �u���C�N�|�C���g(�Q�ӏ��ݒ�j
	move.l	w_breakPoint(regFtbl),d0	; 16
	
	;-- �u���C�N�|�C���g���ݒ肳��Ă��Ȃ��ꍇ�̓f�o�b�K��
	beq.s	enterDebugger2			; 10/ 8 (T/F)
	
	;-- Break Point #1
	cmp.w	regPC,d0			;  4
	beq.s	enterDebugger2			; 10/ 8 (T/F)
	
	;-- Break Point #2
	swap.w	d0				;  4
	cmp.w	regPC,d0			;  4
	beq.s	enterDebugger2			; 10/ 8 (T/F)
	
	;-- ���荞�݃t�F�b�`
	tas	interrupt_inDebugger(regFtbl)	; 18
	bpl.s	@f				; 10/ 8 (T/F)
	
	;-- �ʏ�̎��s
	move.b	#8,d1				;  8
	jmp	OP_Z(regZjmp,d1.w)		; 14
	
@@:	;-- ���荞�ݔ���
	subq.w	#1,regPC			;  8
	move.w	#$ff08,d1			;  8
	jmp	OP_Z(regZjmp,d1.w)		; 14



;----------------------------------------------------------------------------------------------
; d0 : work		a0 : work
; d1 : work		a1 : work
; d2 : regAdr		a2 : regVadr
; d3 : regA		a3 : regIO
; d4 : regF		a4 : regZjmp
; d5 : regBC		a5 : regPC
; d6 : regDE		a6 : regSP
; d7 : regHL		a7 : regFtbl (ssp)

_enterDebugger:
	move.b	#IN_Z80DEBUGGER,Z80INTREQ(regFtbl)
	move.b	#IN_Z80DEBUGGER,Z80intrFlag(regFtbl)
	
enterDebugger2:
	;-- �I���`�F�b�N
	tst.w	terminateFlag(regFtbl)
	beq	@f
	jmp	exitMSXemu
@@:	
	
	;--
	subq.w	#1,regPC
	
	;-- �f�o�b�K�Ăт���
	movem.l	d3-d7/a2-a6,-(sp)
	jsr	_Z80debugger
	movem.l	(sp)+,d3-d7/a2-a6
	
	;--
	moveq.l	#0,d1
	move.l	regHL,regAdr
	
	;--
	; �߂�l:
	;	0:�X�e�b�v���s
	;	1:�u���C�N�|�C���g������s
	;	2:�u���C�N�|�C���g�Ȃ����s
	;	3:trap #9 ���s
	subq.w	#1,d0
	bmi.s	exec_0			; [0]
	beq.s	exec_1			; [1]
	subq.w	#1,d0
	beq.s	exec_2			; [2]
	
	
	;-- [3] trap #9 ���s
	move.b	(regPC)+,(regOP1fetch)
	move.w	(regOP1fetch),d1
	move.b	#8,d1
	
	trap	#9
	
	jmp	OP_Z(regZjmp,d1.w)
	
	
	;-- [2] �u���C�N�|�C���g�Ȃ����s
exec_2:
	move.b	#$04,Z80INTREQ(regFtbl)
	move.w	#$08,(regOP1fetch)
	
	FETCH
	
	
	;-- [1] �u���C�N�|�C���g������s
exec_1:
	move.l	breakPoint(regFtbl),w_breakPoint(regFtbl)
	
	FETCH
	
	
	;-- [0] �X�e�b�v���s
exec_0:
	clr.l	w_breakPoint(regFtbl)
	
	move.b	(regPC)+,(regOP1fetch)
	move.w	(regOP1fetch),d1
	move.b	#8,d1
	jmp	OP_Z(regZjmp,d1.w)



*----------------------------------------------------------------------------------------------
* �C���^���v�g�X�C�b�`����
* Z80�f�o�b�K�̋N��
NMIroutine:
	move.b	#IN_Z80DEBUGGER,(_Z80INTREQ)
	move.b	#IN_Z80DEBUGGER,(_Z80intrFlag)
	
	clr.l	(_w_breakPoint)
	
	bset.b	#2,(SYSPORT_4)
	
	rte



*----------------------------------------------------------------------------------------------
* �R�s�[�L�[�C���^���v�g����
* Z80�f�o�b�K�̋N��
COPYinterrupt:
	move.b	#IN_Z80DEBUGGER,(_Z80INTREQ)
	move.b	#IN_Z80DEBUGGER,(_Z80intrFlag)
	
	clr.l	(_w_breakPoint)
	


	st.b	(_terminateFlag)


	
	rte



;----------------------------------------------------------------------------------------------
	.even

_interrupt_inDebugger:	.dc.w	-1

;-- �u���C�N�|�C���g
_breakPoint:		
_breakPoint_2:		.dc.w	0
_breakPoint_1:		.dc.w	0


_Org_NMIintr:		.dc.l	0
_Org_COPYintr:		.dc.l	0