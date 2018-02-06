#ifndef _CST_EVX_POLLER_HPP
#define _CST_EVX_POLLER_HPP

#include <cst/evx/core/event_loop.hpp>
#include <memory>

namespace cst {
namespace evx {

class event_loop;

class poller {
public:
    poller(const poller&) = delete;
    poller& operator=(const poller&) = delete;

    static std::unique_ptr<poller> new_poller(event_loop& loop);

    explicit poller(event_loop& loop) noexcept
        : loop_(loop)
    { }

    virtual void poll(int timeout) = 0;

    virtual ~poller() = default;

protected:
    event_loop& loop_;

    auto logger() const noexcept -> decltype(loop_.logger())
    { return loop_.logger(); }
};

}
}

#endif
