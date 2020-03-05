/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)

#define PART_THREE (1)


void check(pid32);
void p1();

process	main(void)
{

  #ifdef PART_THREE

  pid32 pid = rcreate(p1, 1024, 20, "p1", 0);

  while(1){
    XDEBUG_KPRINTF(
        "------------------------------\n"
        "P1 Process\n"
        "------------------------------\n");
    check(pid);
    sleep(1);
  }

  #endif


}

/* infinite loop */

void p1() {
  while (1){
    ;
  }
}


void check(pid32 pid) {

  uint32 grosscpumilli = procgrosscpu(pid);
  XDEBUG_KPRINTF("Process %d has gross cpu %d\n", pid, grosscpumilli);

  uint64 grosscpumicro = procgrosscpumicro(pid);
  XDEBUG_KPRINTF("Process %d has gross cpu ticks %d\n", pid, grosscpumicro);

  XDEBUG_KPRINTF("Difference in micro seconds: %d\n", grosscpumilli * 1000 - grosscpumicro);

}
