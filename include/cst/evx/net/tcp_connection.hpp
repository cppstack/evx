#ifndef _CST_EVX_NET_TCP_CONNECTION_HPP
#define _CST_EVX_NET_TCP_CONNECTION_HPP

#include <cst/evx/core/watchers/io_watcher.hpp>

namespace cst {
namespace evx {
namespace net {

class socket;

class tcp_connection;
using tcp_connection_ptr = std::shared_ptr<tcp_connection>;

typedef std::function<void(const tcp_connection_ptr&)> connect_cb_t;
typedef std::function<void(const tcp_connection_ptr&)> close_cb_t;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    tcp_connection(const tcp_connection&) = delete;
    tcp_connection& operator=(const tcp_connection&) = delete;

    tcp_connection(event_loop& loop, socket&& sock);

    void established();

    void set_connect_callback(const connect_cb_t& cb)
    { connect_cb_ = cb; }

    void set_close_callback(const close_cb_t& cb)
    { close_cb_ = cb; }

private:
    void io_handler_(io_watcher&);

    void handle_close();

    std::unique_ptr<socket> sock_;
    io_watcher iow_;
    connect_cb_t connect_cb_;
    close_cb_t close_cb_;
};

}
}
}

#endif
