#
#      Makefile (genetic) - this file is part of SDL_Widgets 
#      [currently unfinished and not compiled, see GeneticMain.c for details]
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

genetic:
	gcc -c $(CFLAGS) $(SRC)/GeneticMain.c      -o $(ODIR)/GeneticMain.o
	
clean:
	if [ -w $(ODIR)/GeneticMain.o ] ; then         rm $(ODIR)/GeneticMain.o; fi

all:
	make clean
	make genetic
