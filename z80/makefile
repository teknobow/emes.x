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

TARGET_SHARE =	$(WORK_SHARE)/disasmtab.o \
		$(WORK_SHARE)/ftbl8.o \
		$(WORK_SHARE)/_debugger.o

TARGET_000 =	$(WORK_000)/z80emu.o \
		$(WORK_000)/z80emusize.equ \
		$(WORK_000)/flagtbl.o \
		$(WORK_000)/relextslot.o

TARGET_030 =	



ALL : $(TARGET_SHARE) $(TARGET_000) $(TARGET_030)


Z80EMU_000 = $(WORK_000)/z80emu.o $(WORK_000)/z80emusize.equ $(WORK_000)/relextslot.o
Z80EMU_030 = 

$(Z80EMU_000) : z80emu.pl z80emu.has \
		z80emu.mac z80emu.equ z80emu.inc \
		$(WORK_000)/flagtbl.o $(WORK_SHARE)/ftbl8.o \
		../io/vramwrite.inc ../io/io.equ ../io/x68k.equ \
		$(WORK_000)/special_adr.dat
	$(PERL) z80emu.pl m68000 $(WORK_000) $(WORK_SHARE)


FLAGTBL_000 =	$(WORK_000)/flagtbl.o		\
		$(WORK_000)/flagtbl.equ		\
		$(WORK_000)/special_adr.dat

FLAGTBL_030 = 

FLAGS =	$(WORK_SHARE)/inc.flg	$(WORK_SHARE)/dec.flg	$(WORK_SHARE)/daa.flg	\
	$(WORK_SHARE)/and.flg	$(WORK_SHARE)/or.flg	$(WORK_SHARE)/rlc.flg	\
	$(WORK_SHARE)/rrc.flg	$(WORK_SHARE)/rl.flg	$(WORK_SHARE)/rr.flg	\
	$(WORK_SHARE)/sla.flg	$(WORK_SHARE)/sra.flg	$(WORK_SHARE)/srl.flg	\
	$(WORK_SHARE)/adc16.flg	$(WORK_SHARE)/sbc16.flg	$(WORK_SHARE)/rrld.flg	\
	$(WORK_SHARE)/iff.flg

$(FLAGTBL_000) : makeftbl.pl z80emu.mac z80emu.equ except.inc \
		../IO/x68k.equ ../io/vdp.inc ../io/vdp.equ \
		../IO/slot.inc ../io/slot.equ ../IO/setvram.inc ../IO/clockIC.inc \
		../IO/keyboard.inc ./special.inc ./romwrite.inc ../IO/joystick.inc \
		$(FLAGS)
	$(PERL) makeftbl.pl m68000 $(WORK_000) $(WORK_SHARE)



#
# SHARE
#

$(WORK_SHARE)/_debugger.o : _debugger.c
	$(CC) -O2 -c _debugger.c -o $(WORK_SHARE)/_debugger.o

$(WORK_SHARE)/disasmtab.o : $(WORK_SHARE)/disasmtab.has
	has -m68000 $(WORK_SHARE)/disasmtab.has -o $(WORK_SHARE)/disasmtab.o

$(WORK_SHARE)/disasmtab.has : mkop.pl op.tab
	$(PERL) mkop.pl < op.tab > $(WORK_SHARE)/disasmtab.has

$(WORK_SHARE)/ftbl8.o : mkaddsub.pl $(WORK_SHARE)/add8.flg $(WORK_SHARE)/sub8.flg
	$(PERL) mkaddsub.pl $(WORK_SHARE)


$(WORK_SHARE)/inc.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag INC > $(WORK_SHARE)/inc.flg

$(WORK_SHARE)/dec.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag DEC > $(WORK_SHARE)/dec.flg

$(WORK_SHARE)/daa.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag DAA > $(WORK_SHARE)/daa.flg

$(WORK_SHARE)/add8.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag ADD > $(WORK_SHARE)/add8.flg

$(WORK_SHARE)/sub8.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag SUB > $(WORK_SHARE)/sub8.flg

$(WORK_SHARE)/and.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag AND > $(WORK_SHARE)/and.flg

$(WORK_SHARE)/or.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag OR > $(WORK_SHARE)/or.flg

$(WORK_SHARE)/rlc.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag RLC > $(WORK_SHARE)/rlc.flg

$(WORK_SHARE)/rrc.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag RRC > $(WORK_SHARE)/rrc.flg

$(WORK_SHARE)/rl.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag RL > $(WORK_SHARE)/rl.flg

$(WORK_SHARE)/rr.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag RR > $(WORK_SHARE)/rr.flg

$(WORK_SHARE)/sla.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag SLA > $(WORK_SHARE)/sla.flg

$(WORK_SHARE)/sra.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag SRA > $(WORK_SHARE)/sra.flg

$(WORK_SHARE)/srl.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag SRL > $(WORK_SHARE)/srl.flg

$(WORK_SHARE)/adc16.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag ADC16 > $(WORK_SHARE)/adc16.flg

$(WORK_SHARE)/sbc16.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag SBC16 > $(WORK_SHARE)/sbc16.flg

$(WORK_SHARE)/rrld.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag RLD > $(WORK_SHARE)/rrld.flg

$(WORK_SHARE)/iff.flg : $(WORK_SHARE)/mkflag.x
	$(WORK_SHARE)/mkflag IFF > $(WORK_SHARE)/iff.flg

$(WORK_SHARE)/mkflag.x : mkflag.c flag.h
	$(CC) -O mkflag.c -o $(WORK_SHARE)/mkflag.x



#
# CLEAN
#
clean :
	-rm -f $(WORK_SHARE)/mkflag.x
	-rm -f $(FLAGS)
	-rm -f $(WORK_SHARE)/add8.flg
	-rm -f $(WORK_SHARE)/sub8.flg
	-rm -f $(WORK_SHARE)/ftbl8.o
	-rm -f $(WORK_SHARE)/ftbl8.has
	-rm -f $(WORK_SHARE)/disasmtab.has
	-rm -f $(WORK_SHARE)/disasmtab.o
	-rm -f $(WORK_SHARE)/_debugger.o
	-rm -f $(WORK_000)/flagtbl.o
	-rm -f $(WORK_000)/drvspace.inc
	-rm -f $(WORK_000)/flagtbl.has
	-rm -f $(WORK_000)/flagtbl.lab
	-rm -f $(WORK_000)/relextslot.has
	-rm -f $(WORK_000)/vramwrite.tmp
	-rm -f $(WORK_000)/z80emu.lab
	-rm -f $(WORK_000)/z80emu.tmp
	-rm -f $(FLAGTBL_000)
	-rm -f $(Z80EMU_000)

