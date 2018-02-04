#ifndef _CST_EVX_POLLER_FACTORY_HPP
#define _CST_EVX_POLLER_FACTORY_HPP

#include <memory>

namespace cst {
namespace evx {

class event_loop;
class poller;

class poller_factory {
public:
    static std::unique_ptr<poller> new_poller(event_loop& loop);

    poller_factory() = delete;
};

}
}

#endif
