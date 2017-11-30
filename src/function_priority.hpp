#pragma once

#include <vector>
#include <string>
#include "function_id.hpp"

#define FUNCTION_PRIORITY_UNKNOWN 0
#define FUNCTION_PRIORITY_LAST 1
#define FUNCTION_PRIORITY_START 2
#define FUNCTION_PRIORITY_ERROR -1

namespace cuttle {
	using function_priority_t = int;

	using function_priority_elem_t = struct function_priority_elem {
		function_id_t prev;
		function_priority_t prior;
		function_id_t next;
	};

	using function_priorities_t = std::vector<function_priority_elem_t>;
	
	using function_priority_table_t = struct function_priority_table {
		function_priorities_t priors;
	};

	void initialize(function_priority_table_t &func_priors);

	function_priority_t add(
		function_priority_table_t &func_priors, function_id_t func, function_id_t after
	);

	bool operator==(const function_priority_elem_t &left, const function_priority_elem_t &right);
	bool operator==(const function_priority_table_t &left, const function_priority_table_t &right);
}