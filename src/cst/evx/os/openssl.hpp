#ifndef _CST_EVX_OS_OPENSSL_HPP
#define _CST_EVX_OS_OPENSSL_HPP

#include "core/errors.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdexcept>

namespace cst {
namespace evx {
namespace os {

using SSL_METHOD = ::SSL_METHOD;
using SSL_CTX = ::SSL_CTX;
using SSL = ::SSL;

using X509 = ::X509;

inline void SSL_Cleanups() noexcept
{
    ::ERR_free_strings();
    ::ERR_remove_thread_state(NULL);
    ::CRYPTO_cleanup_all_ex_data();
    ::SSL_COMP_free_compression_methods();
    ::EVP_cleanup();
}

inline const SSL_METHOD* SSLv23_Client_method()
{
    const SSL_METHOD* method = ::SSLv23_client_method();
    if (method == nullptr)
        throw_runtime_error("SSLv23_client_method() error");

    return method;
}

inline SSL_CTX* SSL_CTX_New(const SSL_METHOD* method)
{
    SSL_CTX* ctx = ::SSL_CTX_new(method);
    if (ctx == nullptr)
        throw_runtime_error("SSL_CTX_new() error");

    return ctx;
}

inline void SSL_CTX_Set_default_verify_paths(SSL_CTX* ctx)
{
    if (::SSL_CTX_set_default_verify_paths(ctx) == 0)
        throw_runtime_error("SSL_CTX_set_default_verify_paths() error");
}

inline SSL* SSL_New(SSL_CTX* ctx)
{
    SSL* ssl = ::SSL_new(ctx);
    if (ssl == nullptr)
        throw_runtime_error("SSL_new() error");

    return ssl;
}

inline int SSL_Shutdown(SSL* ssl) noexcept
{
    int ret = ::SSL_shutdown(ssl);
    if (ret == 0)  /* try again */
        ret = ::SSL_shutdown(ssl);
    return ret;
}

inline void SSL_Set_fd(SSL* ssl, int fd)
{
    if (::SSL_set_fd(ssl, fd) == 0)
        throw_runtime_error("SSL_set_fd() error");
}

inline void SSL_Connect(SSL* ssl)
{
    if (::SSL_connect(ssl) <= 0)
        throw_runtime_error("SSL_connect() error");
}

inline size_t SSL_Peek(SSL* ssl, void* buf, size_t num)
{
    int ret = ::SSL_peek(ssl, buf, num);
    if (ret < 0)
        throw_runtime_error("SSL_peek() error");

    return ret;
}

size_t SSL_Read(SSL* ssl, void* buf, size_t num);
size_t SSL_Readn(SSL* ssl, void* buf, size_t num);

size_t SSL_Write(SSL* ssl, const void* buf, size_t num);
size_t SSL_Writen(SSL* ssl, const void* buf, size_t num);

}
}
}

#endif
