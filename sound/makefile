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

TARGET_000 =	$(BINDIR_000)/psg.drv \
		$(BINDIR_000)/scc.drv

TARGET_030 =	



ALL : $(TARGET_SHARE) $(TARGET_000) $(TARGET_030)



$(BINDIR_000)/psg.drv : $(WORK_000)/psg.r
	cp -f $(WORK_000)/psg.r $(BINDIR_000)/psg.drv

$(WORK_000)/psg.r : $(WORK_000)/psg.x
	-(cd $(WORK_000); cv /r psg.x)

$(WORK_000)/psg.x : $(WORK_000)/psg.o
	hlk $(WORK_000)/psg.o -o $(WORK_000)/psg.x

$(WORK_000)/psg.o : psg.has ../startup/version.equ \
		../z80/z80emu.equ ../io/x68k.equ \
		$(WORK_000)/flagtbl.equ \
		$(WORK_SHARE)/psg2opm.inc
	has -w -m68000 -s MPU=68000 psg.has -o $(WORK_000)/psg.o


$(BINDIR_000)/scc.drv : $(WORK_000)/scc.r
	cp -f $(WORK_000)/scc.r $(BINDIR_000)/scc.drv

$(WORK_000)/scc.r : $(WORK_000)/scc.x
	-(cd $(WORK_000); cv /r scc.x)

$(WORK_000)/scc.x : $(WORK_000)/scc.o
	hlk $(WORK_000)/scc.o -o $(WORK_000)/scc.x

$(WORK_000)/scc.o : scc.has ../startup/version.equ \
		../z80/z80emu.equ ../IO/x68k.equ \
		$(WORK_000)/flagtbl.equ \
		$(WORK_SHARE)/psg2opm.inc
	has -w -m68000 -s MPU=68000 scc.has -o $(WORK_000)/scc.o


#
# SHARE
#
$(WORK_SHARE)/psg2opm.inc : conv.pl
	jperl conv.pl $(WORK_SHARE)


#
# CLEAN
#
clean : 
	-rm -f $(WORK_SHARE)/psg2opm.inc
	-rm -f $(WORK_000)/scc.o
	-rm -f $(WORK_000)/scc.x
	-rm -f $(WORK_000)/scc.r
	-rm -f $(WORK_000)/scc.drv
	-rm -f $(BINDIR_000)/scc.drv
	-rm -f $(WORK_000)/psg.o
	-rm -f $(WORK_000)/psg.x
	-rm -f $(WORK_000)/psg.r
	-rm -f $(WORK_000)/psg.drv
	-rm -f $(BINDIR_000)/psg.drv

