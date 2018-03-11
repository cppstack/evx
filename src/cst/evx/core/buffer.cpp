#include <cst/evx/core/buffer.hpp>

namespace cst {
namespace evx {

void buffer::grow_(std::size_t len)
{
    const std::size_t nread = readable();

    if (len <= prependable() - init_prep_ + writable()) {
        std::copy(rbegin_(), wbegin_(), pbegin_());
        ridx_ = init_prep_;
        widx_ = init_prep_ + nread;
    } else
        buf_.resize(widx_ + len);
}

}
}
