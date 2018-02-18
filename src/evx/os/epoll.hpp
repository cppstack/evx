#ifndef _CST_EVX_OS_EPOLL_HPP
#define _CST_EVX_OS_EPOLL_HPP

#include "evx/core/errors.hpp"
#include <sys/epoll.h>

namespace cst {
namespace evx {
namespace os {

inline int epoll_create1(int flags, const logger_ptr& log)
{
    int epfd = ::epoll_create1(flags);
    if (epfd == -1)
        throw_system_error(errno, log, "epoll_create1()");

    return epfd;
}

inline int epoll_ctl(int epfd, int op, int fd, struct ::epoll_event* event, const logger_ptr& log)
{
    if (::epoll_ctl(epfd, op, fd, event) == 0)
        return 0;

    if (errno == EPERM)
        return errno;

    throw_system_error(errno, log, "epoll_ctl()");
}

inline size_t epoll_wait(int epfd, struct ::epoll_event* events, size_t maxevents, int timeout, const logger_ptr& log)
{
    int nr = ::epoll_wait(epfd, events, maxevents, timeout);
    if (nr == -1)
        throw_system_error(errno, log, "epoll_wait()");

    return nr;
}

}
}
}

#endif
