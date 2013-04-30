#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 nir
#


CC = gcc2

BINDIR_000	= ../bin/000
BINDIR_030	= ../bin/030

WORKDIR		= ../work

WORK_SHARE	= $(WORKDIR)/share
WORK_000	= $(WORKDIR)/000
WORK_030	= $(WORKDIR)/030

TARGET_SHARE =	

TARGET_000 =	$(BINDIR_000)/slot.drv \
		$(BINDIR_000)/slot_mmu.drv \
		$(BINDIR_000)/slot_dma.drv

TARGET_030 =	



ALL : $(TARGET_SHARE) $(TARGET_000) $(TARGET_030)


#
# slot.drv
#
$(BINDIR_000)/slot.drv : $(WORK_000)/slot.r
	cp -f $(WORK_000)/slot.r $(BINDIR_000)/slot.drv

$(WORK_000)/slot.r : $(WORK_000)/slot.x
	-(cd $(WORK_000); cv /r slot.x)

$(WORK_000)/slot.x : $(WORK_000)/slot.o
	hlk $(WORK_000)/slot.o -o $(WORK_000)/slot.x

$(WORK_000)/slot.o : slot.has ../startup/version.equ \
		../z80/z80emu.equ ../io/slot.equ \
		$(WORK_000)/flagtbl.equ
	has -w -m68000 -s MPU=68000 slot.has -o $(WORK_000)/slot.o


#
# slot_mmu.drv
#
$(BINDIR_000)/slot_mmu.drv : $(WORK_000)/slot_mmu.r
	cp -f $(WORK_000)/slot_mmu.r $(BINDIR_000)/slot_mmu.drv

$(WORK_000)/slot_mmu.r : $(WORK_000)/slot_mmu.x
	-(cd $(WORK_000); cv /r slot_mmu.x)

$(WORK_000)/slot_mmu.x : $(WORK_000)/slot_mmu.o
	hlk $(WORK_000)/slot_mmu.o -o $(WORK_000)/slot_mmu.x

$(WORK_000)/slot_mmu.o : slot_mmu.has ../startup/version.equ \
		../z80/z80emu.equ ../io/slot.equ \
		$(WORK_000)/flagtbl.equ
	has -w -m68000 -s MPU=68000 slot_mmu.has -o $(WORK_000)/slot_mmu.o


#
# slot_dma.drv
#
$(BINDIR_000)/slot_dma.drv : $(WORK_000)/slot_dma.r
	cp -f $(WORK_000)/slot_dma.r $(BINDIR_000)/slot_dma.drv

$(WORK_000)/slot_dma.r : $(WORK_000)/slot_dma.x
	-(cd $(WORK_000); cv /r slot_dma.x)

$(WORK_000)/slot_dma.x : $(WORK_000)/slot_dma.o
	hlk $(WORK_000)/slot_dma.o -o $(WORK_000)/slot_dma.x

$(WORK_000)/slot_dma.o : slot_dma.has ../startup/version.equ \
		../z80/z80emu.equ ../io/slot.equ \
		$(WORK_000)/flagtbl.equ
	has -w -m68000 -s MPU=68000 slot_dma.has -o $(WORK_000)/slot_dma.o


#
# CLEAN
#
clean :
	-rm -f $(WORK_000)/slot_dma.o
	-rm -f $(WORK_000)/slot_dma.x
	-rm -f $(WORK_000)/slot_dma.r
	-rm -f $(BINDIR_000)/slot_dma.drv
	-rm -f $(WORK_000)/slot_mmu.o
	-rm -f $(WORK_000)/slot_mmu.x
	-rm -f $(WORK_000)/slot_mmu.r
	-rm -f $(BINDIR_000)/slot_mmu.drv
	-rm -f $(WORK_000)/slot.o
	-rm -f $(WORK_000)/slot.x
	-rm -f $(WORK_000)/slot.r
	-rm -f $(BINDIR_000)/slot.drv
