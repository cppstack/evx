#include <cst/evx/net/tcp_connection.hpp>
#include <cst/lnx/socket.hpp>

namespace cst {
namespace evx {
namespace net {

tcp_connection::tcp_connection(event_loop& loop, socket&& sock)
    : state_(connecting_),
      sock_(std::make_unique<socket>(std::move(sock))),
      iow_(loop, sock_->fd(), ev_in,
           std::bind(&tcp_connection::io_handler_, this, std::placeholders::_1))
{ }

void tcp_connection::established()
{
    state_ = connected_;

    if (connect_cb_)
        connect_cb_(shared_from_this());
}

void tcp_connection::send(const void* data, std::size_t len)
{
    std::size_t nsent = 0;
    std::size_t rem = len;

    if (outbuf_.readable() == 0) {
        nsent = sock_->send(data, len);
        rem -= nsent;
    }

    if (rem == 0) {
        if (write_cb_)
            write_cb_(shared_from_this());
        return;
    }

    outbuf_.append((const char*) data + nsent, rem);
    if (!iow_.is_writing())
        iow_.enable_write();
}

void tcp_connection::shutdown()
{
    if (state_ == connected_) {
        state_ = disconnecting_;
        shutdown_();
    }
}

void tcp_connection::destroy()
{
    state_ = disconnected_;

    iow_.disable_rdwr();
}

void tcp_connection::io_handler_(io_watcher& iow)
{
    /* TODO: check iow events, error handling */
    if (iow.revents() & ev_in)
        handle_read();

    if (iow.revents() & ev_out)
        handle_write();

    if (iow.revents() & ev_hup)
        handle_close();
}

void tcp_connection::handle_read()
{
    std::size_t nr = inbuf_.read_fd(iow_.fd());
    if (nr) {
        if (read_cb_)
            read_cb_(shared_from_this(), inbuf_);
    } else
        handle_close();
}

void tcp_connection::handle_write()
{
    if (iow_.is_writing()) {
        /* still interested in write */
        std::size_t n = sock_->send(outbuf_.rbegin(), outbuf_.readable());
        outbuf_.take(n);

        if (outbuf_.readable() == 0) {
            iow_.disable_write();

            if (write_cb_)
                write_cb_(shared_from_this());

            if (state_ == disconnecting_)
                shutdown_();
        }
    }
}

void tcp_connection::handle_close()
{
    state_ = disconnected_;
    iow_.disable_rdwr();

    /* WARNING:
     * The destructor of this object is invoked in the close_cb_ (maybe should
     * renamed to destroy_cb_).
     */
    close_cb_(shared_from_this());
}

void tcp_connection::shutdown_()
{
    if (!iow_.is_writing())
        sock_->shutdown(SHUT_WR);
}

}
}
}
