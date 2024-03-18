#define C compiler
CC = gcc
#define flags
CFLAGS = -std=c11 -Wall
BUILD_DIR = "./BUILD"

.PHONY: all main clean 


all:
	make reverse reverse2 reader

reverse: reverse.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

reverse2: reverse2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

reader: reader.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
clean:
	rm -rf $(BUILD_DIR)
