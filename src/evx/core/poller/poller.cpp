#include "evx/core/poller/poller.hpp"
#include "evx/core/poller/poll_poller.hpp"
#include <poll.h>

namespace cst {
namespace evx {

std::unique_ptr<poller> poller::new_poller(event_loop& loop)
{
    return std::make_unique<poll_poller>(loop);
}

}
}
