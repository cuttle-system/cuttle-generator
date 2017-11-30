#include <string>
#include "generator_methods.hpp"
#include "generate_error.hpp"
#include "function.hpp"

void cuttle::generate(const cuttle::context_t & context, int& index, const cuttle::values_t & values, const cuttle::call_tree & tree, cuttle::generator_state_t& state) {
	using namespace cuttle;

	while(index < tree.src.size() && values[index].type != value_type::func_name && !state.used[index]) ++index;

	if (index >= tree.src.size()) {
		return;
	}

	state.used[index] = true;

	auto args_indexes = tree.src[index];
	const std::string& function_name = values[index].value;
	function_t function;

	if (args_indexes.size() == 0) {
		function = {function_type::prefix, 0};
	} else if (context.funcname_to_id.find(function_name) != context.funcname_to_id.end()) {
		function_id_t function_id = context.funcname_to_id.at(function_name);
		function = context.funcs[function_id];
		
		if (args_indexes.size() != function.args_number) {
			throw generate_error("'" + function_name + "' recieves " + std::to_string(function.args_number) + " args");
		}
	} else {
		throw generate_error("'" + function_name + "' not found");
	}

	if (function.type == function_type::prefix) {
		state.output += function_name + " ";
	}

	for (int i = 0; i < args_indexes.size(); ++i) {
		if (function.type == function_type::infix && i == function.args_number / 2) {
			state.output += function_name + " ";
		}

		auto argi = args_indexes[i];
		if (values[argi].type == value_type::func_name) {
			generator_state_t child_state = state;
			child_state.output = "";
			generate(context, args_indexes[i], values, tree, child_state);
			state.output += child_state.output + " ";
		} else {
			state.output += values[argi].value + " ";
			state.used[argi] = true;
		}
	}

	if (function.type == function_type::postfix) {
		state.output += function_name;
	}
	else {
		state.output.pop_back();
	}
}

void cuttle::initialize(cuttle::generator_state_t & state, int size) {
	state.used.assign(size, false);
	state.output = "";
}