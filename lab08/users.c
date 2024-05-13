#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <semaphore.h>

#define SHARED_MEMORY_DESCRIPTOR_NAME "shared_memory"
#define MAX_PRINTERS 100 
#define MAX_PRINTER_BUFFER_SIZE 100

typedef struct {
    sem_t printer_semaphore;
    char printer_buffer[MAX_PRINTER_BUFFER_SIZE];
    size_t printer_buffer_size;
    int printer_state;
} printer_t;

typedef struct {
    printer_t printers[MAX_PRINTERS];
    int number_of_printers;
} memory_map_t;

void generate_random_string(char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

int main(int argc, char** argv) {
    long number_of_users = strtol(argv[1], NULL, 10);

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED)
        perror("mmap");


    for (int i = 0; i < number_of_users; i++){
        pid_t user_pid = fork(); 
        if (user_pid < 0) { 
            perror("fork");
            return -1;
        }
        else if(user_pid == 0) {
            while(1) {
                char user_buffer[MAX_PRINTER_BUFFER_SIZE] = {0};
                generate_random_string(user_buffer, 10);

                int printer_index = -1;
                for (int j = 0; j < memory_map->number_of_printers; j++) {
                    int val;
                    sem_getvalue(&memory_map->printers[j].printer_semaphore, &val);
                    if(val > 0) {
                        printer_index = j;
                        break;
                    }
                }

                if(printer_index == -1)
                    printer_index = rand() % memory_map->number_of_printers;

                if(sem_wait(&memory_map->printers[printer_index].printer_semaphore) < 0)
                    perror("sem_wait");

                memcpy(memory_map->printers[printer_index].printer_buffer, user_buffer, MAX_PRINTER_BUFFER_SIZE);
                memory_map->printers[printer_index].printer_buffer_size = strlen(user_buffer);

                memory_map->printers[printer_index].printer_state = 1;

                printf("Klient %d drukuje na drukarce %d slowo %s\n", i, printer_index, user_buffer);
                fflush(stdout);

                sleep(rand()%3+1);
            }
            exit(0);         
        }
    }

    while(wait(NULL) > 0) {};
    munmap(memory_map, sizeof(memory_map_t));
}
