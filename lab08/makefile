
#define C compiler
CC = gcc
#define flags
CFLAGS = -std=c11 -Wall
BUILD_DIR = "./BUILD"

all:
	make system users 


system: system.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

users: users.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)
