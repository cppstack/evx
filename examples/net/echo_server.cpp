#include <cst/evx/evx.h>
#include <cst/evx/net.h>

namespace evx = cst::evx;

class echo_server : public evx::net::tcp_server {
public:
    echo_server(evx::event_loop& loop, const evx::net::socket_address& addr)
        : tcp_server(loop, addr)
    {
        set_read_callback(std::bind(&echo_server::on_read, this,
                          std::placeholders::_1, std::placeholders::_2));
    }

    void on_read(const evx::net::tcp_connection_ptr& conn, evx::buffer& buf)
    {
        char b[8192];
        std::size_t n;
        while ((n = buf.take(b, sizeof(b))))
            conn->send(b, n);
    }
};

int main()
{
    evx::event_loop loop;
    // loop.logger()->set_level(cst::logging::debug);
    echo_server serv(loop, evx::net::socket_address("::", 3033));
    serv.start();
    loop.run();
    return 0;
}
