#include <cst/evx/evx.h>
#include <cst/evx/net.h>
#include <iostream>
#include <unistd.h>

namespace evx = cst::evx;

class echo_client : public evx::net::tcp_client {
public:
    echo_client(evx::event_loop& loop, const evx::net::socket_address& addr)
        : tcp_client(loop, addr),
          w_stdin_(loop, STDIN_FILENO, evx::ev_in,
                   std::bind(&echo_client::on_stdin_read, this,
                             std::placeholders::_1))
    {
        set_connect_callback(std::bind(&echo_client::on_connect, this,
                                       std::placeholders::_1));

        set_read_callback(std::bind(&echo_client::on_net_read, this,
                          std::placeholders::_1, std::placeholders::_2));
    }

    void on_connect(const evx::net::tcp_connection_ptr& conn)
    {
        conn_ = conn;
    }

    void on_net_read(const evx::net::tcp_connection_ptr&, evx::buffer& buf)
    {
        std::string str;
        while (buf.take(str))
            std::cout << str;
    }

    void on_stdin_read(evx::io_watcher& w)
    {
        char buf[1024];

        if (w.revents() & evx::ev_in) {
            int n = ::read(w.fd(), buf, sizeof(buf));
            if (n > 0) {
                if (conn_)
                    conn_->send(buf, n);
            } else
                w.disable_read();
        }
    }

private:
    evx::io_watcher w_stdin_;
    evx::net::tcp_connection_ptr conn_;
};

int main()
{
    evx::event_loop loop;
    // loop.logger()->set_level(cst::logging::debug);
    echo_client cli(loop, evx::net::socket_address("127.0.0.1", 3033));
    cli.connect();
    loop.run();
    return 0;
}
