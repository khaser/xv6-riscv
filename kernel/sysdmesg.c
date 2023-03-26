#include "types.h"
#include "param.h"
#include "riscv.h"
#include "defs.h"

char dbuff[PGSIZE * DMESG_PAGES];
static char* dbuff_it = dbuff;
static char* dbuff_end = dbuff + PGSIZE * DMESG_PAGES;

static void put_to_buff(const char* str, char add_zero) {
  while (dbuff_it < dbuff_end && *str) {
    *dbuff_it++ = *str++;
  } 
  if (add_zero && dbuff_it < dbuff_end) *dbuff_it++ = 0;
}

// [beg, end)
static void strrev(char* beg, char* end) {
  while (beg < end) {
    char t = *beg;
    *beg++ = *--end;
    *end = t;
  }
}

void pr_msg(const char* str) {
  int i = 0;
  uint x = ticks;
  static char digits[] = "0123456789";
  while (dbuff_it + i < dbuff_end && x != 0) {
    dbuff_it[i++] = digits[x % 10];
    x /= 10;
  } 
  strrev(dbuff_it, dbuff_it + i);
  dbuff_it += i;

  put_to_buff(": ", 0);

  put_to_buff(str, 1);
}

uint64 sys_dmesg(void) {
  printf("DMESG:\n");
  for (char* it = dbuff; it < dbuff_it; ++it) {
    consputc(*it ? *it : '\n');
  }
  printf("=====\n");
  return 0;
}
