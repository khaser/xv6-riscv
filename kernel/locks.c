#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"



uint64
sys_init_lock(void)
{

  struct proc *p = myproc();
  printf("Process info:\nPID:\t %d\nCWD:\t %s\n\n", p->pid, p->cwd);
  return 0;
}

uint64
sys_acquire_lock(void)
{
  return 0;
}

uint64
sys_release_lock(void)
{
  return 0;
}

uint64
sys_delete_lock(void)
{

}

