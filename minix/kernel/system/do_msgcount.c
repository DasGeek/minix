/* The kernel call implemented in this file:
 *   m_type:	SYS_MSGCOUNT
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

#if USE_MSGCOUNT


/*===========================================================================*
 *			        do_msgcount				     *
 *===========================================================================*/
int do_msgcount(struct proc *caller, message *m_ptr)
{
    extern int num_msgs;

    m_ptr->m_pm_msgcount.num = num_msgs;
    printf("In do_msgcount. num_msgs value is currently %u\n", num_msgs);

    return OK;
}

#endif /* USE_MSGCOUNT */

