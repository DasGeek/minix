#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>

int trap_counter(void)
{
  message m;

  printf("In libc trap_counter");

  memset(&m, 0, sizeof(m));
  return( (int) _syscall(PM_PROC_NR, PM_TRAPCOUNT, &m));
}


void init_trap_counter(void)
{
  message m;

  printf("In libc init_trap_counter");

  memset(&m, 0, sizeof(m));
  _syscall(PM_PROC_NR, PM_TRAPINIT, &m);
  return;
}
