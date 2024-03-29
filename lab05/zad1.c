#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void obsluga_v1(int signum, siginfo_t * si, void * p3) {
  printf("Obsluga sygnalu v1:%d %d, war:%d\n", si -> si_pid, si->si_uid, si->si_value);
}

int main(void) {
  // signal(SIGUSR1, SIG_IGN);
  // raise(SIGUSR1);
  // while(1);
  // return 0;
  sigset_t set;
  struct sigaction act;
  printf("pid:%d\n", getpid());

  act.sa_sigaction=obsluga_v1;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 4;
  sigaction(SIGUSR1, &act, NULL);
  return 0;

}
