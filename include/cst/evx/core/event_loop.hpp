#ifndef _CST_EVX_EVENT_LOOP_HPP
#define _CST_EVX_EVENT_LOOP_HPP

#include <cst/evx/core/watcher.hpp>
#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <set>

namespace cst {
namespace evx {

class poller;
class io_watcher;

class event_loop {
    friend class watcher;
    friend class io_watcher;
    friend class timer_watcher;
    friend class poll_poller;
    friend class epoll_poller;

public:
    typedef std::function<void()> functor_t;

    event_loop(const event_loop&) = delete;
    event_loop& operator=(const event_loop&) = delete;

    explicit event_loop(const logger_ptr& logger = logging::default_logger());

    const logger_ptr& logger() const noexcept
    { return logger_; }

    void run();

    void assert_in_loop_thread();

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
    bool in_loop_thread_() const noexcept
    { return tid_ == std::this_thread::get_id(); }

    void run_in_loop_(const functor_t& fn);
    void queue_in_loop_(const functor_t& fn);
    void notify_();

    void fd_sync_();

    void invoke_pending_events_();
    void invoke_pending_functors_();


    static thread_local event_loop* thread_loop_;

    const std::thread::id tid_ = std::this_thread::get_id();

    const int waittime_ = 30000;

    logger_ptr logger_;
    std::unique_ptr<poller> poller_;

    struct fd_watch {
        int events = ev_none;
        std::set<watcher*> watchers;
    };

    std::map<int, fd_watch> watchers_;
    std::set<int> changed_fds_;

    int evfd_ = -1;
    std::unique_ptr<io_watcher> fnw_;

    std::list<watcher*> pending_events_;

    bool invoking_functors_ = false;

    std::mutex pfmtx_;
    std::list<functor_t> pending_functors_;
};

}
}

#endif
