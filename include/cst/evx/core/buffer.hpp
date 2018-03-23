#ifndef _CST_EVX_BUFFER_HPP
#define _CST_EVX_BUFFER_HPP

#include <vector>
#include <cstring>
#include <system_error>

namespace cst {
namespace evx {

class buffer {
public:
    explicit buffer(size_t size = def_init_size_)
        : buf_(size > init_prep_ ? size : init_prep_),
          ridx_(init_prep_), widx_(init_prep_)
    { }

    const char* rbegin() const noexcept
    { return buf_.data() + ridx_; }

    size_t readable() const noexcept
    { return widx_ - ridx_; }

    size_t writable() const noexcept
    { return buf_.size() - widx_; }

    size_t prependable() const noexcept
    { return ridx_; }

    ssize_t read_fd(int fd, std::error_code* ec = nullptr);

    void append(const char* data, size_t len)
    {
        ensure_writable_(len);
        std::memmove(wbegin_(), data, len);
        widx_ += len;
    }

    size_t take(std::string& str)
    {
        str = std::string(rbegin(), readable());
        return take_all();
    }

    size_t take(void* buf, size_t len) noexcept
    {
        len = std::min(readable(), len);
        std::memmove(buf, rbegin(), len);
        return take(len);
    }

    size_t take(size_t len) noexcept
    {
       if (len < readable()) {
           ridx_ += len;
           return len;
       } else
           return take_all();
    }

    size_t take_all() noexcept
    {
        const auto len = readable();
        ridx_ = widx_ = init_prep_;
        return len;
    }

private:
    char* pbegin_() noexcept
    { return buf_.data() + init_prep_; }

    char* rbegin_() noexcept
    { return buf_.data() + ridx_; }

    char* wbegin_() noexcept
    { return buf_.data() + widx_; }

    void ensure_writable_(size_t len)
    {
        if (writable() < len)
            grow_(len);
    }

    void grow_(size_t len);

    static constexpr size_t init_prep_ = 8;
    static constexpr size_t def_init_size_ = 1024;

    std::vector<char> buf_;
    size_t ridx_;
    size_t widx_;
};

}
}

#endif
