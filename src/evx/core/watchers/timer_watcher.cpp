#include <cst/evx/core/watchers/timer_watcher.hpp>
#include <cst/evx/core/event_loop.hpp>

namespace cst {
namespace evx {

timer_watcher::timer_watcher(event_loop& loop,
                             const timer_handler_t& handler)
    : watcher(loop, w_timer, ev_in, std::bind(handler, std::ref(*this)))
{
    loop_.add_watcher(this);
}

timer_watcher::~timer_watcher()
{
    loop_.del_watcher(this);
}

}
}
