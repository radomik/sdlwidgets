#
#      Makefile (widgets) - this file is part of SDL_Widgets
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

widgets:
	cd srcContainer; make all
	gcc -c $(CFLAGS) $(SRC)/Button.c         -o $(ODIR)/Button.o
	gcc -c $(CFLAGS) $(SRC)/ButtonImage.c    -o $(ODIR)/ButtonImage.o
	gcc -c $(CFLAGS) $(SRC)/Image.c          -o $(ODIR)/Image.o
	gcc -c $(CFLAGS) $(SRC)/Label.c          -o $(ODIR)/Label.o
	gcc -c $(CFLAGS) $(SRC)/LabelImage.c     -o $(ODIR)/LabelImage.o
	gcc -c $(CFLAGS) $(SRC)/Widget.c         -o $(ODIR)/Widget.o
	gcc -c $(CFLAGS) $(SRC)/Rectangle.c      -o $(ODIR)/Rectangle.o
	gcc -c $(CFLAGS) $(SRC)/TextBlock.c      -o $(ODIR)/TextBlock.o

clean:
	if [ -w $(ODIR)/Button.o ] ; then        rm $(ODIR)/Button.o; fi
	if [ -w $(ODIR)/ButtonImage.o ] ; then   rm $(ODIR)/ButtonImage.o; fi
	if [ -w $(ODIR)/Image.o ] ; then         rm $(ODIR)/Image.o; fi
	if [ -w $(ODIR)/Label.o ] ; then         rm $(ODIR)/Label.o; fi
	if [ -w $(ODIR)/LabelImage.o ] ; then    rm $(ODIR)/LabelImage.o; fi
	if [ -w $(ODIR)/Widget.o ] ; then        rm $(ODIR)/Widget.o; fi
	if [ -w $(ODIR)/Rectangle.o ] ; then     rm $(ODIR)/Rectangle.o; fi
	if [ -w $(ODIR)/TextBlock.o ] ; then     rm $(ODIR)/TextBlock.o; fi

all:
	make clean
	make widgets
