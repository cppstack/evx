#ifndef _CST_EVX_POLLER_HPP
#define _CST_EVX_POLLER_HPP

namespace cst {
namespace evx {

class event_loop;

class poller {
public:
    poller(const poller&) = delete;
    poller& operator=(const poller&) = delete;

    explicit poller(event_loop& loop) noexcept
        : loop_(loop)
    { }

    virtual ~poller() = default;

protected:
    event_loop& loop_;
};

}
}

#endif
