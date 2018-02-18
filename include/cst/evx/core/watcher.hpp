#ifndef _CST_EVX_WATCHER_HPP
#define _CST_EVX_WATCHER_HPP

#include <cst/evx/core/logger_ptr.hpp>
#include <ostream>

namespace cst {
namespace evx {

enum watch_t {
    w_io,
    w_timer,
    w_stat,
    w_signal
};

const char* const watcher_type_text[] = {
    "io", "timer", "stat", "signal"
};

/* the values must be equivalent to (E)POLLxxx for convenience */
enum {
    ev_none = 0x000,
    ev_in   = 0x001,
    ev_out  = 0x004,
    ev_err  = 0x008,
    ev_hup  = 0x010
};

class event_loop;

class watcher {
    friend event_loop;
    friend std::ostream& operator<<(std::ostream& os, const watcher& w);

protected:
    watcher(const watcher&) = delete;
    watcher& operator=(const watcher&) = delete;

    watcher(event_loop& loop, watch_t type, int events) noexcept;

    static int filter(int ev) noexcept
    { return ev & (ev_in | ev_out); }

    int fd() const noexcept
    { return fd_; }

    void enable_events(int ev) noexcept;
    void disable_events(int ev) noexcept;

    bool pending() const noexcept
    { return pending_; }

    virtual void handle() = 0;

    virtual ~watcher() = default;

    event_loop& loop_;
    const watch_t type_;
    int fd_ = -1;
    bool pending_ = false;
    int events_ = ev_none;
    int revents_ = ev_none;

    const logger_ptr& logger_;
};

inline std::ostream& operator<<(std::ostream& os, const watcher& w)
{
    return os << "watcher[type='" << watcher_type_text[w.type_]
              << "', fd=" << w.fd_ << "]";
}

}
}

#endif
