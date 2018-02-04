#ifndef _CST_EVX_POLL_POLLER_HPP
#define _CST_EVX_POLL_POLLER_HPP

#include "core/poller.hpp"

namespace cst {
namespace evx {

class poll_poller : public poller {
public:
    using poller::poller;
};

}
}

#endif
