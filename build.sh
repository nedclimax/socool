#!/bin/sh

CC=clang
CFLAGS=-Wall -Werror -Wextra -pedantic
LFLAGS=-lX11 -lXi -lXcursor -lGL -ldl -lpthread -lm

if [ ! -d "bin" ]; then
	mkdir bin
fi

cd bin
$CC $CFLAGS -I../include/ -o sktest ../src/*.c $LFLAGS
cd ..