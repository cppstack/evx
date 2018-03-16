#include <cst/evx/core/buffer.hpp>
#include <cst/lnx/os/uio.hpp>

namespace cst {
namespace evx {

ssize_t buffer::read_fd(int fd, int* err)
{
    char extrabuf[16384];
    const size_t writable_ = writable();

    lnx::iovec iov[2];
    iov[0].iov_base = wbegin_();
    iov[0].iov_len  = writable_;
    iov[1].iov_base = extrabuf;
    iov[1].iov_len  = sizeof extrabuf;

    ssize_t nr = lnx::Readv(fd, iov, 2, err);

    if (nr >= 0 && (size_t) nr <= writable_)
        widx_ += nr;
    else if ((size_t) nr > writable_) {
        widx_ = buf_.size();
        append(extrabuf, nr - writable_);
    }

    return nr;
}

void buffer::grow_(size_t len)
{
    const size_t nread = readable();

    if (len <= prependable() - init_prep_ + writable()) {
        std::memmove(pbegin_(), rbegin_(), nread);
        ridx_ = init_prep_;
        widx_ = init_prep_ + nread;
    } else
        buf_.resize(widx_ + len);
}

}
}
