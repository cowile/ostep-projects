#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "defs.h"
#include "x86.h"

// mset
// Starting at addr, set len pages with the given permissions.
int mset(void *addr, int len, int set, int flag)
{
  struct proc *cuproc = myproc();
  pte_t *pgdir = curproc()->pgdir;
  pte_t *pgtab;

  for(int i = 0; i < len; i++)
  {
    if((pgtab = walkpgdir(pgdir, addr + (i * PGSIZE), 0)) == 0)
      return -1;
    if(set)
      *pgtab = PTE_ADDR(*pgtab) | flag;
    else
      *pgtab = PTE_ADDR(*pgtab) & ~flag;
  }
  return 0;
}

// mprotect
// Set pages read-only.

int mprotect(void *addr, int len)
{
  return mset(addr, len, 1, PTE_W);
}

// munprotect
// Reverse mprotect.
int munprotect(void *addr, int len)
{
  return mset(addr, len, 0, PTE_W);
}

int sys_mprotect(void)
{
  char *addr;
  int len;

  // Must hae positive len.
  if(argint(1, &len) < 0 || len <= 0)
    return -1;
  // len * PGSIZE is the size of memory we protect. Must be page aligned.
  if(argptr(0, &addr, len * PGSIZE) < 0 || (uint)addr % PGSIZE != 0)
    return -1;
  if(mprotect(addr, len))
    return -1;
  return 0;
}

int sys_munprotect(void)
{
  char *addr;
  int len;

  if(argint(1, &len) < 0 || len <= 0)
    return -1;
  if(argptr(0, &addr, len * PGSIZE) < 0 || (uint)addr % PGSIZE != 0)
    return -1;
  if(mprotect(addr, len))
    return -1;
  return 0;
}

