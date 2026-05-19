#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "Testing useradd syscall...\n");

  if(useradd("patient", "1234", 1) == 0)
    printf(1, "patient user created successfully\n");
  else
    printf(1, "patient user creation failed\n");

  if(useradd("doctor", "1234", 2) == 0)
    printf(1, "doctor user created successfully\n");
  else
    printf(1, "doctor user creation failed\n");

  exit();
}
