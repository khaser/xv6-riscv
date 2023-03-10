#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int MAXN = 255;

int main() {
  int ends[2];
  if (pipe(ends) < 0)  {
    printf("Creating pipe error\n");
    exit(1);
  }

  int child = fork();
  if (child < 0) {
    printf("Fork error\n");
    exit(1);
  }
  if (child == 0) {
    // child
    close(0);
    dup(ends[0]);

    close(ends[1]);

    char* argv[] = {"wc", 0};
    exec("/wc", argv);
  } else {
    // parent
    close(ends[0]);
    close(1);

    char buff[MAXN];
    int len = read(0, buff, MAXN);
    close(0);

    write(ends[1], buff, len);
    close(ends[1]);
    wait(0); // for pretty formating
  }
  exit(0);
}
