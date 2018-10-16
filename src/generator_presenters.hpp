#pragma once

#include <string>
#include <unordered_map>

namespace cuttle {
    struct generator_presenter_params_t {
        bool hide_function_name;
        std::string before_func, after_func;

        std::unordered_map<int, bool> skip;
        std::unordered_map<int, std::string> before, after;
    };

    using generator_separator_func_t = std::string(int, bool, generator_presenter_params_t&);
    using generator_skip_func_t = bool(int, bool, generator_presenter_params_t&);
    struct generator_presenters_t {
        generator_separator_func_t *left_separator;
        generator_separator_func_t *right_separator;
        generator_skip_func_t *skip;
    };
}