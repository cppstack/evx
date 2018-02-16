#ifndef _CST_EVX_OS_TIME_HPP
#define _CST_EVX_OS_TIME_HPP

#include "evx/core/errors.hpp"
#include <sys/timerfd.h>

namespace cst {
namespace evx {
namespace os {

inline int timerfd_create(int clockid, int flags, const logger_ptr& log)
{
    int fd = ::timerfd_create(clockid, flags);
    if (fd == -1)
        throw_system_error(errno, log, "timerfd_create()");

    return fd;
}

inline void timerfd_settime(int fd, int flags,
                            const struct ::itimerspec* new_value,
                            struct ::itimerspec* old_value,
                            const logger_ptr& log)
{
    if (::timerfd_settime(fd, flags, new_value, old_value) == -1)
        throw_system_error(errno, log, "timerfd_settime()");
}

}
}
}

#endif
