#include "syslib.h"

int sys_trapcount(endpoint_t proc_ep, int *num_traps)
{
	message m;
    int ret;

    printf("In sys_trapcount()\n");
    ret = _kernel_call(SYS_TRAPCOUNT, &m);
    if (ret != OK)
    {
        printf("libsys: Error in _kernel_call(SYS_TRAPCOUNT). Error: %d\n", ret);
        return ret;
    }

    *num_traps = m.m_pm_trapcount.num;
	return(OK);
}
