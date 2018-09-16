#pragma once

#include "generator.hpp"
#include "generator_presenters.hpp"

namespace cuttle {
    std::string
    present(generator_state_t &presenters, const std::string &argument_string, int argi, bool is_func = false);
}