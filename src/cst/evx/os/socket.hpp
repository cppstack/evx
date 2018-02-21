#ifndef _CST_EVX_OS_SOCKET_HPP
#define _CST_EVX_OS_SOCKET_HPP

#include "core/errors.hpp"
#include <sys/socket.h>
#include <cstring>

namespace cst {
namespace evx {
namespace os {

using sockaddr = struct sockaddr;

inline int Socket(int domain, int type, int protocol)
{
    int sockfd = ::socket(domain, type, protocol);
    if (sockfd == -1)
        throw_system_error(errno, "socket()");

    return sockfd;
}

inline void Connect(int sockfd, const sockaddr* addr, socklen_t addrlen)
{
    if (::connect(sockfd, addr, addrlen) == -1)
        throw_system_error(errno, "connect()");
}

inline void Bind(int sockfd, const sockaddr* addr, socklen_t addrlen)
{
    if (::bind(sockfd, addr, addrlen) == -1)
        throw_system_error(errno, "bind()");
}

inline void Listen(int sockfd, int backlog)
{
    if (::listen(sockfd, backlog) == -1)
        throw_system_error(errno, "listen()");
}

inline int Accept(int sockfd, sockaddr* addr, socklen_t* addrlen)
{
    int connfd = ::accept(sockfd, addr, addrlen);
    if (connfd == -1)
        throw_system_error(errno, "accept()");

    return connfd;
}

size_t Recv(int sockfd, void* buf, size_t len, int flags = 0);
size_t Recvn(int sockfd, void* buf, size_t len, int flags = 0);

size_t Send(int sockfd, const void* buf, size_t len, int flags = 0);
size_t Sendn(int sockfd, const void* buf, size_t len, int flags = 0);

inline void Setsockopt(int sockfd, int level, int opt,
                       const void* val, socklen_t len)
{
    if (::setsockopt(sockfd, level, opt, val, len) == -1)
        throw_system_error(errno, "setsockopt()");
}

inline void Getsockopt(int sockfd, int level, int opt,
                       void* val, socklen_t* len)
{
    if (::getsockopt(sockfd, level, opt, val, len) == -1)
        throw_system_error(errno, "getsockopt()");
}

}
}
}

#endif
