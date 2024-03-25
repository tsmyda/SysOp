#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int global = 0;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Invalid number of arguments\n");
    return -1;
  }
  printf("Program name: %s \n", strrchr(argv[0], '/') + 1);
  int local = 0;

  pid_t pid = fork();

  if (pid == 0) {
    printf("Child process\n");
    global++;
    local++;

    printf("Child PID = %d, Parent PID = %d \n", getpid(), getppid());
    printf("Child's local = %d, Child's global = %d \n", local, global);
    
    int status = execl("/bin/ls", "ls", argv[1], NULL);
    exit(status);
  }
  int status = 0;
  wait(&status);
  int child_status = WEXITSTATUS(status);
  printf("Parent process\n");
  printf("Parent PID = %d, Child PID = %d \n", getpid(), pid);
  printf("Child exit code: %d \n", child_status);
  printf("parent's local = %d, parent's global = %d \n", local, global);

  return child_status;
}
