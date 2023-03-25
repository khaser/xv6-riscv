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


int sys_pgaccess(void) {
  printf("TODO sys_pgaccess\n");
  return 0;
}
