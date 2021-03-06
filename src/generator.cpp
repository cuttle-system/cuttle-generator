#include <string>
#include "generator_methods.hpp"
#include "generate_error.hpp"
#include "generator_presenters_methods.hpp"
#include "presenter.hpp"
#include "translator.hpp"

#include "generator_defaults.hpp"

using namespace cuttle;

std::string present_function_name(generator_state_t &state) {
    if (state.include_function_name) {
        return present(state, state.function_name, state.arg_index, true);
    }
    return "";
}

void joined_function_check(generator_state_t &state, const generator_config_t &generator_config, bool override) {
    if (generator_config.joined_functions.count(state.function_name) || state.function_name == CUTTLE_MERGE_WITH_PARENT_FUNC) {
        state.joined_function = true;
        state.include_function_name = false;
    } else {
        state.joined_function = false;
        state.arg_index = 0;
        if (generator_config.presenters_map.count(state.function_name)) {
            state.presenters = generator_config.presenters_map.at(state.function_name);
            if (generator_config.presenters_params.count(state.function_name)) {
                state.presenter_params = generator_config.presenters_params.at(state.function_name);
            } else {
                state.presenter_params = {};
            }
        } else {
            if (!override) {
                state.presenters = generator_presenters_t{
                        generator::defaults::basic::presenter_left_separator,
                        generator::defaults::basic::presenter_right_separator,
                        generator::defaults::basic::presenter_skip};
                state.presenter_params = {};
            }
        }
    }
}

void set_function(const context_t &context, generator_state_t &state) {
    if (state.args_indexes.empty()) {
        state.function = {function_type::prefix, 0};
    } else if (state.function_name == GENERATOR_ROOT_FUNCTION_NAME || state.joined_function) {
        state.function = {function_type::prefix, (unsigned int) state.args_indexes.size()};
    } else if (context.funcname_to_id.find(state.function_name) != context.funcname_to_id.end()) {
        function_id_t function_id = context.funcname_to_id.at(state.function_name);
        state.function = context.funcs[function_id];

        if (state.args_indexes.size() != state.function.args_number) {
//            throw generate_error("'" + state.function_name + "' receives " + std::to_string(state.function.args_number) + " args");
        }
    } else {
        throw generate_error("'" + state.function_name + "' not found");
    }
}

void generate_inner(const tokenizer_config_t &tokenizer_config, const generator_config_t &generator_config,
                    const context_t &context, const values_t &values,
                    const call_tree_t &tree, generator_state_t &state, bool override = false);

void generate_child(unsigned int argi,
        const tokenizer_config_t &tokenizer_config,
        const generator_config_t &generator_config,
        const context_t &context, const values_t &values, const call_tree_t &tree,
        generator_state_t &state) {

    generator_state_t child_state;
    child_state.index = argi;
    child_state.arg_index = state.arg_index;
    child_state.presenters = state.presenters;
    child_state.presenter_params = state.presenter_params;
    generate_inner(tokenizer_config, generator_config, context, values, tree, child_state);
    if (child_state.joined_function) {
        state.output += child_state.output;
        state.arg_index = child_state.arg_index;
    } else {
        state.output += present(state, child_state.output, state.arg_index);
        ++state.arg_index;
    }
}

void iterate_through_args(
        const tokenizer_config_t &tokenizer_config,
        const generator_config_t &generator_config,
        const context_t &context, const values_t &values, const call_tree_t &tree,
        generator_state_t &state) {

    for (tree_src_element_t i = 0; i < state.args_indexes.size(); ++i) {
        if (state.function.type == function_type::postinfix && i == 1) {
            state.output += present_function_name(state);
        }
        if (state.function.type == function_type::infix && i == state.args_indexes.size() / 2) {
            state.output += present_function_name(state);
        }

        tree_src_element_t argi = state.args_indexes[i];
        if (argi == CALL_TREE_SRC_NIL) {
            ++state.arg_index;
        } else if (values[argi].type == value_type::func_name) {
            generate_child(argi, tokenizer_config, generator_config, context, values, tree, state);
        } else {
            state.output += present(state, present(tokenizer_config, values[argi]), state.arg_index);
            ++state.arg_index;
        }
    }
}

void generate_inner(const tokenizer_config_t &tokenizer_config, const generator_config_t &generator_config,
                    const context_t &context, const values_t &values,
                    const call_tree_t &tree, generator_state_t &state, bool override) {

	state.args_indexes = tree.src[state.index];
    state.include_function_name = true;
    state.function_name = (values.size() <= state.index) ? GENERATOR_ROOT_FUNCTION_NAME : values[state.index].value;

    if (values.size() <= state.index) {
        state.include_function_name = false;
    }

    joined_function_check(state, generator_config, override);
    set_function(context, state);

	if (state.function.type == function_type::prefix || state.function.type == function_type::postprefix) {
		state.output += present_function_name(state);
	}

	iterate_through_args(tokenizer_config, generator_config, context, values, tree, state);

	if (state.function.type == function_type::postfix) {
		state.output += present_function_name(state);
	}
}

void cuttle::generate(const tokenizer_config_t &tokenizer_config, const generator_config_t &generator_config, const context_t &context,
					  const values_t &values, const call_tree_t &tree, generator_state_t &state) {

    if (tree.src.empty() || (tree.src.size() == 1 && tree.src[0].empty())) {
        return;
    }

    state.index = (unsigned int) tree.src.size() - 1;
    state.presenters = generator_presenters_t{
            generator::defaults::root::presenter_left_separator,
            generator::defaults::root::presenter_right_separator,
            generator::defaults::root::presenter_skip};
    state.presenter_params = {};
    generate_inner(tokenizer_config, generator_config, context, values, tree, state, true);
}

void cuttle::initialize(cuttle::generator_state_t &state) {
	state.index = 0;
	state.output = "";
}