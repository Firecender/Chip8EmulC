CC = gcc
CFLAGS = -Wall -g
src_file := $(wildcard src/*.c)
build_file := $(src_file:.c=.o)
OBJDIR = obj
IncDir = -I /Inc/
libs = -lSDL3

bin/app: obj/Pixel.o obj/cpu.o obj/main.o
	echo $^
	gcc $(CFLAGS) $(libs) $(IncDir) -o $@ $^
obj/Pixel.o: src/Pixel.c 
	gcc $(CFLAGS) -I Inc/Pixel.h -c src/Pixel.c -o obj/Pixel.o
obj/cpu.o: src/cpu.c 
	gcc $(CFLAGS) -c src/cpu.c -o obj/cpu.o
obj/main.o: src/main.c 
	gcc $(CFLAGS) -c src/main.c -o obj/main.o

.PHONY: clean
clean: 
	rm -f obj/*.o
	rm -f bin/*
	
