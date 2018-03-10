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

    void connect();

    void set_connect_callback(const connect_cb_t& cb)
    { connect_cb_ = cb; }

    ~tcp_client();

private:
    void on_connected_(socket&&);
    void on_closed_();

    event_loop& loop_;
    std::unique_ptr<connector> connector_;
    tcp_connection_ptr connection_;
    connect_cb_t connect_cb_;
    const logger_ptr& logger_;
};

}
}
}

#endif
