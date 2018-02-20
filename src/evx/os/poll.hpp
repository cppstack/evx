#ifndef _CST_EVX_OS_POLL_HPP
#define _CST_EVX_OS_POLL_HPP

#include "evx/core/errors.hpp"
#include <poll.h>

namespace cst {
namespace evx {
namespace os {

inline size_t Poll(struct ::pollfd* fds, size_t nfds, int timeout,
                   const logger_ptr& log = nullptr)
{
    int nr = ::poll(fds, nfds, timeout);
    if (nr == -1)
        throw_system_error(errno, "poll()", log);

    return nr;
}

}
}
}

#endif
