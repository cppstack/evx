#include <cst/evx/net/tcp_client.hpp>
#include "net/connector.hpp"

namespace cst {
namespace evx {
namespace net {

tcp_client::tcp_client(event_loop& loop, const socket_address& addr)
    : loop_(loop),
      connector_(std::make_unique<connector>(loop, addr,
          std::bind(&tcp_client::on_connected_, this, std::placeholders::_1))),
      logger_(loop.logger())
{
    connector_->connect();
}

void tcp_client::on_connected_(socket&& sock)
{
    connection_ = std::make_shared<tcp_connection>(loop_, std::move(sock));

    connection_->set_close_callback(std::bind(&tcp_client::on_closed_, this));

    LOG_INFO(logger_) << "tcp connection established, remote address: "
                      << connector_->remote_address_str();
}

void tcp_client::on_closed_()
{
    connection_.reset();

    LOG_INFO(logger_) << "tcp connection closed";
}

tcp_client::~tcp_client() = default;

}
}
}
