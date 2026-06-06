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

static int
append_char(char *buf, int max, int off, char c)
{
  if(off < max-1)
    buf[off++] = c;
  return off;
}

static int
append_str(char *buf, int max, int off, char *s)
{
  while(*s)
    off = append_char(buf, max, off, *s++);
  return off;
}

static int
append_int(char *buf, int max, int off, int x)
{
  char tmp[16];
  int i = 0, j;

  if(x == 0)
    return append_char(buf, max, off, '0');

  if(x < 0){
    off = append_char(buf, max, off, '-');
    x = -x;
  }

  while(x > 0){
    tmp[i++] = '0' + (x % 10);
    x /= 10;
  }

  for(j = i - 1; j >= 0; j--)
    off = append_char(buf, max, off, tmp[j]);

  return off;
}

int
audit_read(char *buf, int max)
{
  int i, idx, off = 0;
  struct audit_entry e;

  acquire(&auditbuf.lock);

  for(i = 0; i < auditbuf.count; i++){
    idx = (auditbuf.next - auditbuf.count + i + AUDIT_SIZE) % AUDIT_SIZE;
    e = auditbuf.entries[idx];

    off = append_str(buf, max, off, "pid=");
    off = append_int(buf, max, off, e.pid);
    off = append_str(buf, max, off, " uid=");
    off = append_int(buf, max, off, e.uid);
    off = append_str(buf, max, off, " syscall=");
    off = append_int(buf, max, off, e.syscall_num);
    off = append_str(buf, max, off, " tick=");
    off = append_int(buf, max, off, e.tick);
    off = append_char(buf, max, off, '\n');
  }

  if(max > 0)
    buf[off < max ? off : max-1] = 0;

  release(&auditbuf.lock);
  return off;
}
