#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"

struct userlock {
  uint active;
  struct sleeplock lock;
};

struct userlock locks[NLOCK];

struct spinlock lk;

void 
lockinit(void)
{
  initlock(&lk, "lock for accessing to locks table");
  for (int i = 0; i < NLOCK; ++i) {
    locks[i].active = 0;
    initsleeplock(&locks[i].lock, "System lock");
  }
}

uint64
sys_init_lock(void)
{
  acquire(&lk);
  for (int i = 0; i < NLOCK; ++i) {
    if (!locks[i].active) {
      locks[i].active = 1;
      release(&lk);
      return i;
    }
  }
  release(&lk);
  return -1;
}

uint64
sys_acquire_lock(void)
{
  int lockid;
  argint(0, &lockid);
  if (lockid < 0 && lockid >= NLOCK) return -1;

  struct userlock* lock = locks + lockid;
  if (lock->active == 0) return -2;

  acquiresleep(&lock->lock);
  return 0;
}

uint64
sys_release_lock(void)
{
  int lockid;
  argint(0, &lockid);
  if (lockid < 0 && lockid >= NLOCK) return -1;

  struct userlock* lock = locks + lockid;
  if (lock->active == 0) return -2;

  releasesleep(&lock->lock);
  return 0;
}

uint64
sys_delete_lock(void)
{
  int lockid;
  argint(0, &lockid);
  if (lockid < 0 && lockid >= NLOCK) return -1;

  struct userlock* lock = locks + lockid;
  if (lock->active == 0) return -2;
  if (lock->lock.locked == 1) return -3;

  acquire(&lk);
  acquire(&lock->lock.lk);
  lock->active = 0;
  release(&lock->lock.lk);
  release(&lk);
  return 0;
}
