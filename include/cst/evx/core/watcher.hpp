#ifndef _CST_EVX_WATCHER_HPP
#define _CST_EVX_WATCHER_HPP

#include <memory>
#include <ostream>
#include <functional>

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
public:
    friend event_loop;

    friend std::ostream& operator<<(std::ostream& os, const watcher& w);

    typedef std::function<void(int)> handler_t;

    watcher(event_loop& loop, enum watch_t type,
            const handler_t& handler = nullptr) noexcept
        : loop_(loop), type_(type), handler_(handler)
    { }

    int fd() const noexcept
    { return fd_; }

    bool pending() const noexcept
    { return pending_; }

    int events() const noexcept
    { return events_; }

    int revents() const noexcept
    { return revents_; }

private:
    void pending(bool p) noexcept
    { pending_ = p; }

    int& revents() noexcept
    { return revents_; }

    void handle()
    { handler_(revents_); }

    event_loop& loop_;
    enum watch_t type_;
    handler_t handler_;
    int fd_ = -1;
    bool pending_ = false;
    int events_ = ev_none;
    int revents_ = ev_none;
};

using watcher_ptr = std::shared_ptr<watcher>;

inline std::ostream& operator<<(std::ostream& os, const watcher& w)
{
    return os << "watcher[type='" << watcher_type_text[w.type_]
              << "', fd=" << w.fd_ << "]";
}

}
}

#endif
