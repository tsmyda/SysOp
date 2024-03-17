#include <fcntl.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> 

void swap(char c[], int i, int size) {
    char temp = c[i];
    c[i] = c[size - i - 1];
    c[size- i - 1] = temp;
}

void reverse_block(char c[], int size) {
    for (size_t j = 0; j < size / 2; ++j) {
        swap(c, j, size);
    }
}
int main(int argc, char *argv[]) {
    int BUFFER_SIZE = 1024;
    int wy = open(argv[1], O_RDONLY);
    int we = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR );
    char c[BUFFER_SIZE];
    off_t file_size = lseek(wy, 0, SEEK_END);
    for (off_t i = 1; i <= file_size; i+=BUFFER_SIZE) {
        int size = i + BUFFER_SIZE < file_size ? BUFFER_SIZE : file_size - i;
        if (i + BUFFER_SIZE < file_size) {
            lseek(wy, -i, SEEK_END); 
        } else{
            lseek(wy, 0, SEEK_SET); 
        }
        read(wy, c, size);
        reverse_block(c, size);
        lseek(we, 0, SEEK_SET);
        write(we, c, size);
    }
    close(we);
    close(wy);
    return 0;
}
