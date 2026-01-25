#include <frixia/frixia_tcp.h>
#include <frixia/frixia_udp.h>
#include <frixia/frixia_fifo.h>
#include <frixia/frixia_timer.h>
#include <frixia/frixia_signal.h>
#include <frixia/frixia_inode.h>
#include <frixia/frixia_eventfd.h>

#include <internal/internal_frixia.h>

bool INTERNAL_FRIXIA_TCP_CODE_IS_OK(FTCP_CODE t)
{
    return ftcp_code_is_ok(t);
}
bool INTERNAL_FRIXIA_UDP_CODE_IS_OK(FUDP_CODE t)
{
    return fudp_code_is_ok(t);
}
bool INTERNAL_FRIXIA_FIFO_CODE_IS_OK(FFIFO_CODE t)
{
    return ffifo_code_is_ok(t);
}
bool INTERNAL_FRIXIA_TIMER_CODE_IS_OK(FTIMER_CODE t)
{
    return ftimer_code_is_ok(t);
}
bool INTERNAL_FRIXIA_SIGNAL_CODE_IS_OK(FSIGNAL_CODE t)
{
    return fsignal_code_is_ok(t);
}
bool INTERNAL_FRIXIA_INODE_CODE_IS_OK(FINODE_CODE t)
{
    return finode_code_is_ok(t);
}
bool INTERNAL_FRIXIA_EVENTFD_CODE_IS_OK(FEVENTFD_CODE t)
{
    return feventfd_code_is_ok(t);
}

FRIXIA_RESULT INTERNAL_FRIXIA_TIMER_FD_RESULT(FRIXIA_TIMER_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_TIMER_CODE_IS_OK(res.code),
        .errno_code = res.errno_code,
        .result = res.code
    };

    return retVal;
}