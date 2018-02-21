#include <cst/evx/net/tcp_server.hpp>
#include "net/socket.hpp"
#include "net/acceptor.hpp"

namespace cst {
namespace evx {
namespace net {

tcp_server::tcp_server(event_loop& loop, const socket_address& addr)
    : loop_(loop), acceptor_(std::make_unique<acceptor>(loop, addr,
        std::bind(&tcp_server::on_accept_, this,
                  std::placeholders::_1, std::placeholders::_2)))
{ }

}
}
}
