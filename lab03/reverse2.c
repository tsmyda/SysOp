#include <fcntl.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> 

void swap(char c[], int i, int size) {
  char temp = c[i];
  c[i] = c[size - i - 1];
  c[size - i - 1] = temp;
}

void reverse_block(char c[], int size) {
  for (int j = 0; j < size / 2; j++) {
    swap(c, j, size);
  }
}
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Invalid number of arguments\nUsage example: ./reverse2 <file1> <file2>\n");
    return -1;
  }
  int BUFFER_SIZE = 10;
  int we = open(argv[1], O_RDONLY);
  int wy = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  char c[BUFFER_SIZE];
  long bytes = lseek(we, 0, SEEK_END);
  while (bytes>0) {
    long size = bytes > BUFFER_SIZE ? BUFFER_SIZE : bytes;
    lseek(we, -size, SEEK_CUR);
    read(we, &c, size);
    reverse_block(c, size);
    write(wy, &c, size);
    lseek(we, -BUFFER_SIZE, SEEK_CUR);
    bytes -= size;
  }
  close(we);
  close(wy);
  return 0;
}
