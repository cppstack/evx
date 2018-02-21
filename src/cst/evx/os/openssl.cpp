#include "core/errors.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdexcept>

namespace cst {
namespace evx {
namespace os {

using SSL = ::SSL;

size_t SSL_Read(SSL* ssl, void* buf, size_t num)
{
    int ret = ::SSL_read(ssl, buf, num);
    if (ret < 0)
        throw_runtime_error("SSL_read() error");

    return ret;
}

size_t SSL_Readn(SSL* ssl, void* buf, size_t num)
{
    char* ptr = (char*) buf;
    size_t nleft = num;
    size_t nread = 0;

    while (nleft > 0) {
        size_t n = SSL_Read(ssl, ptr, nleft);
        if (n == 0)
            break;

        nleft -= n;
        ptr += n;
        nread += n;
    }

    return nread;
}

size_t SSL_Write(SSL* ssl, const void* buf, size_t num)
{
    int ret = ::SSL_write(ssl, buf, num);
    if (ret < 0)
        throw_runtime_error("SSL_write() error");

    return ret;
}

size_t SSL_Writen(SSL* ssl, const void* buf, size_t num)
{
    const char* ptr = (const char*) buf;
    size_t nleft = num;
    size_t nwritten = 0;

    while (nleft > 0) {
        size_t n = SSL_Write(ssl, ptr, nleft);
        if (n == 0)
            break;

        nleft -= n;
        ptr += n;
        nwritten += n;
    }

    return nwritten;
}

}
}
}
