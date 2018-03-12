#include <cst/evx/core/buffer.hpp>
#include "os/uio.hpp"

namespace cst {
namespace evx {

std::size_t buffer::read_fd(int fd)
{
    char extrabuf[16384];
    const std::size_t writable_ = writable();

    struct ::iovec iov[2];
    iov[0].iov_base = wbegin_();
    iov[0].iov_len  = writable_;
    iov[1].iov_base = extrabuf;
    iov[1].iov_len  = sizeof extrabuf;

    std::size_t nr = os::Readv(fd, iov, 2);

    if (nr <= writable_)
        widx_ += nr;
    else {
        widx_ = buf_.size();
        append(extrabuf, nr - writable_);
    }

    return nr;
}

void buffer::grow_(std::size_t len)
{
    const std::size_t nread = readable();

    if (len <= prependable() - init_prep_ + writable()) {
        std::memmove(pbegin_(), rbegin_(), nread);
        ridx_ = init_prep_;
        widx_ = init_prep_ + nread;
    } else
        buf_.resize(widx_ + len);
}

}
}
