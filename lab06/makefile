#define C compiler
CC = gcc
#define flags
CFLAGS = -std=c11 -Wall
BUILD_DIR = "./BUILD"

.PHONY: all zad1 clean 

all:
	make zad1 zad2a zad2b 


zad1: zad1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

zad2a: zad2a.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

zad2b: zad2b.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)
