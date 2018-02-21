#include <cst/evx/core/watcher.hpp>
#include <cst/evx/core/event_loop.hpp>

namespace cst {
namespace evx {

watcher::watcher(event_loop& loop, watch_t type, int events) noexcept
    : loop_(loop), type_(type), events_(filter(events)), logger_(loop.logger())
{ }

void watcher::enable_events(int ev) noexcept
{
    ev = filter(ev);
    if ((events_ & ev) != ev) {
        events_ |= ev;
        loop_.fd_change(fd_);
    }
}

void watcher::disable_events(int ev) noexcept
{
    ev = filter(ev);
    if ((events_ & ev)) {
        events_ &= ~ev;
        loop_.fd_change(fd_);
    }
}

}
}
