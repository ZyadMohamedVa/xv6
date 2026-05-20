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
  printf(1, "=== AUTHENTICATION FULL TEST ===\n");

  printf(1, "\n[1] Create users\n");
  useradd("patient", "1234", 1);
  useradd("doctor", "abcd", 2);

  printf(1, "\n[2] Current role before login\n");
  print_role();

  printf(1, "\n[3] Login patient with correct password\n");
  if(login("patient", "1234") >= 0)
    printf(1, "PASS: patient login accepted\n");
  else
    printf(1, "FAIL: patient login rejected\n");
  print_role();

  printf(1, "\n[4] Try doctor login with wrong password\n");
  if(login("doctor", "wrong") < 0)
    printf(1, "PASS: wrong doctor password rejected\n");
  else
    printf(1, "FAIL: wrong doctor password accepted\n");
  print_role();

  printf(1, "\n[5] Change patient password with wrong old password\n");
  if(passwd("patient", "badold", "9999") < 0)
    printf(1, "PASS: passwd rejected wrong old password\n");
  else
    printf(1, "FAIL: passwd accepted wrong old password\n");

  printf(1, "\n[6] Change patient password with correct old password\n");
  if(passwd("patient", "1234", "9999") == 0)
    printf(1, "PASS: patient password changed\n");
  else
    printf(1, "FAIL: patient password not changed\n");

  printf(1, "\n[7] Login patient with old password\n");
  if(login("patient", "1234") < 0)
    printf(1, "PASS: old password rejected\n");
  else
    printf(1, "FAIL: old password still accepted\n");

  printf(1, "\n[8] Login patient with new password\n");
  if(login("patient", "9999") >= 0)
    printf(1, "PASS: new password accepted\n");
  else
    printf(1, "FAIL: new password rejected\n");
  print_role();

  printf(1, "\n[9] Delete doctor user\n");
  if(userdel("doctor") == 0)
    printf(1, "PASS: doctor deleted\n");
  else
    printf(1, "FAIL: doctor not deleted\n");

  printf(1, "\n[10] Login deleted doctor\n");
  if(login("doctor", "abcd") < 0)
    printf(1, "PASS: deleted doctor cannot login\n");
  else
    printf(1, "FAIL: deleted doctor can still login\n");

  printf(1, "\n=== AUTHENTICATION TEST COMPLETE ===\n");

  exit();
}
