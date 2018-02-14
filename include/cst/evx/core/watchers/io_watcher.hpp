#ifndef _CST_EVX_IO_WATCHER_HPP
#define _CST_EVX_IO_WATCHER_HPP

#include <cst/evx/core/watcher.hpp>

namespace cst {
namespace evx {

class io_watcher : public watcher {
public:
    io_watcher(event_loop& loop, int fd, int events, const handler_t& handler);
    ~io_watcher();
};

}
}

#endif
