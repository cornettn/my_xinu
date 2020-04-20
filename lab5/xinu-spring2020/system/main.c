/*  main.c  - main */

#include <xinu.h>

#define UNUSED(x) (void)(x)

#define PART_3_CASE_I (0)
#define PART_3_CASE_II (1)

pid32 recvpid;
umsg32 ubuffer;

void sender_proc() {

  /* Allow recv proc to register cb func */
  sleepms(1000);

  send(recvpid, 21);
  XDEBUG_KPRINTF("sender_proc: msg sent\n");
}

void recv_proc() {

  long *cbvalid = (long *) &proctab[recvpid].prcbvalid;
  long *tmpvalid = (long *) &proctab[recvpid].prtmpvalid;
  long *tmpbuf = (long *) &proctab[recvpid].prtmpbuf;
  long *mbuf = (long *) &proctab[recvpid].prmbufptr;
  long *ent = (long *) &proctab[recvpid];
  long *cbf = (long *) &proctab[recvpid].prcbptr;
  long *ubuf = (long *) &ubuffer;

  XDEBUG_KPRINTF("Address of proctab[currpid]: 0x%08x (%d)\n", ent, ent);
  XDEBUG_KPRINTF("Address of prcbvalid: 0x%08x (%d)\n", cbvalid, cbvalid);
  XDEBUG_KPRINTF("Address of prtmpvalid: 0x%08x (%d)\n", tmpvalid, tmpvalid);
  XDEBUG_KPRINTF("Address of prtmpbuf: 0x%08x (%d)\n", tmpbuf, tmpbuf);
  XDEBUG_KPRINTF("Address of prmbufptr: 0x%08x (%d)\n", mbuf, mbuf);
  XDEBUG_KPRINTF("Address of prcbptr: 0x%08x (%d)\n", cbf, cbf);
  XDEBUG_KPRINTF("Address of ubuffer: 0x%08x (%d)\n", ubuf, ubuf);

  XDEBUG_KPRINTF("Offset for prcbvalid: %d\n", cbvalid - ent);
  XDEBUG_KPRINTF("Offset for prtmpvalid: %d\n", cbvalid - tmpvalid);
  XDEBUG_KPRINTF("Offset for prtmpbuf (from prtmpvalid) %d\n", tmpvalid - tmpbuf);
  XDEBUG_KPRINTF("Offset for prmbufptr (from prtmpvalid) %d\n", tmpvalid - mbuf);
  XDEBUG_KPRINTF("Offset for prcbotr (from ent) %d\n", cbf - ent);

  XDEBUG_KPRINTF("recv_proc: &cbuser: 0x%08x (%d)\n", &cbuser, &cbuser);
  if (cbregister(&cbuser, &ubuffer) != OK) {
    XDEBUG_KPRINTF("recv_proc: cb func did not register\n");
    userret();
  }

  XDEBUG_KPRINTF("recv_proc: cb func registered\n");

  #if PART_3_CASE_I
  sleepms(500);
  XDEBUG_KPRINTF("recv_proc: out of sleep\n");
  XDEBUG_KPRINTF("weeeeeeee!\n");
  #elif PART_3_CASE_II

  /* ensures that the time slice will be depleted */

  XDEBUG_KPRINTF("recv_proc: starting loop\n");
  uint32 counter = 0;
  while(TRUE) {
    counter++;
    if (counter % 1000000 == 0) {
      XDEBUG_KPRINTF("in recv_proc\n");
      counter = 0;
    }
  }

  #endif

}

process	main(void)
{
  XDEBUG_KPRINTF("main: starting\n");
  resume(create(sender_proc, 1024, 5, "sender", 0, NULL));
  recvpid = create(recv_proc, 1024, 5, "recv", 0, NULL);
  resume(recvpid);

  XDEBUG_KPRINTF("&proctab: 0x%08x (%d)\n",
      &proctab, &proctab);
  XDEBUG_KPRINTF("&proctab[currpid]: 0x%08x (%d)\n",
      &proctab[currpid], &proctab[currpid]);
  XDEBUG_KPRINTF("size of procent: %d\n", sizeof(struct procent));




  XDEBUG_KPRINTF("recv pid: %d\n", recvpid);

}
