#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct pak{
 int i;
 char lit;
};

int main (int lpar, char *tab[]){
  int w1;
  struct pak ob1;
// 1) utworzyc potok nazwany 'potok1'
  int fifo_fd;
  mkfifo("potok1", 0666);
  fifo_fd = open("potok1", O_RDONLY);


  while (1){
// 2) wyswietlic obiekt otrzymany z potoku
    read(fifo_fd, &ob1, sizeof(ob1));
    printf("otrzymano: %d %c\n",ob1.i,ob1.lit); fflush(stdout);
  }

  return 0;
}
