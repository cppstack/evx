#include <cst/evx/net/tcp_connection.hpp>
#include <cst/logging/logger.hpp>
#include <cst/lnx/socket.hpp>

namespace cst {
namespace evx {
namespace net {

tcp_connection::tcp_connection(event_loop& loop, socket&& sock)
    : state_(connecting_),
      sock_(std::make_unique<socket>(std::move(sock))),
      iow_(loop, sock_->fd(), ev_in,
          std::bind(&tcp_connection::io_handler_, this, std::placeholders::_1)),
      logger_(loop.logger())
{
    sock_->throw_exception(false);
}

void tcp_connection::established()
{
    state_ = connected_;

    if (connect_cb_)
        connect_cb_(shared_from_this());
}

void tcp_connection::send(const void* data, size_t len)
{
    if (state_ != connected_) {
        LOG_ERROR(logger_) << *this << ", unconnected.";
        return;
    }

    ssize_t nsent = 0;
    size_t rem = len;

    if (outbuf_.readable() == 0) {
        nsent = sock_->send(data, len);
        if (nsent == -1) {
            LOG_ERROR(logger_) << *this << ", SO_ERROR = " << sock_->error();
            return;
        }

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

    if (iow.revents() & ev_err)
        handle_error();
}

void tcp_connection::handle_read()
{
    std::error_code ec;
    ssize_t n = inbuf_.read_fd(iow_.fd(), &ec);
    if (n > 0) {
        if (read_cb_)
            read_cb_(shared_from_this(), inbuf_);
    } else if (n == 0)
        handle_close();
    else
        handle_error(ec);
}

void tcp_connection::handle_write()
{
    if (iow_.is_writing()) {
        /* still interested in write */
        ssize_t n = sock_->send(outbuf_.rbegin(), outbuf_.readable());
        if (n == -1) {
            handle_error();
            return;
        }

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

void tcp_connection::handle_error()
{
    LOG_ERROR(logger_) << *this << ", " << sock_->error().message();
}

void tcp_connection::handle_error(const std::error_code& ec)
{
    LOG_ERROR(logger_) << *this << ", " << ec.message();
}

void tcp_connection::shutdown_()
{
    if (!iow_.is_writing())
        sock_->shutdown(SHUT_WR);
}

std::ostream& operator<<(std::ostream& os, const tcp_connection& c)
{
    return os << "tcp_connection[fd=" << c.sock_->fd()
              << ", state=" << c.state_ << "]";
}

}
}
}
