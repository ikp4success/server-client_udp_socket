CC = gcc
CFLAGS = -std=gnu99 -g -I.

all: client server

client: client.c minsc.h Fat12BootEntry.h flopCommands.c Fat12BootEntry.c
	$(CC) -o $@ $^ $(CFLAGS)

server: server.c Fat12BootEntry.h Fat12BootEntry.c minsc.h
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o *~ client server
