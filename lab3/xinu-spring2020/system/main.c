/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)

void check(pid32);
void p1();

process	main(void)
{

  pid32 pid = rcreate(p1, 1024, 20, "p1", 0);

  while(1){
    XDEBUG_KPRINTF(
        "------------------------------\n"
        "P1 Process\n"
        "------------------------------\n");
    check(pid);
    sleep(1);
  }

}

/* infinite loop */

void p1() {
  while (1){
    ;
  }
}


void check(pid32 pid) {

  XDEBUG_KPRINTF("Current milli time: %d\n", clktimemilli);

  uint32 lifetime = proclifetime(pid);
  XDEBUG_KPRINTF("Process %d has lifetime %d\n", pid, lifetime);

  uint32 grosscpumilli = procgrosscpu(pid);
  XDEBUG_KPRINTF("Process %d has gross cpu %d\n", pid, grosscpumilli);

  uint64 grosscpumicro = procgrosscpumicro(pid);
  XDEBUG_KPRINTF("Process %d has gross cpu ticks %d\n", pid, grosscpumicro);

}
