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

TARGET_000 =	$(BINDIR_000)/emes.x

TARGET_030 =	



ALL : $(TARGET_SHARE) $(TARGET_000) $(TARGET_030)


$(BINDIR_000)/emes.x : $(WORK_000)/emes.ox
	cp -f $(WORK_000)/emes.x $(BINDIR_000)/emes.x

$(WORK_000)/emes.ox : $(WORK_000)/emes.x
	(cd $(WORK_000); lzxh emes.x)

$(WORK_000)/emes.x : $(WORK_000)/startup.o emes_000.ind \
		$(WORK_000)/z80emu.o $(WORK_000)/flagtbl.o \
		$(WORK_000)/io.o $(WORK_000)/scrupdate.o $(WORK_000)/relextslot.o \
		$(WORK_SHARE)/ftbl8.o $(WORK_SHARE)/pcg.o $(WORK_SHARE)/pcgtbl.o \
		$(WORK_SHARE)/_debugger.o $(WORK_SHARE)/disasmtab.o
	hlk -v -o $(WORK_000)/emes.x -p $(WORK_000)/emes.map -i emes_000.ind

$(WORK_000)/startup.o : startup.has version.equ \
		../z80/z80emu.equ ../io/slot.equ ../io/x68k.equ \
		$(WORK_000)/z80emusize.equ
	has -w -m68000 -s MPU=68000 startup.has -o $(WORK_000)/startup.o


#
# CLEAN
#
clean :
	-rm -f $(WORK_000)/startup.o
	-rm -f $(WORK_000)/emes.x
	-rm -f $(WORK_000)/emes.map
	-rm -f $(WORK_000)/emes.ox
	-rm -f $(BINDIR_000)/emes.x

