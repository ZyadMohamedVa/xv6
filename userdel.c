#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf(1, "Usage: userdel <username>\n");
    exit();
  }

  if(userdel(argv[1]) == 0)
    printf(1, "userdel: deleted %s\n", argv[1]);
  else
    printf(1, "userdel: failed\n");

  exit();
}
