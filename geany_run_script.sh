#!/bin/sh

root_path=/home/darek/Dropbox/e/KCK/SDL_Widgets
version_name=SDL_Widgets_array_stable

# -i  -- new geany instance
# -s  -- do not load previous session

path="${root_path}/${version_name}"
geany -i -s `find $root \( -name "*.c" -o -name "*.sh" -o -name "*.h" -o -name "Makefile" -o -name "*.txt" \) | tr -s '\n' ' '`
