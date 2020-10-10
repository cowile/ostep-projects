#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "defs.h"
#include "x86.h"

// mprotect
// Starting at addr, mark len pages read only.
int mprotect(void *addr, int len);
{
  return 0;
}

// munprotect
// Reverse mprotect.
int munprotect(void *addr, int len);
{
  return 0;
}

int sys_mprotect(void);
{
  void *addr;
  int len;

  if(argint(1, &len) < 0 || len <= 0)
    return -1;
  if(addr % PGSIZE != 0 || argptr(0, &addr, len * PGSIZE))
    return -1;
  if(mprotect(addr, len))
    return -1;
  return 0;
}

int sys_munprotect(void);
{
  void *addr;
  int len;
  void *addr;
  int len;

  if(argint(1, &len) < 0 || len <= 0)
    return -1;
  if(addr % PGSIZE != 0 || argptr(0, &addr, len * PGSIZE))
    return -1;
  if(mprotect(addr, len))
    return -1;
  return 0;
}

