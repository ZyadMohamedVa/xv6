#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#define MAX_USERS 10
#define MAX_USERNAME 16
#define MAX_PASSWORD 16

struct user_entry {
  char username[MAX_USERNAME];
 uint password_hash;
  int uid;
  int used;
};

struct user_entry users[MAX_USERS];
uint
simple_hash(char *s)
{
  uint hash = 5381;
  int c;

  while((c = *s++) != 0)
    hash = ((hash << 5) + hash) + c;

  return hash;
}

int users_initialized = 0;

void
init_default_users(void)
{
  if(users_initialized)
    return;

  safestrcpy(users[0].username, "admin", MAX_USERNAME);
  users[0].password_hash = simple_hash("admin");
  users[0].uid = 0;
  users[0].used = 1;

  safestrcpy(users[1].username, "patient", MAX_USERNAME);
  users[1].password_hash = simple_hash("patient");
  users[1].uid = 1;
  users[1].used = 1;

  safestrcpy(users[2].username, "doctor", MAX_USERNAME);
  users[2].password_hash = simple_hash("doctor");
  users[2].uid = 2;
  users[2].used = 1;

  users_initialized = 1;
}
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_whoami(void)
{
  return myproc()->uid;
}


int
sys_useradd(void)
{
  char *username;
  char *password;
  int uid;
  int i;

  if(argstr(0, &username) < 0)
    return -1;

  if(argstr(1, &password) < 0)
    return -1;

  if(argint(2, &uid) < 0)
    return -1;

  for(i = 0; i < MAX_USERS; i++) {
      if(users[i].used && strncmp(users[i].username, username, MAX_USERNAME) == 0) {
      cprintf("useradd: user already exists\n");
      return -1;
    }
  }

  for(i = 0; i < MAX_USERS; i++) {
    if(users[i].used == 0) {
      safestrcpy(users[i].username, username, MAX_USERNAME);
      users[i].password_hash = simple_hash(password);
      users[i].uid = uid;
      users[i].used = 1;

      cprintf("useradd: created username=%s uid=%d hash=%d\n",
        username, uid, users[i].password_hash);
      return 0;
    }
  }

  cprintf("useradd: user table full\n");
  return -1;
}
int
sys_login(void)
{
  char *username;
  char *password;
  uint hash;
  int i;

  init_default_users();

  if(argstr(0, &username) < 0)
    return -1;

  if(argstr(1, &password) < 0)
    return -1;

  hash = simple_hash(password);

  for(i = 0; i < MAX_USERS; i++) {
    if(users[i].used &&
       strncmp(users[i].username, username, MAX_USERNAME) == 0 &&
       users[i].password_hash == hash) {

      myproc()->uid = users[i].uid;
      myproc()->gid = users[i].uid;

      cprintf("login: success username=%s uid=%d\n", username, users[i].uid);
      return users[i].uid;
    }
  }

  cprintf("login: failed username=%s\n", username);
  return -1;
}
int
sys_userdel(void)
{
  char *username;
  int i;

  if(argstr(0, &username) < 0)
    return -1;

  for(i = 0; i < MAX_USERS; i++) {
    if(users[i].used &&
       strncmp(users[i].username, username, MAX_USERNAME) == 0) {

      users[i].used = 0;
      users[i].username[0] = 0;
      users[i].password_hash = 0;
      users[i].uid = -1;

      cprintf("userdel: deleted username=%s\n", username);
      return 0;
    }
  }

  cprintf("userdel: user not found username=%s\n", username);
  return -1;
}

int
sys_passwd(void)
{
  char *username;
  char *oldpass;
  char *newpass;
  uint oldhash;
  uint newhash;
  int i;

  if(argstr(0, &username) < 0)
    return -1;

  if(argstr(1, &oldpass) < 0)
    return -1;

  if(argstr(2, &newpass) < 0)
    return -1;

  oldhash = simple_hash(oldpass);
  newhash = simple_hash(newpass);

  for(i = 0; i < MAX_USERS; i++) {
    if(users[i].used &&
       strncmp(users[i].username, username, MAX_USERNAME) == 0) {

      if(users[i].password_hash != oldhash) {
        cprintf("passwd: wrong old password for username=%s\n", username);
        return -1;
      }

      users[i].password_hash = newhash;
      cprintf("passwd: password changed username=%s newhash=%d\n",
              username, newhash);
      return 0;
    }
  }

  cprintf("passwd: user not found username=%s\n", username);
  return -1;
}

