#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int sys_clone(void)
{
  void (*func)(void *, void *);
  void *arg_1;
  void *arg_2;
  void *stack;

  // normal pointers only need point to a single byte so size = 1
  if(argptr(0, (char**)&func, 1) < 0)
    return -1;
  // treat arguments as integers because test 1 expects it
  if(argint(1, (int*)&arg_1) < 0)
    return -1;
  if(argint(2, (int*)&arg_2) < 0)
    return -1;
  // stack should match kernel stack size
  if(argptr(3, (char**)&stack, KSTACKSIZE) < 0)
    return -1;

  return clone(func, arg_1, arg_2, stack);
}

int sys_join(void)
{
  void **stack;

  // we don't what the pointer points to because we will overwrite so size = 0
  if(argptr(0, (char**)&stack, 0) < 0)
    return -1;

  return join(stack);
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
