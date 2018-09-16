#pragma once

#include <string>
#include <map>
#include <set>
#include "generator_presenters.hpp"

namespace cuttle {
    using generator_presenters_map_t = std::map<std::string, generator_presenters_t>;
    using merged_up_functions_t = std::set<std::string>;
    struct generator_config_t {
        generator_presenters_map_t presenters_map;
        merged_up_functions_t joined_functions;
    };
}