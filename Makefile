CC = gcc

CFLAGS = -Wall -g
  
LD = gcc

all:
	$(CC) $(CFLAGS) archive.c listing2.c mytar.c -o mytar