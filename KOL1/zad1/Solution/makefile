all:
	make bibl1 zad1

bibl1: bibl1.c
	gcc -shared -fPIC -o bibl1.so bibl1.c

zad1: zad1.c
	gcc -o zad1 zad1.c -ldl
