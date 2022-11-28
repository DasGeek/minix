/* This file contains some utility routines for PM.
 *
 * The entry points are:
 *   get_free_pid:	get a free process or group id
 *   find_param:	look up a boot monitor parameter
 *   find_proc:		return process pointer from pid number
 *   nice_to_priority	convert nice level to priority queue
 *   pm_isokendpt:	check the validity of an endpoint
 *   tell_vfs:		send a request to VFS on behalf of a process
 */

#include "pm.h"
#include <sys/resource.h>
#include <sys/stat.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/endpoint.h>
#include <fcntl.h>
#include <signal.h>		/* needed only because mproc.h needs it */
#include "mproc.h"

#include <minix/config.h>
#include <minix/timers.h>
#include <machine/archtypes.h>
#include "kernel/const.h"
#include "kernel/config.h"
#include "kernel/type.h"
#include "kernel/proc.h"

/*===========================================================================*
 *				get_free_pid				     *
 *===========================================================================*/
pid_t get_free_pid()
{
  static pid_t next_pid = INIT_PID + 1;		/* next pid to be assigned */
  register struct mproc *rmp;			/* check process table */
  int t;					/* zero if pid still free */

  /* Find a free pid for the child and put it in the table. */
  do {
	t = 0;			
	next_pid = (next_pid < NR_PIDS ? next_pid + 1 : INIT_PID + 1);
	for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++)
		if (rmp->mp_pid == next_pid || rmp->mp_procgrp == next_pid) {
			t = 1;
			break;
		}
  } while (t);					/* 't' = 0 means pid free */
  return(next_pid);
}

/*===========================================================================*
 *				find_param				     *
 *===========================================================================*/
char *find_param(name)
const char *name;
{
  register const char *namep;
  register char *envp;

  for (envp = (char *) monitor_params; *envp != 0;) {
	for (namep = name; *namep != 0 && *namep == *envp; namep++, envp++)
		;
	if (*namep == '\0' && *envp == '=') 
		return(envp + 1);
	while (*envp++ != 0)
		;
  }
  return(NULL);
}

/*===========================================================================*
 *				find_proc  				     *
 *===========================================================================*/
struct mproc *find_proc(lpid)
pid_t lpid;
{
  register struct mproc *rmp;

  for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++)
	if ((rmp->mp_flags & IN_USE) && rmp->mp_pid == lpid)
		return(rmp);

  return(NULL);
}

/*===========================================================================*
 *				nice_to_priority			     *
 *===========================================================================*/
int nice_to_priority(int nice, unsigned* new_q)
{
	if (nice < PRIO_MIN || nice > PRIO_MAX) return(EINVAL);

	*new_q = MAX_USER_Q + (nice-PRIO_MIN) * (MIN_USER_Q-MAX_USER_Q+1) /
	    (PRIO_MAX-PRIO_MIN+1);

	/* Neither of these should ever happen. */
	if ((signed) *new_q < MAX_USER_Q) *new_q = MAX_USER_Q;
	if (*new_q > MIN_USER_Q) *new_q = MIN_USER_Q;

	return (OK);
}

/*===========================================================================*
 *				pm_isokendpt			 	     *
 *===========================================================================*/
int pm_isokendpt(int endpoint, int *proc)
{
	*proc = _ENDPOINT_P(endpoint);
	if (*proc < 0 || *proc >= NR_PROCS)
		return EINVAL;
	if (endpoint != mproc[*proc].mp_endpoint)
		return EDEADEPT;
	if (!(mproc[*proc].mp_flags & IN_USE))
		return EDEADEPT;
	return OK;
}

/*===========================================================================*
 *				tell_vfs			 	     *
 *===========================================================================*/
void tell_vfs(rmp, m_ptr)
struct mproc *rmp;
message *m_ptr;
{
/* Send a request to VFS, without blocking.
 */
  int r;

  if (rmp->mp_flags & VFS_CALL)
	panic("tell_vfs: not idle: %d", m_ptr->m_type);

  r = asynsend3(VFS_PROC_NR, m_ptr, AMF_NOREPLY);
  if (r != OK)
  	panic("unable to send to VFS: %d", r);

  rmp->mp_flags |= VFS_CALL;
}


/***************************************
 * CS 551 Additions
 ***************************************/


/*
 * Function: do_nicer_to
 *
 * Abstract: Performs PM-server-level tasks for the nicerTo() system call.
 *
 * Input: Message of type mess_lc_pm_nicer_to, with two data types passed:
 *          - int who: the PID of the process to be changed
 *          - int prio: the priority value amount to change
 *
 * Returns: OK on success, or some negative value on failure.
 */
int do_nicer_to()
{
	int retval;
	struct mproc *rmp;

    int curr_nice;
    int new_nice;

    int arg_who;
    int arg_pri;

	arg_who = m_in.m_lc_pm_nicer_to.who;
	arg_pri = m_in.m_lc_pm_nicer_to.prio;

    printf("In PM server do_nicer_to()\n");

	if (arg_who == 0)
    {
		rmp = mp;
    }
	else
    {
		if ((rmp = find_proc(arg_who)) == NULL)
        {
			return(ESRCH);
        }
    }

	if (mp->mp_effuid != SUPER_USER
        && mp->mp_effuid != rmp->mp_effuid
        && mp->mp_effuid != rmp->mp_realuid)
    {
		return EPERM;
    }

	/* Only root is allowed to reduce the nice level. */
	if (arg_pri < 0 && mp->mp_effuid != SUPER_USER)
    {
		return(EACCES);
    }
	
	/* Ensure that the new value is within the allowed priority range. */
    curr_nice = rmp->mp_nice;
    new_nice = curr_nice + arg_pri;
    if (new_nice < PRIO_MIN || new_nice > PRIO_MAX)
    {
		return(EINVAL);
    }

	/* 
	 * The value passed in is currently between PRIO_MIN and PRIO_MAX.
	 * We have to scale this between MIN_USER_Q and MAX_USER_Q to match
	 * the kernel's scheduling queues.
	 */

	if ((retval = sched_nice(rmp, new_nice)) != OK)
    {
		return retval;
	}

	rmp->mp_nice = new_nice;

    printf("Successfully set PID %u's priority from %d to %d\n", arg_who, curr_nice, new_nice);

	return(OK);
}

/***************************************/
