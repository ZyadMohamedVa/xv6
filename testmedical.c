#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int passed = 0;
int failed = 0;

void
check(char *name, int ok)
{
  if(ok){
    printf(1, "[PASS] %s\n", name);
    passed++;
  } else {
    printf(1, "[FAIL] %s\n", name);
    failed++;
  }
}

void
write_file(char *path, char *data)
{
  int fd = open(path, O_CREATE | O_RDWR);
  if(fd >= 0){
    write(fd, data, strlen(data));
    close(fd);
  }
}

int
main(void)
{
  int fd;

  printf(1, "=== Medical Protected Files Test ===\n");

  mkdir("patient");
  mkdir("dosage");
  mkdir("device");

  write_file("patient/records", "patient medical records");
  write_file("dosage/insulin.log", "initial insulin log");
  write_file("device/config", "device configuration");

  chown("patient/records", 1);
  chmod("patient/records", 1);

  chown("dosage/insulin.log", 2);
  chmod("dosage/insulin.log", 3);

  chown("device/config", 0);
  chmod("device/config", 0);

  check("admin can read device config", open("device/config", O_RDONLY) >= 0);

  fd = open("device/config", O_RDONLY);
  if(fd >= 0)
    close(fd);

  login("patient", "patient");

  fd = open("patient/records", O_RDONLY);
  check("patient can read own records", fd >= 0);
  if(fd >= 0)
    close(fd);

  fd = open("patient/records", O_WRONLY);
  check("patient blocked from writing read-only records", fd < 0);
  if(fd >= 0)
    close(fd);

  fd = open("device/config", O_RDONLY);
  check("patient blocked from device config", fd < 0);
  if(fd >= 0)
    close(fd);

  login("doctor", "doctor");

  fd = open("dosage/insulin.log", O_WRONLY);
  check("doctor can write insulin log", fd >= 0);
  if(fd >= 0){
    write(fd, "doctor update", 13);
    close(fd);
  }

  login("admin", "admin");

  fd = open("device/config", O_WRONLY);
  check("admin bypasses and writes device config", fd >= 0);
  if(fd >= 0){
    write(fd, "admin update", 12);
    close(fd);
  }

  printf(1, "=== Medical File Test Result: passed=%d failed=%d ===\n", passed, failed);

  if(failed == 0)
    printf(1, "FINAL RESULT: PASS\n");
  else
    printf(1, "FINAL RESULT: FAIL\n");

  exit();
}
