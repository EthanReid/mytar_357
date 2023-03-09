CC = gcc

CFLAGS = -Wall -g

LD = gcc

all:
	$(CC) $(CFLAGS) archive.c mytar.c blockBuffer.c -o mytar