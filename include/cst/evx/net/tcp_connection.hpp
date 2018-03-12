#ifndef _CST_EVX_NET_TCP_CONNECTION_HPP
#define _CST_EVX_NET_TCP_CONNECTION_HPP

#include <cst/evx/core/watchers/io_watcher.hpp>
#include <cst/evx/core/buffer.hpp>

namespace cst {
namespace evx {
namespace net {

class socket;

class tcp_connection;
using tcp_connection_ptr = std::shared_ptr<tcp_connection>;

typedef std::function<void(const tcp_connection_ptr&)> connect_cb_t;
typedef std::function<void(const tcp_connection_ptr&, buffer&)> read_cb_t;
typedef std::function<void(const tcp_connection_ptr&)> write_cb_t;
typedef std::function<void(const tcp_connection_ptr&)> close_cb_t;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    tcp_connection(const tcp_connection&) = delete;
    tcp_connection& operator=(const tcp_connection&) = delete;

    tcp_connection(event_loop& loop, socket&& sock);

    void set_connect_callback(const connect_cb_t& cb)
    { connect_cb_ = cb; }

    void set_read_callback(const read_cb_t& cb)
    { read_cb_ = cb; }

    void set_write_callback(const write_cb_t& cb)
    { write_cb_ = cb; }

    void set_close_callback(const close_cb_t& cb)
    { close_cb_ = cb; }

    void established();

    void send(const void* data, std::size_t len);

    void shutdown();

    void destroy();

private:
    enum state {
        disconnected_, connecting_, connected_, disconnecting_
    };

    void io_handler_(io_watcher&);

    void handle_read();
    void handle_write();
    void handle_close();

    void shutdown_();

    state state_;
    std::unique_ptr<socket> sock_;
    io_watcher iow_;
    connect_cb_t connect_cb_;
    read_cb_t read_cb_;
    write_cb_t write_cb_;
    close_cb_t close_cb_;
    buffer inbuf_;
    buffer outbuf_;
};

}
}
}

#endif
