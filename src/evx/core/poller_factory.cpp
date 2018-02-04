#include "core/poller_factory.hpp"
#include "core/poll_poller.hpp"

namespace cst {
namespace evx {

std::unique_ptr<poller> poller_factory::new_poller(event_loop& loop)
{
    return std::make_unique<poll_poller>(loop);
}

}
}
