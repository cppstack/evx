#include "net/acceptor.hpp"

namespace cst {
namespace evx {
namespace net {

void acceptor::io_handler_(io_watcher& /*iow*/)
{
    /* TODO: check iow events, error handling in accept */
    socket_address peer;
    accept_cb_(sock_.accept(peer), peer);
}

}
}
}
