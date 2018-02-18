#include <cst/evx/evx.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

namespace evx = cst::evx;

void stdin_handler(evx::io_watcher& w)
{
    char buf[32];

    if (w.revents() & evx::ev_in) {
        int n = ::read(w.fd(), buf, sizeof(buf));
        if (n > 0)
            ::write(STDOUT_FILENO, buf, n);
        else {
            std::cerr << (n == 0 ? "end of file" : "read error") << std::endl;
            w.disable_read();
        }
    }
}

void timer_handler(evx::timer_watcher& w)
{
    using std::chrono::system_clock;

    static int times = 0;;

    std::time_t now_c = system_clock::to_time_t(system_clock::now());
    std::cout << std::put_time(std::localtime(&now_c), "%F %T") << std::endl;

    if (++times >= 5)
        w.disable();
}

int main()
{
    evx::event_loop loop;
    evx::io_watcher w1(loop, STDIN_FILENO, evx::ev_in, stdin_handler);
    evx::timer_watcher w2(loop, std::chrono::system_clock::now() + std::chrono::seconds(3), std::chrono::seconds(1), timer_handler);
    evx::timer_watcher w3(loop, std::chrono::seconds(3), std::chrono::seconds(1), timer_handler);
    loop.run();
    return 0;
}
