CC ?= gcc
CFLAGS ?= -O2

all: census census_all basin

census: census.c
	$(CC) $(CFLAGS) -o $@ $< -lm

census_all: census_all.c
	$(CC) $(CFLAGS) -o $@ $< -lm

basin: basin.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	bash tests/run_tests.sh

clean:
	rm -f census census_all basin

.PHONY: all test clean
