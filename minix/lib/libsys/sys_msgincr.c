#include "syslib.h"

int sys_msgincr(endpoint_t proc_ep, int *num_msgs)
{
	message m;
    int ret;

    printf("In sys_msgincr()\n");
    ret = _kernel_call(SYS_MSGINCR, &m);
    if (ret != OK)
    {
        printf("libsys: Error in _kernel_call(SYS_MSGINCR). Error: %d\n", ret);
        return ret;
    }

    *num_msgs = m.m_pm_msgcount.num;
	return(OK);
}
