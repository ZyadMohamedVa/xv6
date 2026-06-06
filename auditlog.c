#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char buf[512];
  int n;

  printf(1, "=== Kernel Audit Log ===\n");

  n = auditread(buf, sizeof(buf)-1);

  if(n < 0){
    printf(1, "auditread denied: non-admin user\n");
    exit();
  }

  buf[n] = 0;
  printf(1, "%s\n", buf);

  exit();
}
