#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char username[32];
  char password[32];
  char *argv[] = { "sh", 0 };
  int i;

  while(1) {
    printf(1, "\n=== xv6 Medical Device Login ===\n");

    printf(1, "Username: ");
    gets(username, sizeof(username));

    printf(1, "Password: ");
    gets(password, sizeof(password));

    // Remove newline from username
    for(i = 0; username[i] != 0; i++) {
      if(username[i] == '\n') {
        username[i] = 0;
        break;
      }
    }

    // Remove newline from password
    for(i = 0; password[i] != 0; i++) {
      if(password[i] == '\n') {
        password[i] = 0;
        break;
      }
    }

    if(login(username, password) >= 0) {
      printf(1, "Login successful. Starting shell...\n");
      exec("sh", argv);
      printf(1, "login: exec sh failed\n");
      exit();
    } else {
      printf(1, "Login failed. Try again.\n");
    }
  }
}

