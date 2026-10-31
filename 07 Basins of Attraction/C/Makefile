CC ?= gcc
CFLAGS ?= -O2
# passage needs GMP and MPFR. If they are not installed system-wide, point
# GMP_PREFIX to a prefix that has them, e.g. a conda env with gmp and mpfr:
#   make passage GMP_PREFIX=/home/esul01/envs/gmp
GMP_PREFIX ?=
ifneq ($(GMP_PREFIX),)
GMP_FLAGS = -I$(GMP_PREFIX)/include -L$(GMP_PREFIX)/lib -Wl,-rpath,$(GMP_PREFIX)/lib
endif

all: census census_all basin

census: census.c
	$(CC) $(CFLAGS) -o $@ $< -lm

census_all: census_all.c
	$(CC) $(CFLAGS) -o $@ $< -lm

basin: basin.c
	$(CC) $(CFLAGS) -o $@ $<

passage: passage.c
	$(CC) $(CFLAGS) $(GMP_FLAGS) -o $@ $< -lmpfr -lgmp

test: all
	bash tests/run_tests.sh

test-passage: passage
	bash tests/run_passage_tests.sh

clean:
	rm -f census census_all basin passage

.PHONY: all test test-passage clean
