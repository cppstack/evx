#ifndef _CST_EVX_SYSTEM_ERROR_HPP
#define _CST_EVX_SYSTEM_ERROR_HPP

#include <cst/logging/logger.hpp>
#include <system_error>

namespace cst {
namespace evx {

class system_error : public std::system_error {
public:
    system_error(int ev)
        : std::system_error(ev, std::system_category())
    { }

    system_error(int ev, const std::shared_ptr<logging::logger>& log)
        : system_error(ev)
    {
        LOG_CRIT(log) << std::system_category().message(ev);
    }
};

}
}

#endif
