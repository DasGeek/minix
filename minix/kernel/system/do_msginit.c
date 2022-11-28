/* The kernel call implemented in this file:
 *   m_type:	SYS_MSGINIT
 *
 * The parameters for this kernel call are:
 *   m_pm_msgcount.num        (number of times a message has been passed since last clear)
 */

#include "kernel/system.h"

#include <string.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <minix/endpoint.h>
#include <minix/u64.h>


#if USE_MSGINIT

/*===========================================================================*
 *			        do_msginit	                            			     *
 *===========================================================================*/
int do_msginit(struct proc *caller, message *m_ptr)
{
    extern int num_msgs;

    num_msgs = 0;
    m_ptr->m_pm_msgcount.num = 0;

    printf("In do_msginit. num_msgs value is currently %u\n", num_msgs);

    return OK;
}

#endif /* USE_MSGINIT */

