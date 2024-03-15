#define C compiler
CC = gcc
#define flags
CFLAGS = -std=c11 -Wall 
#define build directory
BUILD_DIR = build


.PHONY: all countdown clean

all:
	make countdown

countdown: countdown.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)

