#ifndef _CST_EVX_NET_TCP_SERVER_HPP
#define _CST_EVX_NET_TCP_SERVER_HPP

#include <cst/evx/core/event_loop.hpp>
#include <cst/evx/net/socket_address.hpp>

namespace cst {
namespace evx {
namespace net {

class socket;
class acceptor;

class tcp_server {
public:
    tcp_server(event_loop& loop, const socket_address& addr);

private:
    void on_accept_(socket&&, const socket_address&);

    event_loop& loop_;
    std::unique_ptr<acceptor> acceptor_;
};

}
}
}

#endif
