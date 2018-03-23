#include <cst/evx/core/event_loop.hpp>
#include <cst/evx/core/watchers/io_watcher.hpp>
#include <cst/lnx/os/eventfd.h>
#include <cst/logging/logger/trivial_loggers.hpp>
#include "core/poller.hpp"
#include <unistd.h>

namespace cst {
namespace evx {

thread_local event_loop* event_loop::thread_loop_ = nullptr;

const logger_ptr evx_default_logger = logging::trivial_stderr_logger("libevx");

event_loop::event_loop(const logger_ptr& logger)
    : logger_(logger),
      poller_(poller::new_poller(*this))
{
    if (!logger_)
        logger_ = evx_default_logger;

    if (thread_loop_ != nullptr)
        throw std::logic_error("event_loop existed in thread");

    thread_loop_ = this;

    evfd_ = lnx::Eventfd(0, 0);
    fnw_ = std::make_unique<io_watcher>(*this, evfd_, ev_in,
        [this](io_watcher& w) {
            uint64_t cnt;
            if (::read(w.fd(), &cnt, 8) != 8) {
                LOG_CRIT(logger_) << "eventfd read() error";
                w.disable_read();
            }
        }
    );
}

void event_loop::add_watcher(watcher* w)
{
    if (w->fd() < 0) {
        LOG_ERROR(logger_) << __func__ << "(): negative fd";
        return;
    }

    if (!watchers_[w->fd()].watchers.insert(w).second)
        LOG_ERROR(logger_) << *w << " already in loop";

    if (w->events_)
        fd_change(w->fd());
}

void event_loop::del_watcher(watcher* w)
{
    auto it1 = watchers_.find(w->fd());
    if (it1 == watchers_.end()) {
        LOG_ERROR(logger_) << *w << " not found in loop";
        return;
    }

    auto& set = it1->second.watchers;
    auto it2 = set.find(w);
    if (it2 == set.end()) {
        LOG_ERROR(logger_) << *w << " not found in loop";
        return;
    }

    set.erase(it2);
    if (set.empty())
        watchers_.erase(it1);

    if (w->events_)
        fd_change(w->fd());
}

void event_loop::fd_kill(int fd)
{
    auto it = watchers_.find(fd);
    if (it != watchers_.end())
        for (auto& w : it->second.watchers) {
            w->disable_events(ev_in | ev_out);
            feed_event(w, ev_err);
        }
}

void event_loop::fd_event(int fd, int revents)
{
    auto it = watchers_.find(fd);
    if (it == watchers_.end()) {
        LOG_ERROR(logger_) << "descriptor " << fd << " not found in loop";
        return;
    }

    LOG_DEBUG_F(logger_, "fd_event(%d, %d)", fd, revents);

    int iev = it->second.events;
    int rev = revents & ~ev_hup & ~ev_err;
    if (iev == ev_none || (rev && !(iev & rev))) {
        /* if we're not interested in any events but we still got events,
         * (which means the fd is still open), we need to del it in poller.
         * if we got events which we are not interested in, we need to update
         * the poller again, the later case shouldn't happen.
         */
        poller_->modify(fd, iev, iev);
        return;
    }

    for (auto& w : it->second.watchers)
        feed_event(w, revents);
}

void event_loop::feed_event(watcher* w, int revents)
{
    int ev = (w->events_ | ev_hup | ev_err) & revents;
    if (ev) {
        if (! w->pending()) {
            w->revents_ = ev;
            w->pending_ = true;
            pending_events_.push_back(w);
        } else
            w->revents_ |= ev;
    }
}

void event_loop::run()
{
    for (;;) {
        fd_sync_();
        poller_->poll(waittime_);
        invoke_pending_events_();
        invoke_pending_functors_();
    }
}

void event_loop::assert_in_loop_thread()
{
    if (!in_loop_thread_())
        throw std::logic_error("not in loop thread");
}

void event_loop::run_in_loop_(const functor_t& fn)
{
    if (in_loop_thread_())
        fn();
    else
        queue_in_loop_(fn);
}

void event_loop::queue_in_loop_(const functor_t& fn)
{
    {
        std::lock_guard<std::mutex> lock(pfmtx_);
        pending_functors_.push_back(fn);
    }

    if (!in_loop_thread_() || invoking_functors_)
        notify_();
}

void event_loop::notify_()
{
    uint64_t one = 1;
    if (::write(evfd_, &one, 8) != 8)
        LOG_CRIT(logger_) << "eventfd write() error";
}

void event_loop::fd_sync_()
{
    for (int fd : changed_fds_) {
        auto it = watchers_.find(fd);
        if (it == watchers_.end())
            /* not found, want to delete it */
            poller_->modify(fd, -1, ev_none);
        else {
            int n_events = ev_none;
            for (auto& w : it->second.watchers)
                n_events |= w->events_;

            if (it->second.events != n_events) {
                poller_->modify(fd, it->second.events, n_events);
                it->second.events = n_events;
            }
        }
    }

    changed_fds_.clear();
}

void event_loop::invoke_pending_events_()
{
    while (!pending_events_.empty()) {
        auto& w = pending_events_.front();
        w->pending_ = false;
        /* TODO: catch exceptions ? */
        w->handle();
        pending_events_.pop_front();
    }
}

void event_loop::invoke_pending_functors_()
{
    std::list<functor_t> functors;
    invoking_functors_ = true;

    {
        std::lock_guard<std::mutex> lock(pfmtx_);
        pending_functors_.swap(functors);
    }

    while (!functors.empty()) {
        /* TODO: catch exceptions ? */
        functors.front()();
        functors.pop_front();
    }

    invoking_functors_ = false;
}

/*
 * Defining destructor here so that it sees the poller's definitioin,
 * in order to make the destruction of unique_ptr<poller> work.
 */
event_loop::~event_loop()
{
    ::close(evfd_);
    thread_loop_ = nullptr;
}

}
}
