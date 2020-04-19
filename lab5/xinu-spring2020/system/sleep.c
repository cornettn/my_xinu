/* sleep.c - sleep sleepms */

#include <xinu.h>

#define	MAXSECONDS	2147483		/* Max seconds per 32-bit msec	*/

/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  int32	delay		/* Time to delay in seconds	*/
	)
{
	if ( (delay < 0) || (delay > MAXSECONDS) ) {
		return SYSERR;
	}
	sleepms(1000*delay);
	return OK;
}

/*------------------------------------------------------------------------
 *  sleepms  -  Delay the calling process n milliseconds
 *------------------------------------------------------------------------
 */
syscall	sleepms(
	  int32	delay			/* Time to delay in msec.	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

	if (delay < 0) {
		return SYSERR;
	}

	if (delay == 0) {
		yield();
		return OK;
	}

	/* Delay calling process */

	mask = disable();
	if (insertd(currpid, sleepq, delay) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	proctab[currpid].prstate = PR_SLEEP;

  /* Sleep the process */
  resched();

  /* Process is awake now */

  struct procent *prptr = &proctab[currpid];
  if (prptr->prcbvalid) {

    XDEBUG_KPRINTF("sleepms: changing return addrs\n");

    /* Process has a callback function */
    long ogretaddr;
    long *saddr;
    long *ebp;

    saddr = (long *) prptr->prstkptr;
    ebp = saddr + 2;
    saddr = (long *)(*ebp);

    /* addr is now the base pointer */
    ogretaddr = *++saddr;

    /* Change the ret addr to be the call back function */
    *saddr = (long *) prptr->prcbptr;

  }

  restore(mask);
	return OK;
}
