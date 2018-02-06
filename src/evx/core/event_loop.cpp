#include <cst/logging/logger/trivial_logger.hpp>
#include <cst/evx/core/event_loop.hpp>
#include "evx/core/poller/poller.hpp"

namespace cst {
namespace evx {

event_loop::logger_ptr_t
event_loop::default_logger_ = logging::trivial_stderr_logger("libevx");

event_loop::event_loop(const logger_ptr_t& logger)
    : logger_(logger),
      poller_(poller::new_poller(*this))
{
    if (!logger_)
        logger_ = get_default_logger();
}

void event_loop::run()
{
}

/*
 * Defining destructor here so that it sees the poller's definitioin,
 * in order to make the destruction of unique_ptr<poller> work.
 */
event_loop::~event_loop() = default;

}
}
