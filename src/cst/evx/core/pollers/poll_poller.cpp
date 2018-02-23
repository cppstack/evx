#include <cst/evx/core/event_loop.hpp>
#include "core/pollers/poll_poller.hpp"
#include "os/poll.hpp"

namespace cst {
namespace evx {

void poll_poller::modify(int fd, int /*oev*/, int nev)
{
    /* TODO: lock it in multithreading */

    auto it = pollidxs_.find(fd);

    if (it == pollidxs_.end()) {
        if (nev) {
            /* new fd and have events interested */
            pollfds_.push_back({fd, (short) nev, 0});
            pollidxs_[fd] = pollfds_.size() - 1;
        }
    } else {
        unsigned idx = it->second;

        if (nev)
            /* just update */
            pollfds_[idx].events = nev;
        else {
            /* nev is none, delete fd */
            unsigned last = pollfds_.size() - 1;
            if (idx != last) {
                std::swap(pollfds_[idx], pollfds_[last]);
                pollidxs_[pollfds_[idx].fd] = idx;
            }

            pollfds_.pop_back();
            pollidxs_.erase(fd);
        }
    }
}

void poll_poller::poll(int timeout)
{
    size_t nr = os::Poll(pollfds_.data(), pollfds_.size(), timeout, logger_);

    for (auto it = pollfds_.cbegin(); nr && it != pollfds_.cend(); ++it)
        if (it->revents) {
            --nr;

            if (it->revents & POLLNVAL)
                loop_.fd_kill(it->fd);
            else {
                int ev = it->revents & (POLLIN | POLLOUT | POLLERR | POLLHUP);
                loop_.fd_event(it->fd, ev);
            }
        }
}

}
}
