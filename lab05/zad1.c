#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void handleSIGUSR1(int signum) {
    printf("Otrzymano sygnał SIGUSR1\n");
}

int main() {
    printf("Select what would you like to be done upon receiving SIGUSR1: \n");
    printf("n - nothing changes\n");
    printf("i - ignore signal\n");
    printf("h - use handler\n");
    printf("m - mask signal\n");
    printf("Select option: ");
    char c;
    scanf("%c", &c);

    sigset_t sigset;

    if (c == 'n') {
        printf("Nothing changed\n");
    } else if (c=='i') {
        signal(SIGUSR1, SIG_IGN);
    } else if (c=='h') {
        signal(SIGUSR1, handleSIGUSR1);
    } else if (c=='m') {
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGUSR1);
        sigprocmask(SIG_BLOCK, &sigset, NULL);
    } else {
        return EXIT_FAILURE;
    }

    raise(SIGUSR1);

    sigpending(&sigset);
    if (sigismember(&sigset, SIGUSR1)) {
        printf("Signal is visible\n");
    } else {
        printf("Signal is invisible\n");
    }

    return 0;
}
