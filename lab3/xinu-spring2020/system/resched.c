/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

//  XDEBUG_KPRINTF("In resched\n");

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */

    /* Change the comparator to accomodate for non-decreasing sched. */

    if (ptold->prprio < firstkey(readylist)) {
      return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;

    /* Lab 3 - use risert instead of insert */
    rinsert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */


	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/



  /* Millisecond counter */

  uint32 beforecputime = clktimemilli;

  /* CPU clock ticks counter */

  uint64 beforecputick;
  asm volatile ( "rdtsc" : "=A"(beforecputick) );

  ctxsw(&ptold->prstkptr, &ptnew->prstkptr);


	/* Old process returns here when resumed */

  /* Lab 3 - 3/3/20 */
  /* Update the prgrosscpu accordingly */

  if (clktimemilli == beforecputime) {
    ptnew->prgrosscpu++;
  }
  else {
    ptnew->prgrosscpu += clktimemilli - beforecputime;
  }

  /* Updated the prgrosscputick accordingly */

  uint64 aftercputick;
  asm volatile ( "rdtsc" : "=A"(aftercputick) );

  ptnew->prgrosscputick += aftercputick - beforecputick;

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
