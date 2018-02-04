#include <cst/evx/core/event_loop.hpp>
#include "core/poller.hpp"
#include "core/poller_factory.hpp"

namespace cst {
namespace evx {

event_loop::event_loop()
    : poller_(poller_factory::new_poller(*this))
{
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
