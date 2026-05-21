#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char buf[80];
  int n;

  printf(1, "=== Phase 3 Audit Test ===\n");

  n = auditread(buf, sizeof(buf));

  if(n < 0){
    printf(1, "auditread denied: non-admin user\n");
  } else {
    printf(1, "audit entries read=%d\n", n);
  }

  exit();
}
