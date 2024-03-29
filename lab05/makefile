#define C compiler
CC = gcc
#define flags
CFLAGS = -std=c11 -Wall
BUILD_DIR = "./BUILD"

.PHONY: all zad1 clean 

all:
	make zad1 

zad1: zad1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)
