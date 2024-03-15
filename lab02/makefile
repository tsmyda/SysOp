#define C compiler
CC = gcc
#define path for binary files
BUILD_DIR = build
#define flags
CFLAGS = -std=c11 -Wall

.PHONY: all main_static main_shared main_dynamic collatz_static collatz_shared clean 


all:
	make main_static main_shared main_dynamic

collatz_static: collatz.c 
	$(CC) -c $<
	ar rcs libcollatz.a collatz.o

collatz_shared: collatz.c
	$(CC) -fPIC -c $<
	$(CC) -shared collatz.o -o libcollatz.so	

main_static: main.c collatz_static
	$(CC) main.c libcollatz.a -o main_stat

main_shared: main.c collatz_shared
	$(CC) main.c -L. -lcollatz -Wl,-rpath=. -o main_shared

main_dynamic: main2.c collatz_shared 
	$(CC) main2.c -o main_dynamic -ldl

clean:
	rm *.so *.a *.o

