#include "syslib.h"

int sys_msgcount(endpoint_t proc_ep, int *num_msgs)
{
	message m;
    int ret;

    ret = _kernel_call(SYS_MSGCOUNT, &m);
    if (ret != OK)
    {
        printf("libsys: Error in _kernel_call(SYS_MSGCOUNT). Error: %d\n", ret);
        return ret;
    }

    *num_msgs = m.m_pm_msgcount.num;
	return(OK);
}
