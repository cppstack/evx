#ifndef _CST_EVX_NET_SOCKET_ADDRESS_HPP
#define _CST_EVX_NET_SOCKET_ADDRESS_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <stdexcept>

namespace cst {
namespace evx {
namespace net {

using sockaddr         = struct sockaddr;
using sockaddr_in      = struct sockaddr_in;
using sockaddr_in6     = struct sockaddr_in6;
using sockaddr_storage = struct sockaddr_storage;
using socklen_t        = ::socklen_t;

class socket_address {
public:
    socket_address() noexcept
    { std::memset(&storage_, 0, sizeof(storage_)); }

    socket_address(const std::string& ip, uint16_t port);
    socket_address(const std::string& ip, uint16_t port, int af);

    int family() const noexcept
    { return storage_.ss_family; }

    const sockaddr* addr() const noexcept
    { return (sockaddr*) &storage_; }

    sockaddr* addr() noexcept
    { return (sockaddr*) &storage_; }

    const sockaddr_in* addr_v4() const noexcept
    { return (sockaddr_in*) &storage_; }

    const sockaddr_in6* addr_v6() const noexcept
    { return (sockaddr_in6*) &storage_; }

    socklen_t len() const noexcept
    { return len_; }

    socklen_t* len() noexcept
    { return &len_; }

private:
    void* sin_addr(int af) noexcept
    {
        /* af must be either AF_INET or AF_INET6 */
        if (af == AF_INET)
            return &((sockaddr_in*) &storage_)->sin_addr;
        else
            return &((sockaddr_in6*) &storage_)->sin6_addr;
    }

    uint16_t& sin_port(int af) noexcept
    {
        /* af must be either AF_INET or AF_INET6 */
        return (af == AF_INET) ?
               ((sockaddr_in*) &storage_)->sin_port
             : ((sockaddr_in6*) &storage_)->sin6_port;
    }

    sockaddr_storage storage_;
    socklen_t len_ = 0;
};

class socket_address_error : public std::logic_error {
public:
    socket_address_error(const std::string& ip, uint16_t)
        : logic_error("bad ip address '" + ip + "'")
    { }

    socket_address_error(const std::string& ip, uint16_t, int af)
        : logic_error("bad ip address '" + ip + "' for address family "
                      + std::to_string(af))
    { }
};

}
}
}

#endif
