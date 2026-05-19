#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int uid = whoami();

  if(uid == 0)
    printf(1, "UID=%d ROLE=ADMIN\n", uid);
  else if(uid == 1)
    printf(1, "UID=%d ROLE=PATIENT\n", uid);
  else if(uid == 2)
    printf(1, "UID=%d ROLE=DOCTOR\n", uid);
  else
    printf(1, "UID=%d ROLE=UNKNOWN\n", uid);

  exit();
}
