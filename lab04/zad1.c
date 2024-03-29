#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  if (argc!=2) {
    printf("Wrong number of arguments\n");
    return -1;
  }
  int processes_number = atoi(argv[1]);
  if (processes_number < 0) {
    printf("Invalid argument, number of processes cannot be less than 0");
    return -1;
  }
  for (int i=0; i < processes_number; i++) {
    pid_t pid = fork();
    if (pid==0) {
      printf("Identyfikator procesu macierzystego: %d; Identyfikator procesu potomnego: %d\n", getppid(), getpid());
      // exit(0);
    }
  }

  while(wait(0) > 0);
  printf("Number of processes: %d\n", processes_number);

  return 0;
}
