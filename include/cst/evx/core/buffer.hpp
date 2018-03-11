#ifndef _CST_EVX_BUFFER_HPP
#define _CST_EVX_BUFFER_HPP

#include <vector>

namespace cst {
namespace evx {

class buffer {
public:
    explicit buffer(std::size_t size = def_init_size_)
        : buf_(size > init_prep_ ? size : init_prep_),
          ridx_(init_prep_), widx_(init_prep_)
    { }

    const char* rbegin() const noexcept
    { return buf_.data() + ridx_; }

    std::size_t readable() const noexcept
    { return widx_ - ridx_; }

    std::size_t writable() const noexcept
    { return buf_.size() - widx_; }

    std::size_t prependable() const noexcept
    { return ridx_; }

    void append(const char* data, std::size_t len)
    {
        ensure_writable_(len);
        std::copy(data, data + len, wbegin_());
        widx_ += len;
    }

    void take(std::size_t len) noexcept
    {
       if (len < readable())
           ridx_ += len;
       else
           take_all();
    }

    void take_all() noexcept
    {
        ridx_ = widx_ = init_prep_;
    }

private:
    char* pbegin_() noexcept
    { return buf_.data() + init_prep_; }

    char* rbegin_() noexcept
    { return buf_.data() + ridx_; }

    char* wbegin_() noexcept
    { return buf_.data() + widx_; }

    void ensure_writable_(std::size_t len)
    {
        if (writable() < len)
            grow_(len);
    }

    void grow_(std::size_t len);

    static constexpr std::size_t init_prep_ = 8;
    static constexpr std::size_t def_init_size_ = 1024;

    std::vector<char> buf_;
    std::size_t ridx_;
    std::size_t widx_;
};

}
}

#endif
