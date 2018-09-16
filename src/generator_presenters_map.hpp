#pragma once

#include <unordered_map>
#include "generator_presenters.hpp"

namespace cuttle {
    using generator_presenters_map_t = std::unordered_map<std::string, generator_presenters_t>;
}