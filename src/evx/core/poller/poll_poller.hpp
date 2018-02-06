#ifndef _CST_EVX_POLL_POLLER_HPP
#define _CST_EVX_POLL_POLLER_HPP

#include "evx/core/poller/poller.hpp"

#include <vector>

struct pollfd;

namespace cst {
namespace evx {

class poll_poller : public poller {
public:
    using poller::poller;

    void poll(int timeout) override;

private:
    std::vector<pollfd> pollfds_;
};

}
}

#endif
