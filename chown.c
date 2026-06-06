#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(1, "Usage: chownfile <file> <uid>\n");
    printf(1, "uid: 0=admin, 1=patient, 2=doctor\n");
    exit();
  }

  if(chown(argv[1], atoi(argv[2])) == 0)
    printf(1, "chownfile: changed %s owner to uid %d\n", argv[1], atoi(argv[2]));
  else
    printf(1, "chownfile: failed\n");

  exit();
}
