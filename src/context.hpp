#pragma once

#include <string>
#include <unordered_map>
#include "function.hpp"
#include "function_priority.hpp"
#include "function_id.hpp"

namespace cuttle {
	using funcname_to_id_t = std::unordered_map<std::string, function_id_t>;
	using funcs_t = std::vector<function_t>;

    typedef struct context {
		funcname_to_id_t funcname_to_id;
		funcs_t funcs;
		function_priority_table_t funcs_prior;
    } context_t;
	
	void initialize(context_t &context);
	void initialize(funcs_t &funcs);

	void add(context_t &context, std::string name, function_t func, function_id_t after);
	function_id_t add(funcs_t &funcs, funcname_to_id_t &funcname_to_id, std::string name, function_t func);
}