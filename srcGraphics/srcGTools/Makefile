#
#      Makefile (raster graphic tools) - this file is part of SDL_Widgets
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

tools:
	gcc -c $(CFLAGS) $(SRC)/GToolsPoint.c            -o $(ODIR)/GToolsPoint.o
	gcc -c $(CFLAGS) $(SRC)/GToolsSplotFilter.c      -o $(ODIR)/GToolsSplotFilter.o
	gcc -c $(CFLAGS) $(SRC)/GToolsMorph.c            -o $(ODIR)/GToolsMorph.o
	gcc -c $(CFLAGS) $(SRC)/GToolsFilters.c          -o $(ODIR)/GToolsFilters.o
	
clean:
	if [ -w $(ODIR)/GToolsPoint.o ]       ; then  rm $(ODIR)/GToolsPoint.o; fi
	if [ -w $(ODIR)/GToolsSplotFilter.o ] ; then  rm $(ODIR)/GToolsSplotFilter.o; fi
	if [ -w $(ODIR)/GToolsMorph.o ]       ; then  rm $(ODIR)/GToolsMorph.o; fi
	if [ -w $(ODIR)/GToolsFilters.o ]     ; then  rm $(ODIR)/GToolsFilters.o; fi

all:
	make clean
	make tools
