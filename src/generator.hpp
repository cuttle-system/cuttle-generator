#pragma once

#include <string>
#include <vector>

namespace cuttle {
	using generator_state_t = struct {
		std::vector<bool> used;
		std::string output;
	};
}