#ifndef _CST_EVX_OS_UIO_HPP
#define _CST_EVX_OS_UIO_HPP

#include "core/errors.hpp"
#include <sys/uio.h>

namespace cst {
namespace evx {
namespace os {

inline size_t Readv(int fd, struct ::iovec* iov, size_t iovcnt,
                    const logger_ptr& log = nullptr)
{
    ssize_t n = ::readv(fd, iov, iovcnt);
    if (n == -1)
        throw_system_error(errno, "readv()", log);

    return n;
}

}
}
}

#endif
