#ifndef _CST_EVX_IO_WATCHER_HPP
#define _CST_EVX_IO_WATCHER_HPP

#include <cst/evx/core/watcher.hpp>

namespace cst {
namespace evx {

class io_watcher : private watcher {
public:
    typedef std::function<void(io_watcher&, int)> io_handler_t;

    io_watcher(event_loop& loop, int fd, int events, const io_handler_t& handler);

    int fd() const noexcept
    { return watcher::fd(); }

    void enable_read() noexcept
    { enable_events(ev_in); }

    void enable_write() noexcept
    { enable_events(ev_out); }

    void disable_read() noexcept
    { disable_events(ev_in); }

    void disable_write() noexcept
    { disable_events(ev_out); }

    ~io_watcher();
};

}
}

#endif
