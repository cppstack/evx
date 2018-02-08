#include <cst/logging/logger/trivial_logger.hpp>
#include <cst/evx/core/event_loop.hpp>
#include "evx/core/poller.hpp"
#include <algorithm>

namespace cst {
namespace evx {

logger_ptr event_loop::default_logger_ = logging::trivial_stderr_logger("libevx");

event_loop::event_loop(const logger_ptr& logger)
    : logger_(logger),
      poller_(poller::new_poller(*this))
{
    if (!logger_)
        logger_ = get_default_logger();
}

void event_loop::add_watcher(const watcher_ptr& w)
{
    if (!watchers_[w->fd()].insert(w).second)
        LOG_ERROR(logger_) << *w << " already in loop";
}

void event_loop::del_watcher(const watcher_ptr& w)
{
    auto it1 = watchers_.find(w->fd());
    if (it1 == watchers_.end()) {
        LOG_ERROR(logger_) << *w << " not found in loop";
        return;
    }

    auto& set = it1->second;
    auto it2 = set.find(w);
    if (it2 == set.end()) {
        LOG_ERROR(logger_) << *w << " not found in loop";
        return;
    }

    set.erase(it2);
    if (set.empty())
        watchers_.erase(it1);
}

void event_loop::feed_event(int fd, int revents)
{
    auto it = watchers_.find(fd);
    if (it == watchers_.end()) {
        LOG_ERROR(logger_) << "descriptor " << fd << " not found in loop";
        return;
    }

    for (auto& w : it->second) {
        w->revents() |= (w->events() | ev_hup | ev_err) & revents;
        if (w->revents() && !w->pending()) {
            pendings_.push_back(w);
            w->pending(true);
        }
    }
}

void event_loop::run()
{
    for (;;) {
        poller_->poll(waittime_);
        invoke_pendings_();
    }
}

void event_loop::invoke_pendings_()
{
    while (! pendings_.empty()) {
        pendings_.front()->handle();
        pendings_.pop_front();
    }
}

/*
 * Defining destructor here so that it sees the poller's definitioin,
 * in order to make the destruction of unique_ptr<poller> work.
 */
event_loop::~event_loop() = default;

}
}
