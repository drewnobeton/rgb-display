CC=gcc

all: rgb-display

rgb-display: rgb-display.c
	$(CC) -o rgb-display rgb-display.c

clean:
	rm rgb-display