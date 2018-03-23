#include "core/pollers/epoll_poller.hpp"
#include <cst/evx/core/event_loop.hpp>
#include <cst/lnx/error/throw.hpp>
#include <cst/logging/logger.hpp>

namespace cst {
namespace evx {

epoll_poller::epoll_poller(event_loop& loop)
    : poller(loop),
      epfd_(lnx::Epoll_create1(0)),
      events_(64)
{ }

void epoll_poller::modify(int fd, int oev, int nev)
{
    /* TODO: lock it in multithreading */

    LOG_DEBUG_F(logger_, "epoller::modify(%d, %d, %d)", fd, oev, nev);

    if (!nev) {
        if (!oev) {
            /* it was already gone, and we want it go again, which means
             * we really need to delete it.
             * this only happens when we got events from a non-interested fd
             */
            std::error_code ec;
            lnx::Epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr, &ec);
            if (ec) {
                if (ec.value() == EPERM) {
                    if (!eperm_fds_.erase(fd))
                        LOG_ERROR(logger_) << "eperm_fd(" << fd << ") not found";
                } else
                    lnx::throw_system_error(ec, "epoll_ctl()", logger_);
            }
        }
    } else {
        lnx::epoll_event event;
        event.events = nev;
        event.data.fd = fd;
        std::error_code ec;

        if (!oev) {
            lnx::Epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event, &ec);
            if (ec) {
                if (ec.value() == EPERM) {
                    LOG_NOTICE(logger_) << "epoll_ctl() EPERM, fd: " << fd;
                    eperm_fds_.insert(fd);
                } else
                    lnx::throw_system_error(ec, "epoll_ctl()", logger_);
            }
        } else {
            lnx::Epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event, &ec);
            if (ec) {
                if (ec.value() == EPERM) {
                    if (!eperm_fds_.count(fd))
                        LOG_ERROR(logger_) << "eperm_fd(" << fd << ") not found";
                } else
                    lnx::throw_system_error(ec, "epoll_ctl()", logger_);
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

    int nr = lnx::Epoll_wait(epfd_, events_.data(), events_.size(), timeout);

    for (int i = 0; i < nr; ++i) {
        int ev = events_[i].events & (EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP);
        loop_.fd_event(events_[i].data.fd, ev);
    }

    if (nr > 0 && (size_t) nr == events_.size()) {
        events_.clear();
        events_.resize(nr << 1);
    }
}

}
}
