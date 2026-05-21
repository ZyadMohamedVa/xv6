#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  printf(1, "Testing useradd syscall...\n");

  if(useradd("nurse", "nurse", 3) == 0)
    printf(1, "PASS: nurse user created successfully\n");
  else
    printf(1, "FAIL: nurse user creation failed\n");

  if(useradd("nurse", "nurse", 3) < 0)
    printf(1, "PASS: duplicate nurse rejected\n");
  else
    printf(1, "FAIL: duplicate nurse accepted\n");

  exit();
}
