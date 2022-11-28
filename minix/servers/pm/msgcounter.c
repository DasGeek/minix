/* Sytem calls for the msg counter functionality for CS 551 Project 2
 *
 * The entry points into this file are:
 *   msg_counter:
 *   init_msg_counter:
 */

#include "pm.h"
#include <minix/callnr.h>
#include <signal.h>
#include <sys/svrctl.h>
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
 *				msg_counter				     *
 *===========================================================================*/
int msg_counter()
{
    register struct mproc *sending_proc = mp;
    int num_msgs = 0;
    int ret = 0;

    printf("In msg_counter()\n");
    ret = sys_msgcount(sending_proc->mp_endpoint, &num_msgs);

    /* If the kernel call failed, print a message with the error code. */
    if (ret != OK)
    {
        printf("PM: msg_counter failed [%d].\n", ret);
        return ret;
    }

    /* Otherwise, return the value of the counted messages. */
    return(num_msgs);
}

/*===========================================================================*
 *				msg_increment				     *
 *===========================================================================*/
int msg_increment()
{
    register struct mproc *sending_proc = mp;
    int num_msgs = 0;
    int ret = 0;
    
    printf("In msg_increment()\n");
    ret = sys_msgincr(sending_proc->mp_endpoint, &num_msgs);

    /* If the kernel call failed, print a message with the error code. */
    if (ret != OK)
    {
        printf("PM: msg_increment failed [%d].\n", ret);
        return ret;
    }

    /* Otherwise, return 0 */
    return OK;
}

/*===========================================================================*
 *				init_msg_counter			       	     *
 *===========================================================================*/
int init_msg_counter()
{
    register struct mproc *sending_proc = mp;
    int num_msgs = 0;
    int ret = 0;

    printf("In init_msg_counter()\n");
    ret = sys_msginit(sending_proc->mp_endpoint, &num_msgs);

    /* If the kernel call failed, print a message with the error code. */
    if (ret != OK)
    {
        printf("PM: init_msg_counter failed [%d].\n", ret);
        return ret;
    }

    /* If somehow the value didn't get set properly, print error here. */
    if (num_msgs != 0)
    {
        printf("PM: init_msg_counter didn't set value to 0! Value: [%u].\n", num_msgs);
        return -1;
    }

    /* Otherwise, return 0. */
    return 0;
}

