#pragma once

#include <string>
#include <set>
#include "generator_presenters_map.hpp"

namespace cuttle {
    using merged_up_functions_t = std::set<std::string>;
    struct generator_config_t {
        generator_presenters_map_t presenters_map;
        merged_up_functions_t joined_functions;
    };
}