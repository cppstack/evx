#include <cst/evx/net/tcp_server.hpp>
#include "net/socket.hpp"
#include "net/acceptor.hpp"

namespace cst {
namespace evx {
namespace net {

tcp_server::tcp_server(event_loop& loop, const socket_address& addr)
    : loop_(loop),
      acceptor_(std::make_unique<acceptor>(loop, addr,
          std::bind(&tcp_server::new_connection_, this,
                    std::placeholders::_1, std::placeholders::_2))),
      logger_(loop.logger())
{ }

void tcp_server::start()
{
    acceptor_->listen();
}

void tcp_server::new_connection_(socket&& sock, const socket_address& peer)
{
    tcp_connection_ptr conn =
        std::make_shared<tcp_connection>(loop_, std::move(sock));

    LOG_INFO(logger_) << "new tcp connection, remote address: " << peer;

    conn->set_connect_callback(connect_cb_);
    conn->set_close_callback(
          std::bind(&tcp_server::end_connection_, this, std::placeholders::_1));

    conn->established();

    connections_.insert(conn);
}

void tcp_server::end_connection_(const tcp_connection_ptr& conn)
{
    LOG_INFO(logger_) << "end tcp connection";

    connections_.erase(conn);
}

tcp_server::~tcp_server() = default;

}
}
}
