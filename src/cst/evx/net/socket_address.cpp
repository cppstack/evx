#include <cst/evx/net/socket_address.hpp>
#include <initializer_list>
#include "os/inet.hpp"

namespace cst {
namespace evx {
namespace net {

using namespace evx::os;

socket_address::socket_address(const std::string& ip, uint16_t port)
    : socket_address()
{
    for (int af : { AF_INET, AF_INET6 }) {
        if (Inet_pton(af, ip.c_str(), sin_addr(af))) {
            sin_port(af) = htons(port);
            storage_.ss_family = af;
            len_ = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
            return;
        }
    }

    throw socket_address_error(ip, port);
}

socket_address::socket_address(const std::string& ip, uint16_t port, int af)
    : socket_address()
{
    if (af != AF_INET && af != AF_INET6)
        throw std::logic_error(std::make_error_code(std::errc::address_family_not_supported).message());

    if (Inet_pton(af, ip.c_str(), sin_addr(af))) {
        sin_port(af) = htons(port);
        storage_.ss_family = af;
        len_ = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
        return;
    }

    throw socket_address_error(ip, port, af);
}

}
}
}
