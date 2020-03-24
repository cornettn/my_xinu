/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)

pid32 recvpid;
void sendmsg();
void sendmoremsg();
void recvmsg();

process	main(void)
{
  rcreate(sendmsg, 1024, 100, "sendmsg", 0, NULL);
  rcreate(sendmoremsg, 1024, 100, "sendmoremsg", 0 , NULL);
  recvpid = rcreate(recvmsg, 1024, 100, "recvmsg", 0, NULL);
  bsend(recvpid, 50000);

}

void sendmsg() {
  sleepms(50);

  bsend(recvpid, 1);
  bsend(recvpid, 2);
  bsend(recvpid, 3);
}

void sendmoremsg() {
  sleepms(50);

  bsend(recvpid, 101);
  bsend(recvpid, 102);
  bsend(recvpid, 103);
}

void recvmsg() {
  sleepms(1000);

  while(1) {
    int32 msg = receive();
    XDEBUG_KPRINTF("recvmsg: received %d\n", msg);
  }
}

