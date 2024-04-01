#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
void handleSIGUSR1(int signum) {
    printf("Otrzymano sygnał SIGUSR1\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ignore|handler|mask>\n", argv[0]);
        return -1;
    }

    sigset_t sigset;

    if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
    } else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, handleSIGUSR1);
    } else if (strcmp(argv[1], "mask") == 0) {
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGUSR1);
        sigprocmask(SIG_BLOCK, &sigset, NULL);
    } else {
        fprintf(stderr, "Unknown param: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    raise(SIGUSR1);

    sigpending(&sigset);
    if (sigismember(&sigset, SIGUSR1)) {
        printf("widoczny");
    } else {
        printf("niewidoczny");
    }

    return 1;
}
