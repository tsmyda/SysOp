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
  
  int len = lseek(we, -1, SEEK_END);
  // read(we,&c,1);  
  // printf("%c", c);
  // printf("%d", len); 
  for (int i=len; i>=0; i--) {
    read(we,&c,1);
    // printf("%d", i);
    write(wy,&c,1);
    lseek(we, -2, SEEK_CUR);
  }
  // lseek(we, 0, SEEK_SET);
  // read(we, &c, 1);
  // write(wy, &c, 1);
  close(we);
  close(wy);

  return 0;
} 
