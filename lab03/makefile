
#define C compiler
CC = gcc
#define flags
CFLAGS = -std=c11 -Wall

.PHONY: all main clean 


all:
	make main 

main: main.c 
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm *.so *.a *.o

