CFLAGS?=-std=c99 -D_GNU_SOURCE -Wall -O2
CC=gcc

default: all

all: bin montecarlo

bin:
	mkdir -p bin/
	
montecarlo: montecarlo.c
	$(CC) $(CFLAGS) -o bin/montecarlo $<

clean:
	rm -rf bin/
