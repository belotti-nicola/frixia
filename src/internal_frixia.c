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

FRIXIA_ADD_RESULT INTERNAL_TCP_CODE_TO_ADD_RESULT(FTCP_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FTCP_SENTINEL__ + code);
}

FRIXIA_ADD_RESULT INTERNAL_UDP_CODE_TO_ADD_RESULT(FUDP_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FUDP_SENTINEL__ + code);
}

FRIXIA_ADD_RESULT INTERNAL_FIFO_CODE_TO_ADD_RESULT(FFIFO_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FFIFO_SENTINEL__ + code);
}

FRIXIA_ADD_RESULT INTERNAL_TIMER_CODE_TO_ADD_RESULT(FTIMER_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FTIMER_SENTINEL__ + code);
}

FRIXIA_ADD_RESULT INTERNAL_INODE_CODE_TO_ADD_RESULT(FINODE_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FINODE_SENTINEL__ + code);
}

FRIXIA_ADD_RESULT INTERNAL_SIGNAL_CODE_TO_ADD_RESULT(FSIGNAL_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FSIGNAL_SENTINEL__ + code);
}

FRIXIA_ADD_RESULT INTERNAL_EVENTFD_CODE_TO_ADD_RESULT(FEVENTFD_CODE code)
{
    return (FRIXIA_ADD_RESULT)(__FEVENTFD_SENTINEL__ + code);
}

FRIXIA_RESULT INTERNAL_FRIXIA_TCP_FD_RESULT(FRIXIA_TCP_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_TCP_CODE_IS_OK(res.res.errno_code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_TCP_CODE_TO_ADD_RESULT(res.res.exit_code)
    };

    return retVal;
}

FRIXIA_RESULT INTERNAL_FRIXIA_UDP_FD_RESULT(FRIXIA_UDP_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_UDP_CODE_IS_OK(res.res.errno_code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_UDP_CODE_TO_ADD_RESULT(res.res.exit_code)
    };

    return retVal;
}
FRIXIA_RESULT INTERNAL_FRIXIA_FIFO_FD_RESULT(FRIXIA_FIFO_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_FIFO_CODE_IS_OK(res.res.code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_FIFO_CODE_TO_ADD_RESULT(res.res.code)
    };

    return retVal;
}
FRIXIA_RESULT INTERNAL_FRIXIA_TIMER_FD_RESULT(FRIXIA_TIMER_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_TIMER_CODE_IS_OK(res.res.code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_TIMER_CODE_TO_ADD_RESULT(res.res.code)
    };

    return retVal;
}
FRIXIA_RESULT INTERNAL_FRIXIA_SIGNAL_FD_RESULT(FRIXIA_SIGNAL_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_SIGNAL_CODE_IS_OK(res.res.code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_SIGNAL_CODE_TO_ADD_RESULT(res.res.code)
    };

    return retVal;
}
FRIXIA_RESULT INTERNAL_FRIXIA_INODE_FD_RESULT(FRIXIA_INODE_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_INODE_CODE_IS_OK(res.res.code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_INODE_CODE_TO_ADD_RESULT(res.res.code)
    };

    return retVal;
}
FRIXIA_RESULT INTERNAL_FRIXIA_EVENTFD_FD_RESULT(FRIXIA_EVENTFD_FD_RESULT res)
{
    FRIXIA_RESULT retVal = 
    {
        .fd = res.fd,
        .kind = INTERNAL_FRIXIA_EVENTFD_CODE_IS_OK(res.res.code)? FRIXIA_OK : FRIXIA_KO,
        .errno_code = res.res.errno_code,
        .result = INTERNAL_EVENTFD_CODE_TO_ADD_RESULT(res.res.code)
    };

    return retVal;
}