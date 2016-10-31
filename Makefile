CC=cc
CFLAGS=-Wall -Wextra

all: dedict strip

dedict: dedict.c
	$(CC) $(CFLAGS) -o dedict dedict.c -lz

strip: strip.c
	$(CC) $(CFLAGS) -o strip strip.c

