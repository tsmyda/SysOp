#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

double f(double x) {
    return 4.0 / (1 + x * x);
}

double calculate_integral(double (*f) (double), double width, double start, double stop) {
    double sum = 0.0;
    double curr = start;
    while (curr < stop) {
        double mid = (2*curr+width)/2;
        sum += width * f(mid);
        curr += width;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Użycie: %s <interval_width> <number_of_proccesses>\n", argv[0]);
        return 1;
    }

    double rectangle_width = atof(argv[1]);
    int number_of_proccesses = atoi(argv[2]);
    double range_start = 0.0;
    double range_end = 1.0;
    double integral_result = 0.0;

    int pipefd[number_of_proccesses][2]; 

    for (int i = 0; i < number_of_proccesses; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    for (int i = 0; i < number_of_proccesses; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            close(pipefd[i][0]);
            double start = range_start + (range_end - range_start) / number_of_proccesses * i;
            double koniec = range_start + (range_end - range_start) / number_of_proccesses * (i + 1);
            double proccess_result = calculate_integral(f, rectangle_width, start, koniec);
            write(pipefd[i][1], &proccess_result, sizeof(proccess_result));
            close(pipefd[i][1]);
            return 0;
        }
    }

    for (int i = 0; i < number_of_proccesses; i++) {
        close(pipefd[i][1]);
        double proccess_result;
        read(pipefd[i][0], &proccess_result, sizeof(proccess_result));
        integral_result += proccess_result;
        close(pipefd[i][0]);
    }

    printf("Wynik całkowania: %f\n", integral_result);

    return 0;
}
