#!/bin/sh

set -xe

CC="gcc"
CFLAGS="-O2 -Wall -Werror -pedantic -std=c99"
LIBS=""

$CC $CFLAGS $LIBS $1 -o output
