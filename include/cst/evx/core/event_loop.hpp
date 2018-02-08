#ifndef _CST_EVX_EVENT_LOOP_HPP
#define _CST_EVX_EVENT_LOOP_HPP

#include <cst/evx/core/common.hpp>
#include <cst/evx/core/watcher.hpp>
#include <list>
#include <map>
#include <set>

namespace cst {
namespace evx {

class poller;

class event_loop {
public:
    event_loop(const event_loop&) = delete;
    event_loop& operator=(const event_loop&) = delete;

    explicit event_loop(const logger_ptr& logger = nullptr);

    static const logger_ptr& get_default_logger() noexcept
    { return default_logger_; }

    static void set_default_logger(const logger_ptr& logger) noexcept
    { default_logger_ = logger; }

    const logger_ptr& logger() const noexcept
    { return logger_; }

    void add_watcher(const watcher_ptr& w);
    void del_watcher(const watcher_ptr& w);

    void feed_event(int fd, int revents);

    void run();

    ~event_loop();

private:
    void invoke_pendings_();

    static logger_ptr default_logger_;

    const int waittime_ = 30000;

    logger_ptr logger_;
    std::unique_ptr<poller> poller_;

    std::map<int, std::set<watcher_ptr>> watchers_;

    std::list<watcher_ptr> pendings_;
};

}
}

#endif
