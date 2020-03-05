/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */


	prptr = &proctab[pid];

  if (prptr->prstate == PR_SLEEP) {
    if (currpid == NULLPROC) {
      prptr->prvgrosscpu = proctab[NULLPROC].prvgrosscpu++;
    }
    else {
      prptr->prvgrosscpu = firstkey(readylist);
      if (firstid(readylist) == NULLPROC) {
        firstkey(readylist)++;
        proctab[NULLPROC].prvgrosscpu++;
      }
    }
  }

  prptr->prstate = PR_READY;

  /* Lab 3 - Use rinsert instead of insert to accomodate for
   *         non-decreasing shed. */

  rinsert(pid, readylist, prptr->prvgrosscpu);
	resched();

	return OK;
}
