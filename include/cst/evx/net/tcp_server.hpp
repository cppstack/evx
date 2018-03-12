#ifndef _CST_EVX_NET_TCP_SERVER_HPP
#define _CST_EVX_NET_TCP_SERVER_HPP

#include <cst/evx/core/event_loop.hpp>
#include <cst/evx/net/socket_address.hpp>
#include <cst/evx/net/tcp_connection.hpp>
#include <set>

namespace cst {
namespace evx {
namespace net {

class socket;
class acceptor;

class tcp_server {
public:
    tcp_server(const tcp_server&) = delete;
    tcp_server& operator=(const tcp_server&) = delete;

    tcp_server(event_loop& loop, const socket_address& addr);

    void start();

    void set_connect_callback(const connect_cb_t& cb)
    { connect_cb_ = cb; }

    void set_read_callback(const read_cb_t& cb)
    { read_cb_ = cb; }

    void set_write_callback(const write_cb_t& cb)
    { write_cb_ = cb; }

    ~tcp_server();

private:
    void new_connection_(socket&&, const socket_address&);
    void end_connection_(const tcp_connection_ptr&);

    event_loop& loop_;
    std::unique_ptr<acceptor> acceptor_;
    std::set<tcp_connection_ptr> connections_;
    connect_cb_t connect_cb_;
    read_cb_t read_cb_;
    write_cb_t write_cb_;
    const logger_ptr& logger_;
};

}
}
}

#endif
