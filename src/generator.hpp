#pragma once

#include <string>
#include <vector>

namespace cuttle {
	struct generator_state_t {
		std::vector<bool> used;
		std::string output;
	};
}