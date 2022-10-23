/* Sytem calls for the trap counter functionality for CS 551 Project 2
 *
 * The entry points into this file are:
 *   trap_counter:
 *   init_trap_counter:
 */

#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/config.h>
#include <minix/sysinfo.h>
#include <minix/type.h>
#include <minix/ds.h>
#include <machine/archtypes.h>
#include <lib.h>
#include <assert.h>
#include "mproc.h"

/*===========================================================================*
 *				trap_counter				     *
 *===========================================================================*/
int trap_counter()
{
    register struct mproc *sending_proc = mp;
    int num_traps = 0;
    int ret = 0;
    
    printf("In trap_counter()\n");
    ret = sys_trapcount(sending_proc->mp_endpoint, &num_traps);

    /* If the kernel call failed, print a message with the error code. */
    if (ret != OK)
    {
        printf("PM: trap_counter failed [%d].\n", ret);
        return ret;
    }

    /* Otherwise, return the value of the counted traps. */
    return(num_traps);
}


/*===========================================================================*
 *				init_trap_counter			       	     *
 *===========================================================================*/
int init_trap_counter()
{
    register struct mproc *sending_proc = mp;
    int num_traps = 0;
    int ret = 0;

    printf("In init_trap_counter()\n");
    ret = sys_trapinit(sending_proc->mp_endpoint, &num_traps);

    /* If the kernel call failed, print a message with the error code. */
    if (ret != OK)
    {
        printf("PM: init_trap_counter failed [%d].\n", ret);
        return ret;
    }

    /* If somehow the value didn't get set properly, print error here. */
    if (num_traps != 0)
    {
        printf("PM: init_trap_counter didn't set value to 0! Value: [%u].\n", num_traps);
        return -1;
    }

    /* Otherwise, return 0. */
    return 0;
}

