// System calls for hw-3 

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

static void print_pdtable(pagetable_t table, int deep) {
  for (int i = 0; i < PDSIZE; ++i) {
    pte_t pte = table[i];
    if (!(pte & PTE_V)) continue;

    for (int _ = 0; _ < 2 * deep; ++_) consputc('-');
    printf("%d: ph_addr %p", i, PTE2PA(pte));

    if (!(pte & (PTE_R | PTE_W | PTE_X))) {
      consputc('\n');
      print_pdtable((pagetable_t)PTE2PA(pte), deep + 1);
    } else {
      printf(" flags ");
      consputc((PTE_R & pte) ? 'R' : '-');
      consputc((PTE_W & pte) ? 'W' : '-');
      consputc((PTE_X & pte) ? 'X' : '-');
      consputc((PTE_U & pte) ? 'U' : '-');
      consputc((PTE_G & pte) ? 'G' : '-');
      consputc((PTE_A & pte) ? 'A' : '-');
      consputc((PTE_D & pte) ? 'D' : '-');
      consputc('\n');
    }
  }
}

int sys_vmprint() {
  print_pdtable(myproc()->pagetable, 0);
  return 0;
}

static int pg_access(pagetable_t table, uint64 res_addr, uint64* entries, uint64 limit) {
  for (int i = 0; i < PDSIZE; ++i) {
    if (*entries >= limit) break;

    pte_t pte = table[i];
    if (!(pte & PTE_V)) continue;

    if (!(pte & (PTE_R | PTE_W | PTE_X))) {
      pg_access((pagetable_t)PTE2PA(pte), res_addr, entries, limit);
    } else {
      if (PTE_A & pte) {
        uint64 tmp = PTE2PA(pte);
        if (copyout(myproc()->pagetable, res_addr + sizeof(uint64) * (*entries)++, (char*)&tmp, sizeof(uint64)) < 0) return -1;
        table[i] &= ~PTE_A;
      }
    }
  }
  return 0;
}

int sys_pgaccess(void) { // return number of accessed pages
  uint64 res_addr, entries = 0, limit;
  argaddr(0, &res_addr);
  argaddr(1, &limit);
  if (pg_access(myproc()->pagetable, res_addr, &entries, limit) < 0) 
    return -1;
  else
    return entries;
}
