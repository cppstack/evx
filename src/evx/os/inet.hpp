#ifndef _CST_EVX_OS_INET_HPP
#define _CST_EVX_OS_INET_HPP

#include "evx/core/errors.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

namespace cst {
namespace evx {
namespace os {

using addrinfo = struct addrinfo;

inline bool Inet_pton(int af, const char* src, void* dst,
                      const logger_ptr& log = nullptr)
{
    int r = ::inet_pton(af, src, dst);
    if (r == -1)
        throw_system_error(errno, "inet_pton()", log);

    return r;
}

inline const char* Inet_ntop(int af, const void* src, char* dst, size_t size,
                             const logger_ptr& log = nullptr)
{
    const char* ptr = ::inet_ntop(af, src, dst, size);
    if (ptr == nullptr)
        throw_system_error(errno, "inet_ntop()", log);

    return ptr;
}

inline void Getaddrinfo(const char* node, const char* service,
                        const addrinfo* hints, addrinfo** res,
                        const logger_ptr& log = nullptr)
{
    int rc = ::getaddrinfo(node, service, hints, res);
    if (rc != 0) {
        char err[128] = "getaddrinfo(): ";
        std::strncat(err, ::gai_strerror(rc), 100);
        throw_runtime_error(err, log);
    }
}

}
}
}

#endif
