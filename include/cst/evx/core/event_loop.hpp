#ifndef _CST_EVX_EVENT_LOOP_HPP
#define _CST_EVX_EVENT_LOOP_HPP

#include <memory>

namespace cst {
namespace evx {

class poller;

class event_loop {
public:
    event_loop(const event_loop&) = delete;
    event_loop& operator=(const event_loop&) = delete;

    event_loop();

    void run();

    ~event_loop();

private:
    std::unique_ptr<poller> poller_;
};

}
}

#endif
