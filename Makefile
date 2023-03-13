CC = gcc

CFLAGS = -Wall -g
  
LD = gcc

all:
	$(CC) $(CFLAGS) listing.c mytar.c archive.c blockBuffer.c -o listing