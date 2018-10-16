#pragma once

#include <string>
#include "generator_presenters_map.hpp"

namespace cuttle {
    namespace generator {
        namespace defaults {
            namespace root {
                std::string presenter_left_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params);
                std::string presenter_right_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params);
                bool presenter_skip(int argi, bool is_func, cuttle::generator_presenter_params_t &params);
            }

            namespace basic {
                std::string presenter_left_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params);
                std::string presenter_right_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params);
                bool presenter_skip(int argi, bool is_func, cuttle::generator_presenter_params_t &params);
            }
        }
    }
}