#include "evx/core/poller.hpp"
#include "evx/core/pollers/poll_poller.hpp"

namespace cst {
namespace evx {

std::unique_ptr<poller> poller::new_poller(event_loop& loop)
{
    return std::make_unique<poll_poller>(loop);
}

}
}
