#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int fd[2];
int number = 0;
int status = 0;
/*
1) program tworzy dwa procesy potomne. Nastepnie proces macierzysty co sekundê
wysy³a SIGUSR1 do procesu potomnego 1. Proces potomny 1 po otrzymaniu sygna³u
przesy³a kolejn± liczbê (rozpoczynajac od zera) przez potok nienazwany do
procesu potomnego 2, który wyswietla te liczbe.

2) Po wcisnieciu ctrl-c (SIGINT) powinno nastapic przerwanie wysy³ania sygnalow.
Powtorne wcisniecie ctrl-c powinno wznowic wysylanie sygnalow*/


void handler_SIGUSR1(int signum) {
  write(fd[1], &number, sizeof(number));
  number += 1;
}

void handler_SIGINT(int signum) {
  if (status == 0) {
    status = 1;
  } else {
    status = 0;
  }
}


int main (int lpar, char *tab[]){
  pid_t pid1, pid2;
  int d,i;
  if (pipe(fd) == -1) {
    perror("pipe");
    return 0;
  }
  
  pid1 = fork();
  if (pid1 < 0) {
    perror("fork");
    return 0;
  } else if (pid1 == 0) { // proces 1
    close(fd[0]);
    signal(SIGUSR1, handler_SIGUSR1);
    signal(SIGINT, SIG_IGN);
    while(1) {
      sleep(1);
    }
  } else {
    pid2 = fork();
    if (pid2 < 0) {
      perror("fork");
      return 0;
    } else if (pid2 == 0) { // proces 2
      close(fd[1]);
      int number;
      signal(SIGINT, SIG_IGN);
      while(1) {
        read(fd[0], &number, sizeof(number));
        printf("Proces potomny 2 przyjal %lu bajtow, wartosc: %d\n", sizeof(number), number);
        sleep(1);
      }
    } else { // proces macierzysty
      close(fd[0]); 
      close(fd[1]);
      signal(SIGINT, handler_SIGINT);
      while(1) {
        sleep(1);
        if (status == 0) {
          kill(pid1, SIGUSR1);
          printf("Wyslano SIGUSR1\n");
        } else {
          printf("Wstrzymano");
        }
      }
    }
  }
  return 0;
}
