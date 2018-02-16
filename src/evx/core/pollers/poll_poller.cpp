#include <cst/evx/core/event_loop.hpp>
#include "evx/core/pollers/poll_poller.hpp"
#include "evx/os/poll.hpp"

namespace cst {
namespace evx {

void poll_poller::modify(int fd, int nev)
{
    /* TODO: lock it in multithreading */

    short pev = (short) (nev & (POLLIN | POLLOUT));

    auto it = pollidxs_.find(fd);

    if (it == pollidxs_.end()) {
        if (pev) {
            /* new fd and have events interested */
            pollfds_.push_back({fd, pev, 0});
            pollidxs_[fd] = pollfds_.size() - 1;
        }
    } else {
        unsigned idx = it->second;

        if (pev)
            /* just update */
            pollfds_[idx].events = pev;
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
    int nr = os::poll(pollfds_.data(), pollfds_.size(), timeout, loop_.logger());

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
