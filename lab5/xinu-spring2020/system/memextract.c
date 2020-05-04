/* memextract.c - memextract */

#include <xinu.h>

/*--------------------------------------------------------------
 * meminsert - insert a block of memory into the processes mem list
 *--------------------------------------------------------------
 */

void memextract(
    struct inusememblk *hdptr, /* the head of the list to insert into */
    char *memptr               /* memory to insert */
    )
{

  intmask mask = disable();

  struct inusememblk *currblk;

  currblk = hdptr;

  if (currblk->memblockptr == memptr) {
    hdptr = currblk->mnext;
    memblockflag = 1;
    freemem(memptr, currblk->memlen);
    return;
  }

  /* Iterate through list to find */

  while (TRUE) {
    if (currblk->mnext->memblockptr == memptr) {
      currblk->mnext = currblk->mnext->mnext;
      memblockflag = 1;
      freemem(memptr, currblk->mnext->memlen);
      break;
    }

    if (currblk->mnext == NULL) {
      break;
    }
  }

  restore(mask);
}


