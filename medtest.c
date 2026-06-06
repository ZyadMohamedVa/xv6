#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int passed = 0;
int failed = 0;

void
check(char *name, int condition)
{
  if(condition){
    printf(1, "[PASS] %s\n", name);
    passed++;
  } else {
    printf(1, "[FAIL] %s\n", name);
    failed++;
  }
}

int
main(void)
{
  int fd, n;
  int before, after;
  struct stat st;
  char buf[80];

  printf(1, "=== Medical Device Security Compliance Test ===\n");

  check("whoami syscall works", whoami() >= 0);
  check("running as admin", whoami() == 0);
  check("invalid login rejected", login("doctor", "wrong") < 0);
  check("valid admin login accepted", login("admin", "admin") >= 0);

  before = auditread(buf, sizeof(buf));
  check("integration attack denied", login("doctor", "wrong") < 0);
  after = auditread(buf, sizeof(buf));
  check("integration denial visible in audit log", after >= before && after > 0);

  unlink("bonusfile");

  fd = open("bonusfile", O_CREATE | O_RDWR);
  check("create file", fd >= 0);
  if(fd >= 0){
    write(fd, "secure", 6);
    close(fd);
  }

  check("stat reads file metadata", stat("bonusfile", &st) == 0);
  check("file has owner field", st.owner >= 0);
  check("file has default mode 3", st.mode == 3);

  check("chmod changes permission", chmod("bonusfile", 1) == 0);
  stat("bonusfile", &st);
  check("mode changed to read-only", st.mode == 1);

  check("chown changes owner", chown("bonusfile", 2) == 0);
  stat("bonusfile", &st);
  check("owner changed to doctor uid 2", st.owner == 2);

  n = auditread(buf, sizeof(buf));
  check("admin can read audit log", n >= 0);
  check("audit log contains entries", n > 0);

  if(n > 0){
    printf(1, "Audit evidence UID stream: %s\n", buf);
  }

  printf(1, "=== Compliance Result: passed=%d failed=%d ===\n", passed, failed);

  if(failed == 0)
    printf(1, "FINAL RESULT: PASS\n");
  else
    printf(1, "FINAL RESULT: FAIL\n");

  exit();
}
