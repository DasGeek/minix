#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>

int msg_counter(void)
{
  message m;

  printf("In libc msg_counter()\n");

  memset(&m, 0, sizeof(m));
  return( (int) _syscall(PM_PROC_NR, PM_MSGCOUNT, &m));
}


void init_msg_counter(void)
{
  message m;

  printf("In libc init_msg_counter()\n");

  memset(&m, 0, sizeof(m));
  _syscall(PM_PROC_NR, PM_MSGINIT, &m);
  return;
}
