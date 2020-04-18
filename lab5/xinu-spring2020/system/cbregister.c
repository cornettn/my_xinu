/* cbregister.c - cbregister */

#include <xinu.h>

/*---------------------------------------------------
 * cbregister - callback function
 *---------------------------------------------------
 */

syscall cbregister(
    void (* fp)(void), /* a function pointer to a user callback */
    umsg32 *mbufptr    /* a pointer to a 1 word msg buffer */
    )
{

  struct procent *prptr;

  /* Check to ensure that fp lies within the bounds of text segment, and
   * mbufptr does not point inside the the text segment */
  if (!( ((uint32)&fp < (uint32)&etext) && /* fp < end of text */
         ((uint32)&fp >= (uint32)&text) /* fp >= beginning of text */
       )) {

    XDEBUG_KPRINTF("fp out of range: %08X\n", (uint32) &fp);

    /* fp is not within text segment bounds */
    return SYSERR;
  }

  /* fp is within bounds, so check membufptr */
  if (!((uint32)&mbufptr > (uint32)&etext)) {

    XDEBUG_KPRINTF("mbufptr out of range\n");

    /* mbufptr is inside text */
    return SYSERR;
   }

  /* Both arguments are now good to go */

  prptr->prcbptr = fp;
  prptr->prcbvalid = TRUE;

  return OK;
}
