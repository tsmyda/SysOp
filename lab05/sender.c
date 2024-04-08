#define _XOPEN_SOURCE 700

#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handler_function(int signo) {
    printf("Otrzymano potwierdzenie\n");
}
int main(int argc, char** argv) {
  printf("Sender PID: %d\n", getpid());

  signal(SIGUSR1, handler_function);

  long signal_pid = strtol(argv[1], NULL, 10);
  long signal_argument = strtol(argv[2], NULL, 10);

  union sigval value = {signal_argument};

  sigqueue(signal_pid, SIGUSR1, value);
  printf("Signal sent with argument: %ld\n", signal_argument);

  return 0;
}
