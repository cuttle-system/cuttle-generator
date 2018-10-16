#include "generator_presenters_methods.hpp"
#include "generator.hpp"

std::string
cuttle::present(generator_state_t &state, const std::string &argument_string, int argi, bool is_func) {
    using namespace cuttle;

    if (state.presenters.skip(argi, is_func, state.presenter_params)) {
        return "";
    }

    if (is_func && state.include_function_name) {
        ++state.arg_index;
    }

    return state.presenters.left_separator(argi, is_func, state.presenter_params)
        + argument_string
        + state.presenters.right_separator(argi, is_func, state.presenter_params);
}