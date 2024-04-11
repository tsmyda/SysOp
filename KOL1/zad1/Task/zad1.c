#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
  int i;
  int tab[20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
/*
1) otworz biblioteke
2) przypisz wskaznikom f1 i f2 adresy funkcji sumuj i dziel z biblioteki
3) stworz Makefile kompilujacy biblioteke 'bibl1' ladowana dynamicznie
   oraz kompilujacy ten program
*/

  for (i=0; i<3; i++) printf("Wynik: %d, %lf\n", f1(tab+i, 20-i), f2(tab[i], tab[i+1]));
  return 0;
}
