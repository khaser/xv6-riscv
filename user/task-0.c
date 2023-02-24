#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    const char* str = "Hello, world\n";
    write(0, str, strlen(str));
    exit(0);
}
