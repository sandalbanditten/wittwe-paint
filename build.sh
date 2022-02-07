#!/bin/sh

set -xe

CC="gcc"
CFLAGS="-O2 -Wall -Werror -pedantic -std=c99 $(sdl2-config --cflags)"
LIBS="$(sdl2-config --libs) -lm"

$CC $CFLAGS $LIBS $1 -o output
