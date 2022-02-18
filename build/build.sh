#!/bin/sh

set -xe

CC="gcc"
CFLAGS="-O2 -Wall -Werror -pedantic -std=c99 $(sdl2-config --cflags)"
LIBS="$(sdl2-config --libs) -lm"
BIN="wittwe"

$CC $CFLAGS $LIBS -o $BIN $@ && mv $BIN ../bin/
