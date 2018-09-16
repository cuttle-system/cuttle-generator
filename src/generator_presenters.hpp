#pragma once

#include <string>

namespace cuttle {
    using generator_separator_func_t = std::string(int, bool);
    using generator_skip_func_t = bool(int, bool);
    struct generator_presenters_t {
        generator_separator_func_t *left_separator;
        generator_separator_func_t *right_separator;
        generator_skip_func_t *skip;
    };
}