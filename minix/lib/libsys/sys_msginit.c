#include "syslib.h"

int sys_msginit(endpoint_t proc_ep, int *num_msgs)
{
	message m;
    int ret;

    printf("In sys_msginit()\n");
    ret = _kernel_call(SYS_MSGINIT, &m);
    if (ret != OK)
    {
        printf("libsys: Error in _kernel_call(SYS_MSGINIT). Error: %d\n", ret);
        return ret;
    }

    *num_msgs = m.m_pm_msgcount.num;
	return(OK);
}
