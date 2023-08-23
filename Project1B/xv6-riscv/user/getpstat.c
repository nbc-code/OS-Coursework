#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

int
main(void)// Program for testing purposes
{
  struct pstat stats;
  getpstat(&stats); // getpstat() has print statements for testing
  exit(0);
}

