#ifndef _CST_EVX_OS_EPOLL_HPP
#define _CST_EVX_OS_EPOLL_HPP

#include "core/errors.hpp"
#include <sys/epoll.h>

namespace cst {
namespace evx {
namespace os {

inline int Epoll_create1(int flags, const logger_ptr& log = nullptr)
{
    int epfd = ::epoll_create1(flags);
    if (epfd == -1)
        throw_system_error(errno, "epoll_create1()", log);

    return epfd;
}

inline int Epoll_ctl(int epfd, int op, int fd, struct ::epoll_event* event,
                     const logger_ptr& log = nullptr)
{
    if (::epoll_ctl(epfd, op, fd, event) == 0)
        return 0;

    if (errno == EPERM)
        return errno;

    throw_system_error(errno, "epoll_ctl()", log);
}

inline size_t Epoll_wait(int epfd, struct ::epoll_event* events,
                         size_t maxevents, int timeout,
                         const logger_ptr& log = nullptr)
{
    int nr = ::epoll_wait(epfd, events, maxevents, timeout);
    if (nr == -1)
        throw_system_error(errno, "epoll_wait()", log);

    return nr;
}

}
}
}

#endif
