#
#      Makefile (main) - this file is part of SDL_Widgets
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

export CFLAGS = -Wall -march=native -mtune=native -pipe -g -ggdb -O0 \
 $(shell sdl-config --cflags) \
 $(shell pkg-config --cflags portaudio-2.0)

export LDFLAGS = \
 $(shell sdl-config --libs) \
 $(shell pkg-config --libs portaudio-2.0) \
 -lSDL_image -lSDL_gfx -lSDL_ttf -lm

export ODIR = $(shell realpath obj)
export BINPATH = $(shell realpath .)/sdlw_stable

_main:
	cd srcAudio;      make all
	cd srcWidgets;    make all
	cd srcCommon;     make all
	cd srcGraphics;   make all
	#cd srcGenetic;    make all
	#cd srcFastEvents; make all
	gcc -c $(CFLAGS) Main.c    -o obj/Main.o
	cd obj;           make all
	
_clean:
	if [ -w obj/simple.o ] ; then   rm obj/simple.o;   fi

install:
	cp $(BINPATH) /usr/bin
	
all:
	make _clean
	make _main
