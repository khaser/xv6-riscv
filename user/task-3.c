#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, const char* argv[]) {
  vmprint();
  pgaccess();
  // TODO: test pgaccess
  vmprint();
  return 0;
}
