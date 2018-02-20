#ifndef _CST_EVX_FILE_DESCRIPTOR_HPP
#define _CST_EVX_FILE_DESCRIPTOR_HPP

#include <utility>
#include <unistd.h>

namespace cst {
namespace evx {

class file_descriptor {
public:
    file_descriptor(const file_descriptor&) = delete;
    file_descriptor& operator=(const file_descriptor&) = delete;

    file_descriptor() = default;

    explicit file_descriptor(int fd, bool cldt = true) noexcept
        : fd_(fd), cldt_(cldt)
    { }

    operator int() const noexcept { return fd_; }

    file_descriptor(file_descriptor&& that) noexcept
        : fd_  (std::exchange(that.fd_, -1)),
          cldt_(std::exchange(that.cldt_, false))
    { }

    file_descriptor& operator=(file_descriptor&& that) noexcept
    {
        if (this != &that) {
            if (cldt_)
                close();

            fd_   = std::exchange(that.fd_, -1);
            cldt_ = std::exchange(that.cldt_, false);
        }

        return *this;
    }

    void reset(int fd, bool cldt = true) noexcept
    {
        if (cldt_)
            close();

        fd_   = fd;
        cldt_ = cldt;
    }

    void close() noexcept
    {
        if (fd_ != -1) {
            ::close(fd_);
            fd_   = -1;
            cldt_ = false;
        }
    }

    ~file_descriptor()
    {
        if (cldt_)
            close();
    }

private:
    int  fd_   = -1;
    bool cldt_ = false;
};

}
}

#endif
