# suckless-harness root Makefile

.PHONY: all test clean

all: test

test:
	ASAN_OPTIONS=detect_leaks=0 sh test/run.sh

clean:
	rm -rf test/build
