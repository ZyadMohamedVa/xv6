#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int fd;
  struct stat st;

  printf(1, "=== Phase 2 Permission Test ===\n");

  fd = open("permfile", O_CREATE | O_RDWR);
  if(fd < 0){
    printf(1, "create failed\n");
    exit();
  }

  write(fd, "hello", 5);
  close(fd);

  if(stat("permfile", &st) == 0)
    printf(1, "owner=%d mode=%d\n", st.owner, st.mode);

  chmod("permfile", 1);

  if(stat("permfile", &st) == 0)
    printf(1, "after chmod: owner=%d mode=%d\n", st.owner, st.mode);

  chown("permfile", 2);

  if(stat("permfile", &st) == 0)
    printf(1, "after chown: owner=%d mode=%d\n", st.owner, st.mode);

  exit();
}
