#ifndef _CST_EVX_IO_WATCHER_HPP
#define _CST_EVX_IO_WATCHER_HPP

#include <cst/evx/core/watcher.hpp>
#include <functional>

namespace cst {
namespace evx {

class io_watcher : public watcher {
public:
    typedef std::function<void(io_watcher&)> io_handler_t;

    io_watcher(event_loop& loop, int fd, int events, const io_handler_t& handler);

    int fd() const noexcept
    { return fd_; }

    int revents() const noexcept
    { return revents_; }

    bool is_reading() const noexcept
    { return events_ & ev_in; }

    bool is_writing() const noexcept
    { return events_ & ev_out; }

    void enable_read()
    { enable_events(ev_in); }

    void enable_write()
    { enable_events(ev_out); }

    void enable_rdwr()
    { enable_events(ev_in | ev_out); }

    void disable_read()
    { disable_events(ev_in); }

    void disable_write()
    { disable_events(ev_out); }

    void disable_rdwr()
    { disable_events(ev_in | ev_out); }

    void handle() override
    { handler_(*this); }

    ~io_watcher();

private:
    const io_handler_t handler_;
};

}
}

#endif
