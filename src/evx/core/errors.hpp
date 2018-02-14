#ifndef _CST_EVX_ERRORS_HPP
#define _CST_EVX_ERRORS_HPP

#include <cst/logging/logger.hpp>
#include <system_error>

namespace cst {
namespace evx {

[[noreturn]] inline void throw_system_error(int ev, const logger_ptr& log, const char* what_arg)
{
    const auto& err = std::system_error(ev, std::system_category(), what_arg);
    LOG_CRIT(log) << err.what();
    throw err;
}

}
}

#endif
