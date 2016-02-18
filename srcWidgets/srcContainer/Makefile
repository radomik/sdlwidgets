#
#      Makefile (containers) - this file is part of SDL_Widgets
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

clean:
	if [ -w $(ODIR)/StackList.o ]    ; then rm $(ODIR)/StackList.o      ; fi
	if [ -w $(ODIR)/StackListX.o ]   ; then rm $(ODIR)/StackListX.o     ; fi
	if [ -w $(ODIR)/StackListY.o ]   ; then rm $(ODIR)/StackListY.o     ; fi
	if [ -w $(ODIR)/Container.o ]    ; then rm $(ODIR)/Container.o      ; fi
	if [ -w $(ODIR)/Grid.o ]         ; then rm $(ODIR)/Grid.o           ; fi
	if [ -w $(ODIR)/FloatingMenu.o ] ; then rm $(ODIR)/FloatingMenu.o   ; fi

container:
	gcc -c $(CFLAGS) $(SRC)/StackList.c      -o $(ODIR)/StackList.o
	gcc -c $(CFLAGS) $(SRC)/StackListX.c     -o $(ODIR)/StackListX.o
	gcc -c $(CFLAGS) $(SRC)/StackListY.c     -o $(ODIR)/StackListY.o
	gcc -c $(CFLAGS) $(SRC)/Container.c      -o $(ODIR)/Container.o
	gcc -c $(CFLAGS) $(SRC)/Grid.c           -o $(ODIR)/Grid.o
	gcc -c $(CFLAGS) $(SRC)/FloatingMenu.c   -o $(ODIR)/FloatingMenu.o

all:
	make clean
	make container
