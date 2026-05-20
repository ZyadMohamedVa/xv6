#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "audit.h"

struct {
  struct spinlock lock;
  struct audit_entry entries[AUDIT_SIZE];
  int next;
  int count;
} auditbuf;

void
audit_log(int pid, int uid, int syscall_num, int eip)
{
  acquire(&auditbuf.lock);

  auditbuf.entries[auditbuf.next].pid = pid;
  auditbuf.entries[auditbuf.next].uid = uid;
  auditbuf.entries[auditbuf.next].tick = ticks;
  auditbuf.entries[auditbuf.next].syscall_num = syscall_num;
  auditbuf.entries[auditbuf.next].eip = eip;

  auditbuf.next = (auditbuf.next + 1) % AUDIT_SIZE;
  if(auditbuf.count < AUDIT_SIZE)
    auditbuf.count++;

  release(&auditbuf.lock);
}

int
audit_read(char *buf, int max)
{
  int i, n;

  acquire(&auditbuf.lock);

  n = auditbuf.count;
  if(n > max)
    n = max;

  for(i = 0; i < n; i++)
    buf[i] = 'A';

  release(&auditbuf.lock);
  return n;
}
