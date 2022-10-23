/* The kernel call implemented in this file:
 *   m_type:	SYS_TRAPCOUNT
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

#if USE_TRAPCOUNT


/*===========================================================================*
 *			        do_trapcount				     *
 *===========================================================================*/
int do_trapcount(struct proc *caller, message *m_ptr)
{
    extern int num_traps;

    m_ptr->m_pm_trapcount.num = num_traps;
    printf("In do_trapcount. num_traps value is currently %u\n", num_traps);

    return OK;
}

#endif /* USE_TRAPCOUNT */

