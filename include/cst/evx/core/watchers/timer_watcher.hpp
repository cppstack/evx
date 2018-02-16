#ifndef _CST_EVX_TIMER_WATCHER_HPP
#define _CST_EVX_TIMER_WATCHER_HPP

#include <cst/evx/core/watcher.hpp>
#include <functional>
#include <chrono>

namespace cst {
namespace evx {

class timer_watcher : public watcher {
public:
    typedef std::function<void(timer_watcher&)> timer_handler_t;

    timer_watcher(event_loop& loop,
          const std::chrono::time_point<std::chrono::system_clock>& time_point,
          const std::chrono::nanoseconds& interval,
          const timer_handler_t& handler);

    timer_watcher(event_loop& loop,
          const std::chrono::time_point<std::chrono::system_clock>& time_point,
          const timer_handler_t& handler)
        : timer_watcher(loop, time_point, std::chrono::nanoseconds(0), handler)
    { }

    timer_watcher(event_loop& loop,
          const std::chrono::nanoseconds& time,
          const std::chrono::nanoseconds& interval,
          const timer_handler_t& handler);

    timer_watcher(event_loop& loop,
          const std::chrono::nanoseconds& time,
          const timer_handler_t& handler)
        : timer_watcher(loop, time, std::chrono::nanoseconds(0), handler)
    { }

    uint64_t expirations() const noexcept
    { return expirations_; }

    void enable() noexcept
    { enable_events(ev_in); }

    void disable() noexcept
    { disable_events(ev_in); }

    void handle() override;

    ~timer_watcher();

private:
    uint64_t expirations_ = 0;

    const timer_handler_t handler_;
};

}
}

#endif
