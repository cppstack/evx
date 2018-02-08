#include <cst/evx/core/event_loop.hpp>
#include "evx/core/poller.hpp"
#include "evx/core/pollers/poll_poller.hpp"
#include <poll.h>

namespace cst {
namespace evx {

std::unique_ptr<poller> poller::new_poller(event_loop& loop)
{
    return std::make_unique<poll_poller>(loop);
}

const logger_ptr& poller::logger() const noexcept
{
    return loop_.logger();
}

}
}
