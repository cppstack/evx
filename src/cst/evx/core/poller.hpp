#ifndef _CST_EVX_POLLER_HPP
#define _CST_EVX_POLLER_HPP

#include <cst/logging/logger_ptr.hpp>

namespace cst {
namespace evx {

using logging::logger_ptr;

class event_loop;

class poller {
public:
    poller(const poller&) = delete;
    poller& operator=(const poller&) = delete;

    static std::unique_ptr<poller> new_poller(event_loop& loop);

    explicit poller(event_loop& loop) noexcept;

    /*
     * nev == ev_none means we want to del it, but not necessarily do it
     * if it's epoll, which needs to check oev further.
     * oev == ev_none means it was already gone
     * oev == -1      means we didn't find the watcher
     */
    virtual void modify(int fd, int oev, int nev) = 0;
    virtual void poll(int timeout) = 0;

    virtual ~poller() = default;

protected:
    event_loop& loop_;
    const logger_ptr& logger_;
};

}
}

#endif
