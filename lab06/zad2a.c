#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define input_fifo "input"
#define output_fifo "output"

int main() {
    int fifo_fd;
    double info[3];
    mkfifo(input_fifo, 0666);

    fifo_fd = open(input_fifo, O_WRONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Podaj początek przedziału: ");
    scanf("%lf", &info[0]);
    printf("Podaj koniec przedziału: ");
    scanf("%lf", &info[1]);
    printf("Podaj przyrost: ");
    scanf("%lf", &info[2]);

    write(fifo_fd, info, sizeof(info));

    close(fifo_fd);
    
    printf("Dane zostały wysłane przez potok.\n");
    mkfifo(output_fifo, 0666);
    fifo_fd = open(output_fifo, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    double result;
    read(fifo_fd, &result, sizeof(result));
    close(fifo_fd); 
    unlink(output_fifo);
    printf("Otrzymana wartość wynosi: %lf\n", result);
    return 0;
}
