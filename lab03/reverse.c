#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Invalid number of arguments\nUsage example: ./main <file1> <file2>\n");
    return -1;
  }
  char c;
  int we, wy;
  
  we=open(argv[1], O_RDONLY);
  wy=open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  
  int bytes = lseek(we, 0, SEEK_END);
  while (bytes>0) {
    lseek(we, -1, SEEK_CUR);
    read(we, &c, 1);
    write(wy, &c, 1);
    lseek(we, -1, SEEK_CUR);
    bytes -= 1;
  }
  close(we);
  close(wy);

  return 0;
} 
