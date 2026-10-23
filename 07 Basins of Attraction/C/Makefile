CC ?= gcc
CFLAGS ?= -O2

all: census basin

census: census.c
	$(CC) $(CFLAGS) -o $@ $< -lm

basin: basin.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f census basin

.PHONY: all clean
