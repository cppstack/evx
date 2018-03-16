#ifndef _CST_EVX_POLL_POLLER_HPP
#define _CST_EVX_POLL_POLLER_HPP

#include "core/poller.hpp"
#include <cst/lnx/os/poll.hpp>

#include <vector>
#include <map>

namespace cst {
namespace evx {

class poll_poller : public poller {
public:
    using poller::poller;

    void modify(int fd, int oev, int nev) override;
    void poll(int timeout) override;

private:
    std::vector<lnx::pollfd> pollfds_;
    std::map<int, unsigned> pollidxs_;
};

}
}

#endif
