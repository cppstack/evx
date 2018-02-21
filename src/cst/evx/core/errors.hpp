#ifndef _CST_EVX_ERRORS_HPP
#define _CST_EVX_ERRORS_HPP

#include <cst/evx/core/logger_ptr.hpp>
#include <cst/logging/logger.hpp>
#include <system_error>

namespace cst {
namespace evx {

[[noreturn]] inline void throw_runtime_error(const char* what_arg,
                                             const logger_ptr& log = nullptr)
{
    const auto& err = std::runtime_error(what_arg);
    LOG_CRIT(log) << err.what();
    throw err;
}

[[noreturn]] inline void throw_system_error(int ev, const char* what_arg,
                                            const logger_ptr& log = nullptr)
{
    const auto& err = std::system_error(ev, std::system_category(), what_arg);
    LOG_CRIT(log) << err.what();
    throw err;
}

}
}

#endif
