#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_shutdown(void)
{
	outw(0xB004, 0x0|0x2000);
	outw(0x604, 0x0|0x2000);
	return 0;
}

int sys_shutdown2(char * msg) {
	if(argstr(0, &msg) < 0){
    		return -1;
  	}

	cprintf("%s\n", msg);
	outw(0xB004, 0x0 | 0x2000);
	outw(0x604, 0x0 | 0x2000);
}

int sys_uptime2(int i) {
	uint xticks;

  	acquire(&tickslock);
  	xticks = ticks;
  	release(&tickslock);
  	
  	if(argint(0, &i) < 0) {
  		return -1;
  	}
  	//cprintf("%d, %d\n", i, xticks);
  	
  	if (i == 1) return xticks;
  	else if (i == 2) return xticks / 100;
  	else if (i == 3) return xticks / (100 * 60);
  	return -1;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_exit2(void)
{
  int status;

  if(argint(0, &status) < 0)
    return -1;
  cprintf("Process %d exiting with status %d\n", myproc()->pid, status);
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

//Custom syscall to perform factorial by Samruddhi Deshpande
int
sys_fact(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;

  int result = 1;
  for (int i = 1; i <= n; i++) {
      result *= i;
  }
  return result;
}
