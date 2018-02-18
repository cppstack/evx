#include <cst/evx/core/event_loop.hpp>
#include "evx/core/poller.hpp"
#include "evx/core/pollers/poll_poller.hpp"
#include "evx/core/pollers/epoll_poller.hpp"
#include "evx/config.h"

namespace cst {
namespace evx {

poller::poller(event_loop& loop) noexcept
    : loop_(loop), logger_(loop.logger())
{ }

std::unique_ptr<poller> poller::new_poller(event_loop& loop)
{
#if USE_EPOLL
    return std::make_unique<epoll_poller>(loop);
#else
    return std::make_unique<poll_poller>(loop);
#endif
}

}
}
