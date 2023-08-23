#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  /*
  * Input such as $ nice 19 wc README, will result in main's argv[] as such.
  * argv[0] = "nice"
  * argv[1] = "19"
  * argv[2] = "wc"
  * argv[3] = "README"
  * argv[4] = 0 // the array ends with a null string
  */

  if(argc <= 2)// Check number of arguments, used to at least change the nice value of the process
  {
    printf("Too few arguments. Follow format example: \"nice 19 wc README\"\n");
    exit(0);
  }

  int nv = atoi(argv[1]); // convert nice value from command line

  if(nv > 19) // Check value, no need to check for negative values because of implementation of atoi
  {
    printf("Nice value too large, cannot exceed 19\n");
    exit(0);
  }

  char **args = (char**)&argv[2];
  nice(nv); // Set nice value
  if(argc >= 3) // no need to call exec if only setting nice value
  {
    exec(argv[2], args);
  }

  exit(0);
}
