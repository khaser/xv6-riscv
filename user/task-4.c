#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, const char* argv[]) {
  dmesg();
  sleep(2);
  int child = fork();
  if (child) {
    //parent
    wait(0);
    printf("Hello from parent\n");
    dmesg();
  } else {
    //child
    printf("Hello from child\n");
  }
  return 0;
}
