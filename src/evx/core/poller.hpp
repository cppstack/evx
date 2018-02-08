#ifndef _CST_EVX_POLLER_HPP
#define _CST_EVX_POLLER_HPP

#include <cst/evx/core/common.hpp>
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

    virtual void modify(int fd, int nev) = 0;
    virtual void poll(int timeout) = 0;

    virtual ~poller() = default;

protected:
    const logger_ptr& logger() const noexcept;

    event_loop& loop_;
};

}
}

#endif
