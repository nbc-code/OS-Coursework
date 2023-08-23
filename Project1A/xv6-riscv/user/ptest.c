// Copied from user/zombie.c
/*
*  Noah Cantrell, nbc@iastate.edu
*
*  Question answer:
*
*  fork() is used to create a new process by duplicating the calling process.
*  So, based on this information as well as where we were told to place the print statements,
*  I would expect that the two print statements would be different and that second print
*  statement would produce a larger number than the first because we are creating a new
*  process inbetween print statements.
*
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("Before fork(): %d\n", pcount());

  if(fork() > 0)
    sleep(5);  // Let child exit before parent.

  printf("After fork(): %d\n", pcount());

  exit(0);
}
