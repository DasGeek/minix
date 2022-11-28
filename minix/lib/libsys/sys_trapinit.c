#include "syslib.h"

int sys_trapinit(endpoint_t proc_ep, int *num_traps)
{
	message m;
    int ret;

    printf("In sys_trapinit()\n");
    ret = _kernel_call(SYS_TRAPINIT, &m);
    if (ret != OK)
    {
        printf("libsys: Error in _kernel_call(SYS_TRAPINIT). Error: %d\n", ret);
        return ret;
    }

    *num_traps = m.m_pm_trapcount.num;
	return(OK);
}
