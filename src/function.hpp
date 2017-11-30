#pragma once

#include <string>

namespace cuttle {
    enum function_type {
        PREFIX_FUNCTION,
        INFIX_FUNCTION,
        POSTFIX_FUNCTION
    };

    using function_t = struct function {
        enum function_type type;
        int args_number = 0;
    };
}
