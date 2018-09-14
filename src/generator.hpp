#pragma once

#include <string>
#include <vector>
#include "call_tree.hpp"

namespace cuttle {
    const std::string GENERATOR_ROOT_FUNCTION_NAME = "";

    struct generator_state_separator_t {
        std::string str;
        bool enabled;
    };

	struct generator_state_t {
        unsigned int index;
        bool include_function_name = true;
        generator_state_separator_t separator;
        std::string output;
	};
}