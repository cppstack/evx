#ifndef _CST_EVX_TIMER_WATCHER_HPP
#define _CST_EVX_TIMER_WATCHER_HPP

#include <cst/evx/core/watcher.hpp>

namespace cst {
namespace evx {

class timer_watcher : private watcher {
public:
    typedef std::function<void(timer_watcher&)> timer_handler_t;

    timer_watcher(event_loop& loop, const timer_handler_t& handler);

    void enable() noexcept
    { }

    void disable() noexcept
    { }

    ~timer_watcher();
};

}
}

#endif
