#include "net/acceptor.hpp"
#include <cst/evx/core/event_loop.hpp>
#include <cst/logging/logger.hpp>

namespace cst {
namespace evx {
namespace net {

acceptor::acceptor(event_loop& loop, const inet_address& addr, const accept_cb_t& cb)
    : loop_(loop),
      sock_(socket::tcp_bind(addr, SOCK_NONBLOCK)),
      iow_(loop, sock_.fd(), ev_none,
           std::bind(&acceptor::io_handler_, this, std::placeholders::_1)),
      accept_cb_(cb),
      logger_(loop.logger())
{
    sock_.throw_exception(false);
}

void acceptor::listen(size_t backlog)
{
    loop_.assert_in_loop_thread();
    sock_.listen(backlog);
    iow_.enable_read();
}

void acceptor::io_handler_(io_watcher& iow)
{
    if (iow.revents() & evx::ev_err) {
        LOG_CRIT(logger_) << "acceptor got ev_err";
        iow.disable_read();
        return;
    }

    socket nsk;
    inet_address peer;
    do {
        if (sock_.accept(&nsk, &peer) == -1) {
            auto& ec = sock_.error();
            if (ec.value() != EAGAIN && ec.value() != EWOULDBLOCK)
                LOG_CRIT(logger_) << "socket accept() error: " << ec.message();
            break;
        }
        accept_cb_(std::move(nsk), peer);
    } while (true); /* false if want to accept only one */
}

}
}
}
