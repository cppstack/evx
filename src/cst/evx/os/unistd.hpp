#ifndef _CST_EVX_OS_UNISTD_HPP
#define _CST_EVX_OS_UNISTD_HPP

#include "core/errors.hpp"
#include <unistd.h>

namespace cst {
namespace evx {
namespace os {

inline size_t Read(int fd, void* buf, size_t len, const logger_ptr& log = nullptr)
{
    ssize_t n = ::read(fd, buf, len);
    if (n == -1)
        throw_system_error(errno, "read()", log);

    return n;
}

}
}
}

#endif
