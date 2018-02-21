#ifndef _CST_EVX_EPOLL_POLLER_HPP
#define _CST_EVX_EPOLL_POLLER_HPP

#include "core/poller.hpp"
#include <sys/epoll.h>

#include <vector>
#include <set>

namespace cst {
namespace evx {

class epoll_poller : public poller {
public:
    explicit epoll_poller(event_loop& loop);

    void modify(int fd, int oev, int nev) override;
    void poll(int timeout) override;

private:
    int epfd_ = -1;
    std::vector<struct ::epoll_event> events_;
    std::set<int> eperm_fds_;
};

}
}

#endif
