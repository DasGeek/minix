#include <sys/cdefs.h>
#include <sys/resource.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int nicer_to(int pid, int prio_change)
{
    message m;

    printf("In libc nicer_to()\n");

    // There are no negative PIDs, so don't bother
    // continuing if one is specified.
    if (pid < 0)
    {
        printf("ERROR: libc nicer_to() - invalid PID parameter specified!\n");
        return EINVAL;
    }

    // If the value to change the priority by would always result
    // in a value outside the bounds of what's allowed, don't continue.
    if (abs(prio_change) > PRIO_MAX)
    {
        printf("ERROR: libc nicer_to() - invalid priority parameter specified!\n");
        return EINVAL;
    }

    memset(&m, 0, sizeof(m));
    m.m_lc_pm_nicer_to.who = pid;
    m.m_lc_pm_nicer_to.prio = prio_change;

    return( (int) _syscall(PM_PROC_NR, PM_NICERTO, &m));
}
