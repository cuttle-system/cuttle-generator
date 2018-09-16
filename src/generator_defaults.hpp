#pragma once

#include <string>

namespace cuttle {
    namespace generator {
        namespace defaults {
            namespace root {
                std::string presenter_left_separator(int argi, bool is_func) {
                    if (argi == 0) return "";
                    return "\n";
                }

                std::string presenter_right_separator(int argi, bool is_func) {
                    return "";
                }

                bool presenter_skip(int argi, bool is_func) {
                    return is_func;
                }
            }

            namespace basic {
                std::string presenter_left_separator(int argi, bool is_func) {
                    if (argi == 0) return "";
                    return " ";
                }

                std::string presenter_right_separator(int argi, bool is_func) {
                    return "";
                }

                bool presenter_skip(int argi, bool is_func) {
                    return false;
                }
            }
        }
    }
}