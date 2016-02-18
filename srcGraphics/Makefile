#
#      Makefile (graphics) - this file is part of SDL_Widgets
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

graphics:
	cd srcGTools; make all
	gcc -c $(CFLAGS) $(SRC)/Histogram.c         -o   $(ODIR)/Histogram.o
	gcc -c $(CFLAGS) $(SRC)/GraphicsMain.c      -o   $(ODIR)/GraphicsMain.o
	gcc -c $(CFLAGS) $(SRC)/PixelDrawBox.c      -o   $(ODIR)/PixelDrawBox.o
	gcc -c $(CFLAGS) $(SRC)/GraphicsCallbacks.c -o   $(ODIR)/GraphicsCallbacks.o
	gcc -c $(CFLAGS) $(SRC)/HistStretchGraph.c  -o   $(ODIR)/HistStretchGraph.o

clean:
	if [ -w $(ODIR)/Histogram.o ]         ; then  rm $(ODIR)/Histogram.o;         fi
	if [ -w $(ODIR)/GraphicsMain.o ]      ; then  rm $(ODIR)/GraphicsMain.o;      fi
	if [ -w $(ODIR)/PixelDrawBox.o ]      ; then  rm $(ODIR)/PixelDrawBox.o;      fi
	if [ -w $(ODIR)/GraphicsCallbacks.o ] ; then  rm $(ODIR)/GraphicsCallbacks.o; fi
	if [ -w $(ODIR)/HistStretchGraph.o ]  ; then  rm $(ODIR)/HistStretchGraph.o; fi

all:
	make clean
	make graphics
