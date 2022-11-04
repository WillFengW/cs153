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

int
sys_exit(void) // original exit
{
    exit();
    return 0;  // not reached
}

int
sys_exitNew(void) // new exit with status, use argint to get the argument just like sys_kill
{
    int status;

    if(argint(0, &status) < 0)
        return -1;
    exitNew(status);
    return 0;  // not reached
}

int
sys_wait(void) // original wait
{
    return wait();
}

int
sys_waitNew(void) // new wait with status, use argint to get the argument
{
    int *status;
    if(argptr(0, (void*)&status, sizeof(status)) < 0)
        return -1;
    return waitNew(status);
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

int
sys_waitpid(void) {
    int pidArg;
    int options = 0;
    int* status;

    // if argint, 1st argument
    if (argint(0, &pidArg) < 0) {
        return -1;
    }
    // if argptr, 2nd argument
    if (argptr(1, (void*)&status, sizeof(status)) < 0) {
        return -1;
    }
    // return waitpid
    //argint(0, &pidArg);
    //argptr(0, (void*)&status, sizeof(status));
    return waitpid(pidArg, status, options);
}

int
sys_setPrior(void) {
    int priority;
    if (argint(0, &priority) < 0) {
        return -1;
    }
    return setPrior(priority);
}