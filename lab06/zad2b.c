#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define input_fifo "input"
#define output_fifo "output" 

double f(double x) {
    return 4 / (1 + x * x);
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

int main() {
    int fifo_fd;
    double info[3];

    fifo_fd = open(input_fifo, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    read(fifo_fd, info, sizeof(info));
    
    double result = calculate_integral(f, info[2], info[0], info[1]);
    
    close(fifo_fd);
    unlink(input_fifo);

    mkfifo(output_fifo, 0666);
    fifo_fd = open(output_fifo, O_WRONLY);
    
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    write(fifo_fd, &result, sizeof(result));
    
    close(fifo_fd);
    
    return 0;
}
