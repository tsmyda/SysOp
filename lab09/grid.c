#include "grid.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

//
//
//

typedef struct ThreadArguments {
    char *src;
    char *dst;
    int start;
    int stop;
} ThreadArguments;

pthread_t *threads = NULL;
ThreadArguments *args = NULL;

void free_mem() {
    free(threads);
    free(args);
}


void ignore_signal(int signum) {}

void* update_cell(void *args) {
    ThreadArguments *thread_args = (ThreadArguments*) args;
    int start = thread_args->start;
    int stop = thread_args->stop;

    while (1) {
        for (int i = start; i<=stop; i++) {
             thread_args->dst[i] = is_alive(i/grid_width, i%grid_height, thread_args->src);
        }
        
        pause();
        char *temp = thread_args->src;
        thread_args->src = thread_args->dst;
        thread_args->dst = temp;
    }

    return NULL;
}

void update_grid_parallel(char *src, char *dst, int threads_no) {
    struct sigaction sa;
    sa.sa_handler = ignore_signal;
    
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);


    const int A = grid_width*grid_height;
    int size = A/threads_no;
    int r = A % threads_no;

    threads = malloc(threads_no * sizeof(pthread_t));
    args = malloc(threads_no * sizeof(ThreadArguments));
    
    for (int i=0; i<threads_no; i++) { 
        args[i].src = src;
        args[i].dst = dst;    
        args[i].start = i > 0 ? args[i-1].stop+1 : 0;
        args[i].stop = args[i].start + size - 1 + (r>i ? 1 : 0); 
        pthread_create(&threads[i], NULL, update_cell, (void *) &args[i]);
    }

    for (int i=0; i<threads_no; i++) {
        pthread_kill(threads[i], SIGUSR1);
    }
}
