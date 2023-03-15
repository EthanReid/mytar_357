CC = gcc

CFLAGS = -Wall -g
  
LD = gcc

all:
	$(CC) $(CFLAGS) mytar.c archive.c blockBuffer.c -o mytar