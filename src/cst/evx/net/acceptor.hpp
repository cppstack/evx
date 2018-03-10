#ifndef _CST_EVX_NET_ACCEPTOR_HPP
#define _CST_EVX_NET_ACCEPTOR_HPP

#include <cst/evx/core/watchers/io_watcher.hpp>
#include "net/socket.hpp"

namespace cst {
namespace evx {
namespace net {

class acceptor {
public:
    acceptor(const acceptor&) = delete;
    acceptor& operator=(const acceptor&) = delete;

    typedef std::function<void(socket&&, const socket_address&)> accept_cb_t;

    acceptor(event_loop& loop, const socket_address& addr, const accept_cb_t& cb)
        : sock_(socket::tcp_bind(addr)),
          iow_(loop, sock_.fd(), ev_none,
               std::bind(&acceptor::io_handler_, this, std::placeholders::_1)),
          accept_cb_(cb)
    { }

    void listen(size_t backlog = SOMAXCONN)
    {
        iow_.enable_read();
        sock_.listen(backlog);
    }

private:
    void io_handler_(io_watcher&);

    socket sock_;
    io_watcher iow_;
    accept_cb_t accept_cb_;
};

}
}
}

#endif
