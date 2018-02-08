#ifndef _CST_EVX_COMMON_HPP
#define _CST_EVX_COMMON_HPP

#include <memory>

namespace cst {

namespace logging {

class logger;
using logger_ptr = std::shared_ptr<logger>;

}

namespace evx {

using logging::logger_ptr;

}

}

#endif
