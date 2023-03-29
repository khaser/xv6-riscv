#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, const char* argv[]) {
  const int len = 16;
  uint64* buff = (uint64*)malloc(len * sizeof(uint64));
  vmprint();
  int entries = pgaccess((uint64)buff, len);
  printf("pgaccess entries: %d\n", entries);
  for (int i = 0; i < entries; ++i) {
    printf("%p ", buff[i]);
  }
  printf("\n");
  vmprint();
  return 0;
}
