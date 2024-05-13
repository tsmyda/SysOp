#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
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

int main(int argc, char** argv) {
    long number_of_printers = strtol(argv[1], NULL, 10);
    
    //CREATE AND OPEN
    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0)
        perror("shm_open");

    //SHARED MEMORY SIZE
    if(ftruncate(memory_fd, sizeof(memory_map_t)) < 0)
        perror("ftruncate");

    //map shared memory region to address space
    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED)
        perror("mmap");
    
    // CLEAN
    memset(memory_map, 0, sizeof(memory_map_t));

    memory_map->number_of_printers = number_of_printers;

    for (int i = 0; i < number_of_printers; i++){
        sem_init(&memory_map->printers[i].printer_semaphore, 1, 1);

        pid_t printer_pid = fork();
        if(printer_pid < 0) {
            perror("fork");
            return -1;
        }
        else if(printer_pid == 0) {
            while(1) {
                if (memory_map->printers[i].printer_state == 1) {

                    for (int j = 0; j < memory_map->printers[i].printer_buffer_size; j++) {
                        printf("%c", memory_map->printers[i].printer_buffer[j]);
                        // fflush(stdout);
                        sleep(1);
                    }
                    // fflush(stdout);
                    // printf("\nWydrukowano.\n");
                    fflush(stdout);
                    
                    memory_map->printers[i].printer_state = 0;

                    sem_post(&memory_map->printers[i].printer_semaphore);
                }
            }
            exit(0);
        }
    }

    while(wait(NULL) > 0) {}

    for (int i = 0; i < number_of_printers; i++)
        sem_destroy(&memory_map->printers[i].printer_semaphore);

    munmap(memory_map, sizeof(memory_map_t));
    shm_unlink(SHARED_MEMORY_DESCRIPTOR_NAME);
}
