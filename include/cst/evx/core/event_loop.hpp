#ifndef _CST_EVX_EVENT_LOOP_HPP
#define _CST_EVX_EVENT_LOOP_HPP

#include <cst/evx/core/watcher.hpp>
#include <list>
#include <map>
#include <set>

namespace cst {
namespace evx {

class poller;

class event_loop {
    friend class watcher;
    friend class io_watcher;
    friend class timer_watcher;
    friend class poll_poller;
    friend class epoll_poller;

public:
    event_loop(const event_loop&) = delete;
    event_loop& operator=(const event_loop&) = delete;

    explicit event_loop(const logger_ptr& logger = logging::default_logger());

    const logger_ptr& logger() const noexcept
    { return logger_; }

    void run();

    ~event_loop();

private: /* may used by friends */
    void add_watcher(watcher* w);
    void del_watcher(watcher* w);

    void fd_change(int fd)
    { changed_fds_.insert(fd); }

    void fd_kill(int fd);

    void fd_event(int fd, int revents);

    void feed_event(watcher* w, int revents);

private: /* internal used */
    void fd_sync_();

    void invoke_pendings_();

    const int waittime_ = 30000;

    logger_ptr logger_;
    std::unique_ptr<poller> poller_;

    struct fd_watch {
        int events = ev_none;
        std::set<watcher*> watchers;
    };

    std::map<int, fd_watch> watchers_;
    std::set<int> changed_fds_;

    std::list<watcher*> pendings_;
};

}
}

#endif
