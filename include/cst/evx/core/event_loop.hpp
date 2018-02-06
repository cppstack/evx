#ifndef _CST_EVX_EVENT_LOOP_HPP
#define _CST_EVX_EVENT_LOOP_HPP

#include <memory>

namespace cst {

namespace logging { class logger; }

namespace evx {

class poller;

class event_loop {
public:
    event_loop(const event_loop&) = delete;
    event_loop& operator=(const event_loop&) = delete;

    using logger_ptr_t = std::shared_ptr<logging::logger>;

    explicit event_loop(const logger_ptr_t& logger = nullptr);

    static const logger_ptr_t& get_default_logger() noexcept
    { return default_logger_; }

    static void set_default_logger(const logger_ptr_t& logger) noexcept
    { default_logger_ = logger; }

    const logger_ptr_t& logger() const noexcept
    { return logger_; }

    void run();

    ~event_loop();

private:
    static logger_ptr_t default_logger_;

    logger_ptr_t logger_;
    std::unique_ptr<poller> poller_;
};

}
}

#endif
