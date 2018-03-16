#ifndef _CST_EVX_NET_CONNECTOR_HPP
#define _CST_EVX_NET_CONNECTOR_HPP

#include <cst/evx/core/watchers/io_watcher.hpp>
#include <cst/lnx/socket.hpp>

namespace cst {
namespace evx {
namespace net {

using lnx::socket;
using lnx::inet_address;

class connector {
public:
    connector(const connector&) = delete;
    connector& operator=(const connector&) = delete;

    typedef std::function<void(socket&&)> connect_cb_t;

    connector(event_loop& loop, const inet_address& addr, const connect_cb_t& cb);

    void start();
    void cancel();

    std::string remote_address_str() const;

private:
    void io_handler_(io_watcher&);

    socket sock_;

    std::string host_;
    std::string port_;
    inet_address addr_;

    io_watcher iow_;
    connect_cb_t connect_cb_;

    const logger_ptr& logger_;
};

}
}
}

#endif
