#ifndef _CST_EVX_NET_SSL_HPP
#define _CST_EVX_NET_SSL_HPP

#include "net/ssl_ctx.hpp"

namespace cst {
namespace evx {
namespace net {

using evx::os::SSL;

class ssl {
public:
    ssl(const ssl&) = delete;
    ssl& operator=(const ssl&) = delete;

    ssl() = default;

    ssl(ssl&& that) noexcept
        : ssl_(std::move(that.ssl_)),
          state_(std::exchange(that.state_, shutdown_))
    { }

    ssl& operator=(ssl&& that) noexcept
    {
        if (this != &that) {
            shutdown();

            ssl_ = std::move(that.ssl_);
            state_ = std::exchange(that.state_, shutdown_);
        }

        return *this;
    }

    operator SSL*() const noexcept { return ssl_.get(); }

    void connect(int fd)
    {
        using namespace evx::os;

        ssl_.reset(SSL_New(ssl_ctx::get()));
        SSL_Set_fd(ssl_.get(), fd);
        SSL_Connect(ssl_.get());
        state_ = connected_;
    }

    void shutdown() noexcept
    {
        if (state_ == connected_) {
            os::SSL_Shutdown(ssl_.get());
            state_ = shutdown_;
        }
    }

    ~ssl() { shutdown(); }

private:
    enum state { init_, connected_, shutdown_ };

    std::unique_ptr<SSL, void(*)(SSL*)> ssl_{nullptr, ::SSL_free};

    enum state state_ = init_;
};

}
}
}

#endif
