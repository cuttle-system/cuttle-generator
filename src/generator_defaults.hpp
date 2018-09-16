#pragma once

#include <string>

namespace cuttle {
    namespace generator {
        namespace defaults {
            namespace root {
                std::string presenter_left_separator(int argi, bool is_func);
                std::string presenter_right_separator(int argi, bool is_func);
                bool presenter_skip(int argi, bool is_func);
            }

            namespace basic {
                std::string presenter_left_separator(int argi, bool is_func);
                std::string presenter_right_separator(int argi, bool is_func);
                bool presenter_skip(int argi, bool is_func);
            }
        }
    }
}