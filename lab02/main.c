#include <stdio.h>
extern int collatz_conjecture(int);
extern int test_collatz_convergence(int, int);

int main(void) {
  printf("%d\n", test_collatz_convergence(10, 100));
  return 0;
}
