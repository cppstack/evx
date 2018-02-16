#ifndef _CST_EVX_OS_POLL_HPP
#define _CST_EVX_OS_POLL_HPP

#include "evx/core/errors.hpp"
#include <poll.h>

namespace cst {
namespace evx {
namespace os {

inline int poll(struct ::pollfd* fds, ::nfds_t nfds, int timeout, const logger_ptr& log)
{
    int nr = ::poll(fds, nfds, timeout);
    if (nr == -1)
        throw_system_error(errno, log, "poll()");

    return nr;
}

}
}
}

#endif
