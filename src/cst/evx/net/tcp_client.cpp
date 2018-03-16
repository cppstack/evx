#include <cst/evx/net/tcp_client.hpp>
#include <cst/logging/logger.hpp>
#include "net/connector.hpp"

namespace cst {
namespace evx {
namespace net {

tcp_client::tcp_client(event_loop& loop, const inet_address& addr)
    : loop_(loop),
      connector_(std::make_unique<connector>(loop, addr,
          std::bind(&tcp_client::new_connection_, this, std::placeholders::_1))),
      logger_(loop.logger())
{ }

void tcp_client::connect()
{
    connector_->start();
}

void tcp_client::disconnect()
{
    if (connection_)
        connection_->shutdown();
    else
        connector_->cancel();
}

void tcp_client::new_connection_(socket&& sock)
{
    connection_ = std::make_shared<tcp_connection>(loop_, std::move(sock));

    connection_->set_connect_callback(connect_cb_);
    connection_->set_read_callback(read_cb_);
    connection_->set_write_callback(write_cb_);
    connection_->set_close_callback(
        std::bind(&tcp_client::end_connection_, this, std::placeholders::_1));

    LOG_INFO(logger_) << "tcp connection established, remote address: "
                      << connector_->remote_address_str();

    connection_->established();
}

void tcp_client::end_connection_(const tcp_connection_ptr& /*conn*/)
{
    connection_->destroy();

    connection_.reset();

    LOG_INFO(logger_) << "tcp connection closed";
}

tcp_client::~tcp_client() = default;

}
}
}
