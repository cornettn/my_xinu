#include <xinu.h>

void mymotd(void) {
  char *name = "Noah Cornett";
  char *username = "cornettn";
  kprintf("\nHello! My name is %s and I am the developer of this version of"
     " xinu! My username is %s.", name, username);
}
