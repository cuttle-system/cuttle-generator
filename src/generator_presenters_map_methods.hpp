#pragma once

#include "generator_presenters_map.hpp"

namespace cuttle {
    void add(generator_presenters_map_t &presenters_map,
             const std::string &function_name,
             generator_separator_func_t *left_separator = nullptr,
             generator_separator_func_t *right_separator = nullptr,
             generator_skip_func_t *skip = nullptr);
}