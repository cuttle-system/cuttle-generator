#pragma once

#include <string>
#include <unordered_map>
#include <set>

namespace cuttle {
//    using separators_map_t = std::unordered_map<std::string, function_type_here>;
    using merged_up_functions_t = std::set<std::string>;
    struct generator_config_t {
//        separators_map_t separators;
        merged_up_functions_t joined_functions;
    };
}