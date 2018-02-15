#ifndef _CST_EVX_POLLER_HPP
#define _CST_EVX_POLLER_HPP

#include <cst/evx/core/event_loop.hpp>

namespace cst {
namespace evx {

class poller {
public:
    poller(const poller&) = delete;
    poller& operator=(const poller&) = delete;

    static std::unique_ptr<poller> new_poller(event_loop& loop);

    explicit poller(event_loop& loop) noexcept
        : loop_(loop)
    { }

    virtual void modify(int fd, int nev) = 0;
    virtual void poll(int timeout) = 0;

    virtual ~poller() = default;

protected:
    const logger_ptr& logger() const noexcept
    { return loop_.logger(); }

    event_loop& loop_;
};

}
}

#endif
