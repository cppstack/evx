#include "evx/net/socket.hpp"
#include <sstream>

namespace cst {
namespace evx {
namespace net {

socket socket::tcp_bind(const socket_address& addr)
{
    const int on = 1;
    socket sock(addr.family(), SOCK_STREAM, 0);
    sock.set_opt(SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    sock.bind(addr.addr(), addr.len());
    return sock;
}

socket socket::tcp_bind(const std::string& host, const std::string& serv)
{
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    const int on = 1;

    addrinfo_uptr res = getaddrinfo(host, serv, hints);

    for (const addrinfo* rp = res.get(); rp != nullptr; rp = rp->ai_next) {
        socket sock(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        sock.set_opt(SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        try {
            sock.bind(rp->ai_addr, rp->ai_addrlen);
            return sock;
        } catch (const std::system_error&) { }

        sock.close();
    }

    std::ostringstream oss("tcp_bind() error for ", std::ios_base::ate);
    oss << host << ":" << serv;
    throw std::runtime_error(oss.str());
}

socket socket::tcp_connect(const socket_address& addr)
{
    socket sock(addr.family(), SOCK_STREAM, 0);
    sock.connect(addr.addr(), addr.len());
    return sock;
}

socket socket::tcp_connect(const std::string& host, const std::string& serv)
{
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo_uptr res = getaddrinfo(host, serv, hints);

    for (const addrinfo* rp = res.get(); rp != nullptr; rp = rp->ai_next) {
        socket sock(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        try {
            sock.connect(rp->ai_addr, rp->ai_addrlen);
            return sock;
        } catch (const std::system_error&) { }

        sock.close();
    }

    std::ostringstream oss("tcp_connect() error for ", std::ios_base::ate);
    oss << host << ":" << serv;
    throw std::runtime_error(oss.str());
}

void socket::connect(const std::string& host, uint16_t port)
{
    try {
        /* try if host is an ip address */
        socket_address addr(host, port, family_);
        connect(addr);
        return;
    } catch (const socket_address_error&) { }

    {
        addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = family_;
        hints.ai_socktype = type_;
        hints.ai_protocol = proto_;

        addrinfo_uptr res = getaddrinfo(host, std::to_string(port), hints);

        for (const addrinfo* rp = res.get(); rp != nullptr; rp = rp->ai_next) {
            try {
                connect(rp->ai_addr, rp->ai_addrlen);
                return;
            } catch (const std::system_error&) { }

            reinit();
        }

        std::ostringstream oss("connect() error for ", std::ios_base::ate);
        oss << host << ":" << port;
        throw std::runtime_error(oss.str());
    }
}

bool socket::verify_certificate() const
{
    std::unique_ptr<X509, decltype(::X509_free)*>
    cert(::SSL_get_peer_certificate(ssl_), ::X509_free);
    if (!cert)
        return false;

    if (::SSL_get_verify_result(ssl_) != X509_V_OK)
        return false;

    return true;
}

socket::addrinfo_uptr socket::getaddrinfo(const std::string& host,
                                          const std::string& serv,
                                          const addrinfo& hints)
{
    addrinfo* res = nullptr;

    const char* node = host.c_str();
    if (*node == '\0')
        node = nullptr;

    os::Getaddrinfo(node, serv.c_str(), &hints, &res);
    return addrinfo_uptr(res, ::freeaddrinfo);
}

}
}
}
