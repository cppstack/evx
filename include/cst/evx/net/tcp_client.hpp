#ifndef _CST_EVX_NET_TCP_CLIENT_HPP
#define _CST_EVX_NET_TCP_CLIENT_HPP

#include <cst/evx/core/event_loop.hpp>
#include <cst/evx/net/socket_address.hpp>
#include <cst/evx/net/tcp_connection.hpp>

namespace cst {
namespace evx {
namespace net {

class socket;
class connector;

class tcp_client {
public:
    tcp_client(const tcp_client&) = delete;
    tcp_client& operator=(const tcp_client&) = delete;

    tcp_client(event_loop& loop, const socket_address& addr);

    void set_connect_callback(const connect_cb_t& cb)
    { connect_cb_ = cb; }

    void set_read_callback(const read_cb_t& cb)
    { read_cb_ = cb; }

    void set_write_callback(const write_cb_t& cb)
    { write_cb_ = cb; }

    void connect();

    void disconnect();

    ~tcp_client();

private:
    void new_connection_(socket&&);
    void end_connection_(const tcp_connection_ptr&);

    event_loop& loop_;
    std::unique_ptr<connector> connector_;
    tcp_connection_ptr connection_;
    connect_cb_t connect_cb_;
    read_cb_t read_cb_;
    write_cb_t write_cb_;
    const logger_ptr& logger_;
};

}
}
}

#endif
