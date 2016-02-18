#
#      Makefile (common) - this file is part of SDL_Widgets
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

common:
	gcc -c $(CFLAGS) $(SRC)/Object.c                 -o $(ODIR)/Object.o
	gcc -c $(CFLAGS) $(SRC)/Memory.c                 -o $(ODIR)/Memory.o
	gcc -c $(CFLAGS) $(SRC)/FileBrowsePanel.c        -o $(ODIR)/FileBrowsePanel.o
	gcc -c $(CFLAGS) $(SRC)/Static.c                 -o $(ODIR)/Static.o
	gcc -c $(CFLAGS) $(SRC)/file_browse_common.c     -o $(ODIR)/file_browse_common.o
	gcc -c $(CFLAGS) $(SRC)/Screen.c                 -o $(ODIR)/Screen.o
	gcc -c $(CFLAGS) $(SRC)/Callback.c               -o $(ODIR)/Callback.o
	gcc -c $(CFLAGS) $(SRC)/List2D.c                 -o $(ODIR)/List2D.o
	gcc -c $(CFLAGS) $(SRC)/Undo.c                   -o $(ODIR)/Undo.o
	gcc -c $(CFLAGS) $(SRC)/perr.c                   -o $(ODIR)/perr.o
	
clean:
	if [ -w $(ODIR)/Object.o ] ; then                rm $(ODIR)/Object.o; fi
	if [ -w $(ODIR)/Memory.o ] ; then                rm $(ODIR)/Memory.o; fi
	if [ -w $(ODIR)/FileBrowsePanel.o ] ; then       rm $(ODIR)/FileBrowsePanel.o; fi
	if [ -w $(ODIR)/Static.o ] ; then                rm $(ODIR)/Static.o; fi
	if [ -w $(ODIR)/file_browse_common.o ] ; then    rm $(ODIR)/file_browse_common.o; fi
	if [ -w $(ODIR)/Screen.o ] ; then                rm $(ODIR)/Screen.o; fi
	if [ -w $(ODIR)/Callback.o ] ; then              rm $(ODIR)/Callback.o; fi
	if [ -w $(ODIR)/List2D.o ] ; then                rm $(ODIR)/List2D.o; fi
	if [ -w $(ODIR)/Undo.o ] ; then                  rm $(ODIR)/Undo.o; fi
	if [ -w $(ODIR)/perr.o ] ; then                  rm $(ODIR)/perr.o; fi

all:
	make clean
	make common
