#ifndef _CST_EVX_ERRORS_HPP
#define _CST_EVX_ERRORS_HPP

#include <cst/logging/logger.hpp>
#include <system_error>

/* TODO: use logging::logger_ptr */

namespace cst {
namespace evx {

[[noreturn]] inline void throw_system_error(int ev, const std::shared_ptr<logging::logger>& log, const char* what_arg)
{
    const auto& err = std::system_error(ev, std::system_category(), what_arg);
    LOG_CRIT(log) << err.what();
    throw err;
}

}
}

#endif
