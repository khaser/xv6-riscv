#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, const char* argv[]) {
  const int buffsize = 100;
  char* buff = malloc(buffsize);

  printf("dmesg return code %p\n", dmesg(buff, buffsize));
  buff[buffsize - 1] = 0; // buff returns non-zero terminated
  printf("dmest returned buffer:\n===\n%s\n===\n", buff);

  return 0;
}
