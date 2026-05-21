#include "types.h"
#include "stat.h"
#include "user.h"

void
print_role(void)
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
}

int
main(void)
{
  printf(1, "Creating users...\n");

  useradd("patient", "patient", 1);
  useradd("doctor", "doctor", 2);

  printf(1, "\nCurrent role before login:\n");
  print_role();

  printf(1, "\nLogging in as patient...\n");
  if(login("patient", "patient") >= 0)
    printf(1, "patient login successful\n");
  else
    printf(1, "patient login failed\n");

  printf(1, "Current role after patient login:\n");
  print_role();

  printf(1, "\nTrying wrong password for doctor...\n");
  if(login("doctor", "wrong") >= 0)
    printf(1, "doctor login successful - ERROR\n");
  else
    printf(1, "doctor login failed as expected\n");

  printf(1, "Current role after failed doctor login:\n");
  print_role();

  printf(1, "\nLogging in as doctor...\n");
  if(login("doctor", "doctor") >= 0)
    printf(1, "doctor login successful\n");
  else
    printf(1, "doctor login failed\n");

  printf(1, "Current role after doctor login:\n");
  print_role();

  exit();
}
