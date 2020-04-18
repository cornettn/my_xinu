/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)


pid32 recvpid;
umsg32 ubuffer;

void sender_proc() {
  send(recvpid, 1);
  XDEBUG_KPRINTF("msg sent\n");
}

void cbuser() {
  XDEBUG_KPRINTF("in callback function\n");
}

void recv_proc() {
  if (cbregister(&cbuser, &ubuffer) != OK) {
    XDEBUG_KPRINTF("cb func did not register\n");
    userret();
  }

  XDEBUG_KPRINTF("cb func registered\n");

  sleepms(500);

  XDEBUG_KPRINTF("out of sleep\n");
}

process	main(void)
{

  resume(create(sender_proc, 1024, 5, "sender", 0, NULL));
  recvpid = create(recv_proc, 1024, 5, "recv", 0, NULL);
  resume(recvpid);

}
