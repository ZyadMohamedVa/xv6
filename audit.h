#ifndef AUDIT_H
#define AUDIT_H

#define AUDIT_SIZE 64

struct audit_entry {
  int pid;
  int uid;
  int tick;
  int syscall_num;
  int eip;
};

void audit_log(int pid, int uid, int syscall_num, int eip);
int audit_read(char *buf, int max);

#endif
