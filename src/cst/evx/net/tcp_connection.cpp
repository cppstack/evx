#include <cst/evx/net/tcp_connection.hpp>
#include "net/socket.hpp"

namespace cst {
namespace evx {
namespace net {

tcp_connection::tcp_connection(event_loop& loop, socket&& sock)
    : sock_(std::make_unique<socket>(std::move(sock))),
      iow_(loop, sock_->fd(), ev_in,
           std::bind(&tcp_connection::io_handler_, this, std::placeholders::_1))
{ }

void tcp_connection::established()
{
    if (connect_cb_)
        connect_cb_(shared_from_this());
}

void tcp_connection::io_handler_(io_watcher& /*iow*/)
{
    /* TODO: check iow events, error handling */
    handle_close();
}

void tcp_connection::handle_close()
{
    /* WARNING:
     * The destructor of this object is invoked in the close_cb_ (maybe should
     * renamed to destroy_cb_).
     */
    close_cb_(shared_from_this());
}

}
}
}
