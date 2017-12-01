#pragma once

#include <string>
#include "tokenizer_config.hpp"
#include "value.hpp"

namespace cuttle {
	std::string present(const tokenizer_config_t& config, const value_t& value);
}