#include <cst/evx/evx.h>
#include <cst/evx/net.h>
#include <iostream>

namespace evx = cst::evx;

int main()
{
    evx::event_loop loop;
    loop.logger()->set_level(cst::logging::debug);
    evx::net::tcp_server serv(loop, evx::net::socket_address("::", 3033));
    serv.start();
    loop.run();
    return 0;
}
