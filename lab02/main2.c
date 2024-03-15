#include <dlfcn.h>
#include <stdio.h>

int main(void) {
  void *handle = dlopen("./libcollatz.so", RTLD_LAZY);
  if (!handle){printf("Error when opening lib\n"); return 0;}

  int (*f)(int, int);
  f = dlsym(handle, "test_collatz_convergence");
  if (dlerror()!=0) {printf("Error f\n"); return 0;}

  printf("%d\n", f(10, 100));

  dlclose(handle);
  return 0;
}
