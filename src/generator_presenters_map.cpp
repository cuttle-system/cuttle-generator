#include "generator_presenters_map_methods.hpp"
#include "generator_defaults.hpp"

void cuttle::add(generator_presenters_map_t &presenters_map,
                 const std::string &function_name,
                 generator_separator_func_t *left_separator,
                 generator_separator_func_t *right_separator,
                 generator_skip_func_t *skip) {

    presenters_map[function_name].left_separator = (left_separator
                                                    ? left_separator
                                                    : generator::defaults::basic::presenter_left_separator);

    presenters_map[function_name].right_separator = (right_separator
                                                     ? right_separator
                                                     : generator::defaults::basic::presenter_right_separator);

    presenters_map[function_name].skip = (skip
                                          ? skip
                                          : generator::defaults::basic::presenter_skip);
}