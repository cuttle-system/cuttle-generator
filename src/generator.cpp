#include <string>
#include "generator_methods.hpp"
#include "generate_error.hpp"
#include "presenter.hpp"

using namespace cuttle;

std::string present_function_name(generator_state_t &state, const std::string &function_name) {
    if (state.include_function_name) {
        return function_name + state.separator.str;
    }
    return "";
}

void generate_inner(const tokenizer_config_t &tokenizer_config, const generator_config_t &generator_config,
                    const context_t &context, const values_t &values,
                    const call_tree_t &tree, generator_state_t &state) {

	auto index = state.index;
	auto args_indexes = tree.src[index];
    state.include_function_name = true;
    const std::string &function_name = (values.size() <= index) ? GENERATOR_ROOT_FUNCTION_NAME : values[index].value;

    if (values.size() <= index) {
        state.include_function_name = false;
    }

    std::string separator = " ";

	if (generator_config.joined_functions.count(function_name)) {
        state.include_function_name = false;
        separator = state.separator.str;
        state.separator.enabled = false;
	} else {
        if (state.separator.enabled) {
            separator = state.separator.str;
            state.separator.enabled = false;
        } else {
            state.separator.str = separator;
        }
    }

	function_t function;

	if (args_indexes.empty()) {
        function = {function_type::prefix, 0};
    } else if (function_name == GENERATOR_ROOT_FUNCTION_NAME) {
	    function = {function_type::prefix, (unsigned int) args_indexes.size()};
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
		state.output += present_function_name(state, function_name);
	}

	for (tree_src_element_t i = 0; i < args_indexes.size(); ++i) {
		if (function.type == function_type::infix && i == function.args_number / 2) {
			state.output += present_function_name(state, function_name);
		}

		tree_src_element_t argi = args_indexes[i];
		if (values[argi].type == value_type::func_name) {
			generator_state_t child_state = state;
			child_state.index = argi;
			child_state.output = "";
            generate_inner(tokenizer_config, generator_config, context, values, tree, child_state);
			state.output += child_state.output + separator;
		} else {
			state.output += present(tokenizer_config, values[argi]) + separator;
		}
	}

	if (function.type == function_type::postfix && state.include_function_name) {
		state.output += function_name;
	} else {
		state.output.pop_back();
	}
}

void cuttle::generate(const tokenizer_config_t &tokenizer_config, const generator_config_t &generator_config, const context_t &context,
					  const values_t &values, const call_tree_t &tree, generator_state_t &state) {

    if (tree.src.empty() || (tree.src.size() == 1 && tree.src[0].empty())) {
        return;
    }

    state.index = (unsigned int) tree.src.size() - 1;
    state.separator = {"\n", true};
    generate_inner(tokenizer_config, generator_config, context, values, tree, state);
}

void cuttle::initialize(cuttle::generator_state_t &state) {
	state.index = 0;
	state.output = "";
}