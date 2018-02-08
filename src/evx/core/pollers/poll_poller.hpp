#ifndef _CST_EVX_POLL_POLLER_HPP
#define _CST_EVX_POLL_POLLER_HPP

#include "evx/core/poller.hpp"

#include <vector>
#include <map>

struct pollfd;

namespace cst {
namespace evx {

class poll_poller : public poller {
public:
    using poller::poller;

    void modify(int fd, int nev) override;
    void poll(int timeout) override;

private:
    std::vector<pollfd> pollfds_;
    std::map<int, unsigned> pollidxs_;
};

}
}

#endif
