/* The kernel call implemented in this file:
 *   m_type:	SYS_TRAPINIT
 *
 * The parameters for this kernel call are:
 *   m_pm_trapcount.num        (number of times a trap call has happened since last clear)
 */

#include "kernel/system.h"

#include <string.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <minix/endpoint.h>
#include <minix/u64.h>


#if USE_TRAPINIT

/*===========================================================================*
 *			        do_trapinit	                            			     *
 *===========================================================================*/
int do_trapinit(struct proc *caller, message *m_ptr)
{
    extern int num_traps;

    num_traps = 0;
    m_ptr->m_pm_trapcount.num = 0;

    printf("In do_trapinit. num_traps value is currently %u\n", num_traps);

    return OK;
}

#endif /* USE_TRAPINIT */

