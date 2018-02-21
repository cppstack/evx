#ifndef _CST_EVX_NET_SSL_CTX_HPP
#define _CST_EVX_NET_SSL_CTX_HPP

#include "os/openssl.hpp"
#include <memory>

namespace cst {
namespace evx {
namespace net {

using evx::os::SSL_CTX;

class ssl_ctx {
public:
    ssl_ctx(const ssl_ctx&) = delete;
    ssl_ctx& operator=(const ssl_ctx&) = delete;

    static ssl_ctx& get()
    {
        static ssl_ctx ctx;
        return ctx;
    }

    operator SSL_CTX*() const noexcept { return ctx_.get(); }

    ~ssl_ctx() { os::SSL_Cleanups(); }

private:
    ssl_ctx()
    {
        using namespace evx::os;

        ::SSL_library_init();
        ::SSL_load_error_strings();

        ctx_.reset(SSL_CTX_New(SSLv23_Client_method()));

        SSL_CTX_Set_default_verify_paths(ctx_.get());
        ::SSL_CTX_set_verify(ctx_.get(), SSL_VERIFY_NONE, nullptr);

        SSL_CTX_set_mode(ctx_.get(), SSL_MODE_ENABLE_PARTIAL_WRITE);
        SSL_CTX_set_mode(ctx_.get(), SSL_MODE_AUTO_RETRY);
    }

    std::unique_ptr<SSL_CTX, void(*)(SSL_CTX*)> ctx_{nullptr, ::SSL_CTX_free};
};

}
}
}

#endif
