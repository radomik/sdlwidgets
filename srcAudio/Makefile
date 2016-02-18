#
#      Makefile (audio) - this file is part of SDL_Widgets
#
#      Copyright (C) 2013 Dariusz Mikołajczuk <radomik(at)gmail(dot)com>
#
#      This program is free software; you can redistribute it and/or modify
#      it under the terms of the GNU General Public License as published by
#      the Free Software Foundation; either version 2 of the License, or
#      (at your option) any later version.
#
#      This program is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#      GNU General Public License for more details.
#
#      You should have received a copy of the GNU General Public License along
#      with this program; if not, write to the Free Software Foundation, Inc.,
#      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

.PHONY: obj

SRC := $(shell realpath .)

audio:
	gcc -c $(CFLAGS) $(SRC)/Audio.c           -o $(ODIR)/Audio.o
	gcc -c $(CFLAGS) $(SRC)/AudioMain.c       -o $(ODIR)/AudioMain.o
	gcc -c $(CFLAGS) $(SRC)/AudioFromGraph.c  -o $(ODIR)/AudioFromGraph.o
	gcc -c $(CFLAGS) $(SRC)/AudioSinus.c      -o $(ODIR)/AudioSinus.o
	gcc -c $(CFLAGS) $(SRC)/AudioTest.c       -o $(ODIR)/AudioTest.o
	gcc -c $(CFLAGS) $(SRC)/AudioCallbacks.c  -o $(ODIR)/AudioCallbacks.o
	gcc -c $(CFLAGS) $(SRC)/WaveDrawBox.c     -o $(ODIR)/WaveDrawBox.o
	gcc -c $(CFLAGS) $(SRC)/CMPlayer.c        -o $(ODIR)/CMPlayer.o
	
clean:
	if [ -w $(ODIR)/Audio.o ] ; then           rm $(ODIR)/Audio.o; fi
	if [ -w $(ODIR)/AudioMain.o ] ; then       rm $(ODIR)/AudioMain.o; fi
	if [ -w $(ODIR)/AudioFromGraph.o ] ; then  rm $(ODIR)/AudioFromGraph.o; fi
	if [ -w $(ODIR)/AudioSinus.o ] ; then      rm $(ODIR)/AudioSinus.o; fi
	if [ -w $(ODIR)/AudioTest.o ] ; then       rm $(ODIR)/AudioTest.o; fi
	if [ -w $(ODIR)/AudioCallbacks.o ] ; then  rm $(ODIR)/AudioCallbacks.o; fi
	if [ -w $(ODIR)/WaveDrawBox.o ] ; then     rm $(ODIR)/WaveDrawBox.o; fi
	if [ -w $(ODIR)/CMPlayer.o ] ; then        rm $(ODIR)/CMPlayer.o; fi
	
all:
	make clean
	make audio
