#ifndef _CST_EVX_OS_UNISTD_HPP
#define _CST_EVX_OS_UNISTD_HPP

#include "evx/core/errors.hpp"
#include <unistd.h>

namespace cst {
namespace evx {
namespace os {

inline size_t read(int fd, void* buf, size_t len, const logger_ptr& log)
{
    ssize_t n = ::read(fd, buf, len);
    if (n == -1)
        throw_system_error(errno, log, "read()");

    return n;
}

}
}
}

#endif
