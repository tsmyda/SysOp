#define _XOPEN_SOURCE 700 

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>


int status = -1;
int status_changes = 0;

void argument_handler(int argument){
  status_changes++;
  status = argument;
}

void handle_function(int signo, siginfo_t *info, void *extra){
  int int_val = info->si_int;
  printf("Otrzymano status: %d od pid: %d\n", int_val, info->si_pid);

  argument_handler(int_val);

  kill(info->si_pid, SIGUSR1);
}

int main() {
  printf("Catcher PID: %d\n", getpid());

  struct sigaction action;
  action.sa_sigaction = handle_function;
  action.sa_flags = SA_SIGINFO;

  sigaction(SIGUSR1, &action, NULL);

  while(1) {
    switch(status){
      case 1:
        for(int i = 1; i <= 100; i++){
          printf("%i, ", i);
        }
        printf("\n");
        status = -1;
        break;
      case 2:
        printf("Status zmienil sie %d razy\n", status_changes);
        status = -1;
        break;
      case 3:
        printf("Otrzymano sygnal zakonczenia.\n");
        exit(0);
        break;
      default:
        break;
    }
  } 

  return 0;
}
