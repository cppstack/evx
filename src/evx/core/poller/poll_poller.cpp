#include "evx/core/poller/poll_poller.hpp"
#include "evx/core/system_error.hpp"
#include <poll.h>

namespace cst {
namespace evx {

void poll_poller::poll(int timeout)
{
    int r = ::poll(pollfds_.data(), pollfds_.size(), timeout);
    if (r == -1)
        throw system_error(errno, logger());
}

}
}
