#ifndef _CST_EVX_POLLER_HPP
#define _CST_EVX_POLLER_HPP

#include <cst/evx/core/logger_ptr.hpp>

namespace cst {
namespace evx {

class event_loop;

class poller {
public:
    poller(const poller&) = delete;
    poller& operator=(const poller&) = delete;

    static std::unique_ptr<poller> new_poller(event_loop& loop);

    explicit poller(event_loop& loop) noexcept;

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
