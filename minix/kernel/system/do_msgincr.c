/* The kernel call implemented in this file:
 *   m_type:	SYS_MSGINCR
 *
 * The parameters for this kernel call are:
 *   m_pm_msgcount.num        (number of times a msg call has happened since last clear)
 */

#include "kernel/system.h"

#include <string.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <minix/endpoint.h>
#include <minix/u64.h>

#if USE_MSGINCR


/*===========================================================================*
 *			        do_msgincr				     *
 *===========================================================================*/
int do_msgincr(struct proc *caller, message *m_ptr)
{
    extern int num_msgs;

    num_msgs += 1;
    m_ptr->m_pm_msgcount.num = num_msgs;
    printf("In do_msgincr(). Value of num_msgs is currently %u\n", num_msgs);

    return OK;
}

#endif /* USE_MSGINCR */

