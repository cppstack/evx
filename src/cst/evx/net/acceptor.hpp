#ifndef _CST_EVX_NET_ACCEPTOR_HPP
#define _CST_EVX_NET_ACCEPTOR_HPP

#include <cst/evx/core/watchers/io_watcher.hpp>
#include <cst/lnx/socket.hpp>

namespace cst {
namespace evx {
namespace net {

using lnx::socket;
using lnx::inet_address;

class acceptor {
public:
    acceptor(const acceptor&) = delete;
    acceptor& operator=(const acceptor&) = delete;

    typedef std::function<void(socket&&, const inet_address&)> accept_cb_t;

    acceptor(event_loop& loop, const inet_address& addr, const accept_cb_t& cb);

    void listen(size_t backlog = SOMAXCONN);

private:
    void io_handler_(io_watcher&);

    event_loop& loop_;
    socket sock_;
    io_watcher iow_;
    accept_cb_t accept_cb_;
    const logger_ptr& logger_;
};

}
}
}

#endif
