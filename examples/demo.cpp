#include <cst/evx/evx.h>
#include <unistd.h>

namespace evx = cst::evx;

void stdin_handler(evx::io_watcher& w)
{
    char buf[32];

    if (w.revents() & evx::ev_in) {
        int n = ::read(w.fd(), buf, sizeof(buf));
        if (n > 0)
            ::write(STDOUT_FILENO, buf, n);
        else
            w.disable_read();
    }
}

int main()
{
    evx::event_loop loop;
    evx::io_watcher w(loop, STDIN_FILENO, evx::ev_in, stdin_handler);
    loop.run();
    return 0;
}
