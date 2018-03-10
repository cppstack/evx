#include <cst/evx/evx.h>
#include <cst/evx/net.h>
#include <iostream>

namespace evx = cst::evx;

int main()
{
    evx::event_loop loop;
    loop.logger()->set_level(cst::logging::debug);
    evx::net::tcp_client cli(loop, evx::net::socket_address("127.0.0.1", 3033));
    cli.connect();
    loop.run();
    return 0;
}
