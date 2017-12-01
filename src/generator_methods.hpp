#pragma once

#include "generator.hpp"
#include "context.hpp"
#include "call_tree.hpp"
#include "value.hpp"
#include "tokenizer_config.hpp"

namespace cuttle {
	void generate(
		const tokenizer_config_t& tokenizer_config, const context_t& context,
		int& index, const values_t& values, const call_tree& tree, generator_state_t& state
	);

	void initialize(generator_state_t& state, int size);
}