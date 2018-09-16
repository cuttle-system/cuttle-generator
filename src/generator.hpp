#pragma once

#include <string>
#include <vector>
#include "call_tree.hpp"
#include "generator_presenters.hpp"
#include "function.hpp"

namespace cuttle {
    const std::string GENERATOR_ROOT_FUNCTION_NAME = "";

	struct generator_state_t {
        unsigned int index;
        int arg_index;

        bool joined_function;
        bool include_function_name;
        generator_presenters_t presenters;

        tree_src_elements_t args_indexes;

        function_t function;
        std::string function_name;

        std::string output;
	};
}