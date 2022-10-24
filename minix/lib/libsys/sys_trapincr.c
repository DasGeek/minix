#include "syslib.h"

int sys_trapincr(endpoint_t proc_ep, int *num_traps)
{
	message m;
    int ret;

    printf("In sys_trapincr()\n");
    ret = _kernel_call(SYS_TRAPINCR, &m);
    if (ret != OK)
    {
        printf("libsys: Error in _kernel_call(SYS_TRAPINCR). Error: %d\n", ret);
        return ret;
    }

    *num_traps = m.m_pm_trapcount.num;
	return(OK);
}
