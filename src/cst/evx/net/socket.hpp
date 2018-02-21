#ifndef _CST_EVX_NET_SOCKET_HPP
#define _CST_EVX_NET_SOCKET_HPP

#include <cst/evx/net/socket_address.hpp>
#include "core/file_descriptor.hpp"
#include "net/ssl.hpp"
#include "os/socket.hpp"
#include "os/inet.hpp"
#include <memory>

namespace cst {
namespace evx {
namespace net {

class socket {
public:
    socket(const socket&) = delete;
    socket& operator=(const socket&) = delete;

    socket() = default;

    socket(int domain, int type, int proto)
        : family_(domain), type_(type), proto_(proto),
          fd_(os::Socket(family_, type_, proto_))
    { }

    socket(int domain, int type, int proto, file_descriptor&& fd)
        : family_(domain), type_(type), proto_(proto), fd_(std::move(fd))
    { }

    socket(socket&&) = default;
    socket& operator=(socket&&) = default;

    static socket tcp_bind(const socket_address& addr);

    static socket tcp_bind(const std::string& host, const std::string& serv);

    static socket tcp_bind(const std::string& host, uint16_t port)
    {
        try {
            return tcp_bind(socket_address(host, port));
        } catch (const socket_address_error&) { }

        return tcp_bind(host, std::to_string(port));
    }

    static socket tcp_connect(const socket_address& addr);

    static socket tcp_connect(const std::string& host, const std::string& serv);

    static socket tcp_connect(const std::string& host, uint16_t port)
    {
        try {
            return tcp_connect(socket_address(host, port));
        } catch (const socket_address_error&) { }

        return tcp_connect(host, std::to_string(port));
    }

    int fd() const noexcept
    { return fd_; }

    void connect(const std::string& host, uint16_t port);

    void connect(const socket_address& address)
    {
        os::Connect(fd_, address.addr(), address.len());
    }

    void connect(const sockaddr* addr, socklen_t len)
    {
        os::Connect(fd_, addr, len);
    }

    void ssl_connect()
    {
        ssl_.connect(fd_);
    }

    void bind(const socket_address& address)
    {
        os::Bind(fd_, address.addr(), address.len());
    }

    void bind(const sockaddr* addr, socklen_t len)
    {
        os::Bind(fd_, addr, len);
    }

    void listen(int backlog)
    {
        os::Listen(fd_, backlog);
    }

    socket accept(socket_address& address)
    {
        int connfd = os::Accept(fd_, address.addr(), address.len());
        return socket(family_, type_, proto_, file_descriptor(connfd));
    }

    socket accept()
    {
        int connfd = os::Accept(fd_, nullptr, nullptr);
        return socket(family_, type_, proto_, file_descriptor(connfd));
    }

    std::size_t peek(void* buf, std::size_t len, int flags = 0)
    {
        if (ssl_)
            return os::SSL_Peek(ssl_, buf, len);
        else
            return os::Recv(fd_, buf, len, MSG_PEEK | flags);
    }

    std::size_t recv(void* buf, std::size_t len, int flags = 0)
    {
        if (ssl_)
            return os::SSL_Read(ssl_, buf, len);
        else
            return os::Recv(fd_, buf, len, flags);
    }

    std::size_t recvn(void* buf, std::size_t len, int flags = 0)
    {
        if (ssl_)
            return os::SSL_Readn(ssl_, buf, len);
        else
            return os::Recvn(fd_, buf, len, flags);
    }

    std::size_t recv_until(void* buf, std::size_t len, char delim, int flags = 0)
    {
        std::size_t n = peek(buf, len, flags);

        char* m = (char*) std::memchr(buf, delim, n);
        if (m != nullptr)
            n = m - (char*) buf + 1;

        return recv(buf, n, flags);
    }

    std::size_t send(const void* buf, std::size_t len, int flags = 0)
    {
        if (ssl_)
            return os::SSL_Write(ssl_, buf, len);
        else
            return os::Send(fd_, buf, len, flags);
    }

    std::size_t sendn(const void* buf, std::size_t len, int flags = 0)
    {
        if (ssl_)
            return os::SSL_Writen(ssl_, buf, len);
        else
            return os::Sendn(fd_, buf, len, flags);
    }

    std::size_t sends(const char* str, int flags = 0)
    {
        std::size_t len = std::strlen(str);

        if (ssl_)
            return os::SSL_Write(ssl_, str, len);
        else
            return os::Send(fd_, str, len, flags);
    }

    bool verify_certificate() const;

    void set_opt(int level, int opt, const void* val, socklen_t len)
    {
        os::Setsockopt(fd_, level, opt, val, len);
    }

    void get_opt(int level, int opt, void* val, socklen_t* len) const
    {
        os::Getsockopt(fd_, level, opt, val, len);
    }

    void reinit()
    {
        close();
        fd_.reset(os::Socket(family_, type_, proto_));
    }

    void close() noexcept
    {
        ssl_.shutdown();
        fd_.close();
    }

private:
    using addrinfo_uptr = std::unique_ptr<addrinfo, decltype(::freeaddrinfo)*>;

    static addrinfo_uptr getaddrinfo(const std::string& host,
                                     const std::string& serv,
                                     const addrinfo& hints);

    int family_ = 0;
    int type_   = 0;
    int proto_  = 0;
    file_descriptor fd_;
    ssl ssl_;
};

}
}
}

#endif
