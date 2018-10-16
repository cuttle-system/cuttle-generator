#include "generator_defaults.hpp"

using namespace cuttle::generator::defaults;

std::string root::presenter_left_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params) {
    if (argi == 0) return "";
    return "\n";
}


std::string root::presenter_right_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params) {
    return "";
}

bool root::presenter_skip(int argi, bool is_func, cuttle::generator_presenter_params_t &params) {
    return is_func;
}

std::string basic::presenter_left_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params) {
    if (argi == 0) return "";
    return " ";
}

std::string basic::presenter_right_separator(int argi, bool is_func, cuttle::generator_presenter_params_t &params) {
    return "";
}

bool basic::presenter_skip(int argi, bool is_func, cuttle::generator_presenter_params_t &params) {
    return false;
}