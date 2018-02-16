#include <cst/evx/core/watchers/timer_watcher.hpp>
#include <cst/evx/core/event_loop.hpp>
#include <unistd.h>
#include "evx/os/time.hpp"

namespace cst {
namespace evx {

timer_watcher::timer_watcher(event_loop& loop,
          const std::chrono::time_point<std::chrono::system_clock>& time_point,
          const std::chrono::nanoseconds& interval,
          const timer_handler_t& handler)
    : watcher(loop, w_timer, ev_in), handler_(handler)
{
    const logger_ptr& log = loop_.logger();

    fd_ = os::timerfd_create(CLOCK_REALTIME, 0, log);

    const auto tpns = std::chrono::duration_cast<std::chrono::nanoseconds>(time_point.time_since_epoch());
    const auto sc = std::nano::den;

    struct ::itimerspec tspec;
    tspec.it_value.tv_sec = tpns.count() / sc;
    tspec.it_value.tv_nsec = tpns.count() % sc;
    tspec.it_interval.tv_sec = interval.count() / sc;
    tspec.it_interval.tv_nsec = interval.count() % sc;

    os::timerfd_settime(fd_, TFD_TIMER_ABSTIME, &tspec, nullptr, log);

    loop_.add_watcher(this);
}

timer_watcher::timer_watcher(event_loop& loop,
          const std::chrono::nanoseconds& time,
          const std::chrono::nanoseconds& interval,
          const timer_handler_t& handler)
    : watcher(loop, w_timer, ev_in), handler_(handler)
{
    const logger_ptr& log = loop_.logger();

    fd_ = os::timerfd_create(CLOCK_MONOTONIC, 0, log);

    const auto sc = std::nano::den;

    struct ::itimerspec tspec;
    tspec.it_value.tv_sec = time.count() / sc;
    tspec.it_value.tv_nsec = time.count() % sc;
    tspec.it_interval.tv_sec = interval.count() / sc;
    tspec.it_interval.tv_nsec = interval.count() % sc;

    os::timerfd_settime(fd_, 0, &tspec, nullptr, log);

    loop_.add_watcher(this);
}

void timer_watcher::handle()
{
    const logger_ptr& log = loop_.logger();

    if (revents_ & ev_err) {
        LOG_ERROR(log) << "timer events error";
        disable();
        return;
    }

    /* maybe turn off the read() to save a system call */
    if (::read(fd_, &expirations_, sizeof(uint64_t)) != sizeof(uint64_t)) {
        LOG_ERROR(log) << "timerfd read error";
        disable();
        return;
    }

    handler_(*this);
}

timer_watcher::~timer_watcher()
{
    ::close(fd_);

    loop_.del_watcher(this);
}

}
}
