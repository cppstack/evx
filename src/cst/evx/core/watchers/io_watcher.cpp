#include <cst/evx/core/watchers/io_watcher.hpp>
#include <cst/evx/core/event_loop.hpp>

namespace cst {
namespace evx {

io_watcher::io_watcher(event_loop& loop, int fd, int events,
                       const io_handler_t& handler)
    : watcher(loop, w_io, events), handler_(handler)
{
    fd_ = fd;
    loop_.add_watcher(this);
}

io_watcher::~io_watcher()
{
    loop_.del_watcher(this);
}

}
}
