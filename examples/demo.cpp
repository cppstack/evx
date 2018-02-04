#include <cst/evx/evx.h>

namespace evx = cst::evx;

int main()
{
    evx::event_loop loop;
    loop.run();
    return 0;
}
