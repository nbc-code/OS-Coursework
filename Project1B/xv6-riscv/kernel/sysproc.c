#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_pcount(void)
{
  int pcount = 0;

  for(int i = 0; i < NPROC; i++)
  {
    if(proc[i].state != UNUSED)
      pcount +=  1;
  }

  return pcount;
}

uint64
sys_nice(void)
{
  int n;
  if(argint(0, &n) < 0) // get input value
  {
    return -1;
  }

  for(int i = 0; i < NPROC; i++) // search for current process
  {
    if(proc[i].pid == myproc()->pid)
    {
      proc[i].nicevalue = n; // set nice value of process
      break;
    }
  }

  printf("Proc nice value set to %d\n", n); // print nice value for testing purposes

  return 0;
}


uint64
sys_getpstat(void)
{
  uint64 result = 0;
  struct proc *p = myproc();
  uint64 upstat; // the virtual (user) address of the passed argument struct pstat
  struct pstat kpstat; // a struct pstat in kernel memory

  // get the system call argument passed by the user program
  if(argaddr(0, &upstat) < 0)
    return -1;

  // TODO: fill the arrays in kpstat (see the definition of struct pstat above).
  // The data to fill in the arrays comes from the process table array proc[].
  for(int i = 0; i < NPROC; i++) // look through list of proc's
  {
    if(proc[i].state != UNUSED) // check state
    {
      kpstat.inuse[i] = 1; // update kpstat
    }
    else
    {
      kpstat.inuse[i] = 0; // update kpstat
    }

    kpstat.pid[i] = proc[i].pid; // set pid
    kpstat.nice[i] = proc[i].nicevalue; // set nice value

    // print values for testing purposes
    printf("Proc: (index) %d, (in use) %d, (pid) %d, (nice value) %d\n", i, kpstat.inuse[i], kpstat.pid[i], kpstat.nice[i]);
  }

  // copy pstat from kernel memory to user memory
  if(copyout(p->pagetable, upstat, (char *) &kpstat, sizeof(kpstat)) < 0)
    return -1;

  return result;
}
