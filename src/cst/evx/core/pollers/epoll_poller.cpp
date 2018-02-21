#include <cst/evx/core/event_loop.hpp>
#include "core/pollers/epoll_poller.hpp"
#include "os/epoll.hpp"

namespace cst {
namespace evx {

using namespace os;

epoll_poller::epoll_poller(event_loop& loop)
    : poller(loop),
      epfd_(Epoll_create1(0, logger_)),
      events_(64)
{ }

void epoll_poller::modify(int fd, int oev, int nev)
{
    /* TODO: lock it in multithreading */

    if (oev == nev && nev)
        return;

    if (!nev) {
        int err = Epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr, logger_);
        if (err == EPERM) {
            if (!eperm_fds_.erase(fd))
                LOG_ERROR(logger_) << "eperm_fd(" << fd << ") not found";
        }
    } else {
        struct ::epoll_event event;
        event.events = nev;
        event.data.fd = fd;

        if (!oev) {
            int err = Epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event, logger_);
            if (err == EPERM) {
                LOG_NOTICE(logger_) << "epoll_ctl() EPERM, fd: " << fd;
                eperm_fds_.insert(fd);
            }
        } else {
            int err = Epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event, logger_);
            if (err == EPERM) {
                if (!eperm_fds_.count(fd))
                    LOG_ERROR(logger_) << "eperm_fd(" << fd << ") not found";
            }
        }
    }
}

void epoll_poller::poll(int timeout)
{
    if (eperm_fds_.size()) {
        for (int fd : eperm_fds_)
            loop_.fd_event(fd, EPOLLIN | EPOLLOUT);

        timeout = 0;
    }

    size_t nr = Epoll_wait(epfd_, events_.data(), events_.size(), timeout, logger_);

    for (size_t i = 0; i < nr; ++i) {
        int ev = events_[i].events & (EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);
        loop_.fd_event(events_[i].data.fd, ev);
    }

    if (nr && nr == events_.size()) {
        events_.clear();
        events_.resize(nr << 1);
    }
}

}
}
