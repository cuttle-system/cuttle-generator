#include <string>
#include "generator_methods.hpp"
#include "generate_error.hpp"
#include "presenter.hpp"

using namespace cuttle;

void generate_inner(const tokenizer_config_t &tokenizer_config, const context_t &context, const values_t &values,
					  const call_tree_t &tree, generator_state_t &state) {

	auto index = state.index;

	auto args_indexes = tree.src[index];
	const std::string& function_name = values[index].value;
	function_t function;

	if (args_indexes.empty()) {
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

	for (unsigned int i = 0; i < args_indexes.size(); ++i) {
		if (function.type == function_type::infix && i == function.args_number / 2) {
			state.output += function_name + " ";
		}

		unsigned int argi = args_indexes[i];
		if (values[argi].type == value_type::func_name) {
			generator_state_t child_state = state;
			child_state.index = argi;
			child_state.output = "";
			generate_inner(tokenizer_config, context, values, tree, child_state);
			state.output += child_state.output + " ";
		} else {
			state.output += present(tokenizer_config, values[argi]) + " ";
		}
	}

	if (function.type == function_type::postfix) {
		state.output += function_name;
	}
	else {
		state.output.pop_back();
	}
}

void cuttle::generate(const tokenizer_config_t &tokenizer_config, const context_t &context, const values_t &values,
              const call_tree_t &tree, generator_state_t &state) {

    for (auto root_index : tree.src.back()) {
        state.index = root_index;
        generate_inner(tokenizer_config, context, values, tree, state);
    }
}

void cuttle::initialize(cuttle::generator_state_t &state) {
	state.index = 0;
	state.output = "";
}