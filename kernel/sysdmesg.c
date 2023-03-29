#include "types.h"
#include "param.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

#include <stdarg.h>

char dbuff[PGSIZE * DMESG_PAGES];
static char* dbuff_it = dbuff;
static char* dbuff_end = dbuff + PGSIZE * DMESG_PAGES;

char was_overflow = 0;

static void put_char(char c) {
  *dbuff_it++ = c;
  if (dbuff_it == dbuff_end) {
    dbuff_it = dbuff;
    was_overflow = 1;
  }
  if (dbuff_it > dbuff_end) panic("dbuff overflow");
}

static void put_str(const char* str) {
  while (*str)
    put_char(*str++);
}

const char digits[] = "0123456789abcdef";

static void put_ptr(uint64 x) {
  int i;
  put_str("0x");
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    put_char(digits[x >> (sizeof(uint64) * 8 - 4)]);
}

static void put_int(int x, int base) {
  if (base > 16 || base < 2) panic("not supported put_int integer base in dmesg module");
  char lbuff[sizeof(int)];

  if (x == 0) {
    put_char('0');
    return;
  }
  if (x < 0) {
    x = -x;
    put_char('-');
  } 

  int i = 0;
  while (x != 0) {
    lbuff[i++] = digits[x % base];
    x /= base;
  } 

  while(--i >= 0)
    put_char(lbuff[i]);
}

void pr_msg(char *fmt, ...) {
  va_list ap;
  char c;

  if (fmt == 0) panic("null fmt");

  va_start(ap, fmt);
  for (int i = 0; (c = fmt[i]) != 0; i++) {
    if(c != '%') {
      put_char(c);
      continue;
    }
    c = fmt[++i];
    if(c == 0) break;
    switch(c) {
    case 'd':
      put_int(va_arg(ap, int), 10);
      break;
    case 'x':
      put_int(va_arg(ap, int), 16);
      break;
    case 'p':
      put_ptr(va_arg(ap, uint64));
      break;
    case 's':
      char* s;
      if ((s = va_arg(ap, char*)) != 0) {
        put_str(s);
      } else {
        put_str("(null)");
      }
      break;
    case '%':
      put_char('%');
      break;
    default:
      put_char('%');
      put_char(c);
      break;
    }
  }
  va_end(ap);
}

static uint64 min(uint64 a, uint64 b) { return a < b ? a : b; }

// Writes last bytes of journal to res_addr
// return number of bytes written to res(maximum: limit)
// -1 if error on writing
uint64 sys_dmesg(void) {
  uint64 res_addr, limit;
  argaddr(0, &res_addr);
  argaddr(1, &limit);
  if (res_addr == 0) return 0;

  uint64 fblock, sblock;
  if (was_overflow) {
    sblock = min(limit, dbuff_it - dbuff);
    fblock = min(limit - sblock, dbuff_end - dbuff_it);
  } else {
    fblock = 0;
    sblock = min(limit, dbuff_it - dbuff);
  }

  if (copyout(myproc()->pagetable, res_addr, dbuff_end - fblock, fblock) < 0)
    return -1;
  if (copyout(myproc()->pagetable, res_addr + fblock, dbuff_it - sblock, sblock) < 0)
    return -1;

  return fblock + sblock;
}
