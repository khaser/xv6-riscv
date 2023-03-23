#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test() {
  int mutex = init_lock();
  printf("mutex: %d\n", mutex);
  int child = fork();
  int pid = getpid();
  if (child < 0) {
    printf("Fork error\n");
    exit(1);
  }

  if (child != 0) {
    // parent
    acquire_lock(mutex);
    for (int i = 0; i < 10; ++i) {
      printf("PARENT: %d\n", pid);
    }
    release_lock(mutex);
    wait(0);
    delete_lock(mutex);
    printf("new mutex: %d\n", init_lock());
  } else {
    //child
    acquire_lock(mutex);
    for (int i = 0; i < 10; ++i) {
      printf("CHILD: %d\n", pid);
    }
    release_lock(mutex);
  }

  exit(0);
}

const int MAXN = 255;
const char* fmt = "%d: received %c\n";

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    printf("Expected at least 1 argument\n");
    exit(1);
  }
  const char* msg = argv[1];
  close(0);

  int parent2child[2];
  int child2parent[2];
  if (pipe(parent2child) < 0 || pipe(child2parent) < 0) {
    printf("Creating pipe error\n");
    exit(-1);
  }
  int lock;
  if ((lock=init_lock()) < 0) {
    printf("Creating lock error\n");
    exit(-1);
  }

  int child = fork();
  int pid = getpid();
  if (child < 0) {
    printf("Fork error\n");
    delete_lock(lock);
    exit(-1);
  }
  if (child == 0) {
    // child
    close(parent2child[1]);
    close(child2parent[0]);

    char c;
    while (read(parent2child[0], &c, 1) == 1) {
      acquire_lock(lock);
      printf(fmt, pid, c);
      release_lock(lock);
      write(child2parent[1], &c, 1);
    }

    close(1);
    close(child2parent[1]);
    close(parent2child[0]);
  } else {
    // parent
    close(parent2child[0]);
    close(child2parent[1]);

    for (int i = 0; msg[i]; ++i) {
      write(parent2child[1], &msg[i], 1);
    }
    close(parent2child[1]);

    char c;
    while (read(child2parent[0], &c, 1) == 1) {
      acquire_lock(lock);
      printf(fmt, pid, c);
      release_lock(lock);
    }

    close(1);
    close(child2parent[0]);
    close(parent2child[1]);
    wait(0);
    delete_lock(lock);
  }
  exit(0);
}
