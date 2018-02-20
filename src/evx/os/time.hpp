#ifndef _CST_EVX_OS_TIME_HPP
#define _CST_EVX_OS_TIME_HPP

#include "evx/core/errors.hpp"
#include <sys/timerfd.h>

namespace cst {
namespace evx {
namespace os {

inline int Timerfd_create(int clockid, int flags,
                          const logger_ptr& log = nullptr)
{
    int fd = ::timerfd_create(clockid, flags);
    if (fd == -1)
        throw_system_error(errno, "timerfd_create()", log);

    return fd;
}

inline void Timerfd_settime(int fd, int flags,
                            const struct ::itimerspec* new_value,
                            struct ::itimerspec* old_value,
                            const logger_ptr& log = nullptr)
{
    if (::timerfd_settime(fd, flags, new_value, old_value) == -1)
        throw_system_error(errno, "timerfd_settime()", log);
}

}
}
}

#endif
