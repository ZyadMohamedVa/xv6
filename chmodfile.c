#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(1, "Usage: chmodfile <file> <mode>\n");
    printf(1, "mode: 1=read, 2=write, 3=read+write\n");
    exit();
  }

  if(chmod(argv[1], atoi(argv[2])) == 0)
    printf(1, "chmodfile: changed %s mode to %d\n", argv[1], atoi(argv[2]));
  else
    printf(1, "chmodfile: failed\n");

  exit();
}
