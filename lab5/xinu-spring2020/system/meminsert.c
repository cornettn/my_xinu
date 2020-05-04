/* meminsert.c - meminsert, memextract */

#include <xinu.h>


uint32 memblockflag;


void printblk(struct inusememblk *blk) {
  XDEBUG_KPRINTF("blk {\n"
                 "  memptr: 0x%08x (%d)\n"
                 "  memlen: %d\n"
                 "  mnext: 0x%08x (%d)\n"
                 "}\n", (long *) blk->memblockptr, (long *) blk->memblockptr,
                 blk->memlen, (long *) blk->mnext, (long *) blk->mnext);
}


/*--------------------------------------------------------------
 * meminsert - insert a block of memory into the processes mem list
 *--------------------------------------------------------------
 */

void meminsert(
    struct inusememblk *hdptr, /* the head of the list to insert into */
    char *memptr               /* memory to insert */
    )
{

  intmask mask = disable();

  uint32 len;
  struct inusememblk *currblk;
  struct inusememblk *newblk;


  len = strlen(memptr);

  // Ensure that len is not 0
  len += (len == 0) ? 8 : 0;

  // Ensure that len is a miltiple of 8
  len += (len % 8 == 0) ? 0 : (8 - (len % 8));

  memblockflag = 1;
  newblk = (struct inusememblk *) getmem(len);

  newblk->memlen = len;
  memptr = newblk->memblockptr;
  newblk->mnext = NULL;

  currblk = hdptr;

  if (currblk == NULL) {
    *hdptr = *newblk;
    hdptr->mnext = NULL;
  }
  else {

    /* Insert into list */
    while (currblk->mnext != NULL) {
      if (currblk->mnext->memlen < len) {
        currblk = currblk->mnext;
      }
      else {
        /* len of new block is more than currblk->next */

        newblk->mnext = currblk->mnext;
        currblk->mnext = newblk;
      }
    }
  }

  restore(mask);
}


