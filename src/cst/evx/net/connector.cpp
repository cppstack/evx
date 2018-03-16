#include "net/connector.hpp"
#include <cst/evx/core/event_loop.hpp>
#include <cst/logging/logger.hpp>
#include <sstream>

namespace cst {
namespace evx {
namespace net {

connector::connector(event_loop& loop, const inet_address& addr,
                     const connect_cb_t& cb)
    : sock_(addr.family(), SOCK_STREAM | SOCK_NONBLOCK, 0),
      addr_(addr),
      iow_(loop, sock_.fd(), ev_none,
           std::bind(&connector::io_handler_, this, std::placeholders::_1)),
      connect_cb_(cb),
      logger_(loop.logger())
{ }

void connector::start()
{
    sock_.throw_exception(false);

    iow_.enable_rdwr();

    if (!host_.empty())
        ;
    else
        sock_.connect(addr_);
}

void connector::cancel()
{
    iow_.disable_rdwr();
    sock_.close();
}

std::string connector::remote_address_str() const
{
    std::ostringstream oss;

    if (!host_.empty())
        oss << host_ << ':' << port_;
    else
        oss << addr_;

    return oss.str();
}

void connector::io_handler_(io_watcher& iow)
{
    iow.disable_rdwr();

    if (iow.revents() & ev_err) {
        LOG_ERROR(logger_) << "Failed to connect to " << remote_address_str();
        sock_.close();
        return;
    }

    connect_cb_(std::move(sock_));
}

}
}
}
