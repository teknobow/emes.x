#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 teknobow
#


CC = gcc2
PERL = jperl

WORKDIR		= ../work

WORK_SHARE	= $(WORKDIR)/share
WORK_000	= $(WORKDIR)/000
WORK_030	= $(WORKDIR)/030

TARGET_SHARE =	$(WORK_SHARE)/pcgtbl.o \
		$(WORK_SHARE)/pcg.o

TARGET_000 =	$(WORK_000)/io.o \
		$(WORK_000)/scrupdate.o

TARGET_030 =	



ALL : $(TARGET_SHARE) $(TARGET_000) $(TARGET_030)

$(WORK_000)/io.o : io.pl io.has io.inc io.equ ../Z80/z80emu.equ \
		$(WORK_000)/flagtbl.equ \
		vdp.equ x68k.equ
	$(PERL) io.pl m68000 $(WORK_000)

$(WORK_000)/scrupdate.o : scrupdate.has gra2test.has \
			x68k.equ ../Z80/z80emu.equ \
			$(WORK_000)/flagtbl.equ
	has -w -m68000 -s MPU=68000 scrupdate.has -o $(WORK_000)/scrupdate.o



#
# SHARE
#

$(WORK_SHARE)/pcg.o : pcg.has
	has -m68000 -w pcg.has -o $(WORK_SHARE)/pcg.o

$(WORK_SHARE)/pcgtbl.o : $(WORK_SHARE)/pcgtbl.has
	has -m68000 -w $(WORK_SHARE)/pcgtbl.has -o $(WORK_SHARE)/pcgtbl.o

$(WORK_SHARE)/pcgtbl.has : $(WORK_SHARE)/mkpcgtbl.x
	$(WORK_SHARE)/mkpcgtbl $(WORK_SHARE)

$(WORK_SHARE)/mkpcgtbl.x : mkpcgtbl.c
	$(CC) -O mkpcgtbl.c -o $(WORK_SHARE)/mkpcgtbl.x



#
# CLEAN
#
clean :
	-rm -f $(WORK_SHARE)/mkpcgtbl.x
	-rm -f $(WORK_SHARE)/pcgtbl.has
	-rm -f $(WORK_SHARE)/pcgtbl.o
	-rm -f $(WORK_SHARE)/pcg.o
	-rm -f $(WORK_000)/scrupdate.o
	-rm -f $(WORK_000)/io.tmp
	-rm -f $(WORK_000)/io.lab
	-rm -f $(WORK_000)/io.o
