#include "evx/core/errors.hpp"
#include <sys/socket.h>
#include <climits>

namespace cst {
namespace evx {
namespace os {

size_t Recv(int sockfd, void* buf, size_t len, int flags)
{
    if (len > SSIZE_MAX)
        len = SSIZE_MAX;

    ssize_t n = ::recv(sockfd, buf, len, flags);
    if (n == -1)
        throw_system_error(errno, "recv()");

    return n;
}

size_t Recvn(int sockfd, void* buf, size_t len, int flags)
{
    char* ptr = (char*) buf;
    size_t nleft = len;
    size_t nrecv = 0;

    while (nleft > 0) {
        size_t n = Recv(sockfd, ptr, nleft, flags);
        if (n == 0)
            break;

        nleft -= n;
        ptr += n;
        nrecv += n;
    }

    return nrecv;
}

size_t Send(int sockfd, const void* buf, size_t len, int flags)
{
    if (len > SSIZE_MAX)
        len = SSIZE_MAX;

    ssize_t n = ::send(sockfd, buf, len, flags);
    if (n == -1)
        throw_system_error(errno, "send()");

    return n;
}

size_t Sendn(int sockfd, const void* buf, size_t len, int flags)
{
    const char* ptr = (const char*) buf;
    size_t nleft = len;
    size_t nsent = 0;

    while (nleft > 0) {
        size_t n = Send(sockfd, ptr, nleft, flags);
        if (n == 0)
            break;

        nleft -= n;
        ptr += n;
        nsent += n;
    }

    return nsent;
}

}
}
}
