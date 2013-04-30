#
# MSX Emulator for X680x0 - emes.x
#
#    Copyright 1997-1998 nir
#

ALL :
	(cd z80; make ALL)
	(cd io; make ALL)
	(cd slot; make ALL)
	(cd sound; make ALL)
	(cd startup; make ALL)

clean :
	(cd z80; make clean)
	(cd io; make clean)
	(cd slot; make clean)
	(cd sound; make clean)
	(cd startup; make clean)
