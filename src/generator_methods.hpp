#pragma once

#include "generator.hpp"
#include "context.hpp"
#include "call_tree.hpp"
#include "value.hpp"
#include "tokenizer_config.hpp"
#include "generator_config.hpp"

namespace cuttle {
	void generate(const tokenizer_config_t &tokenizer_config, const generator_config_t &generator_config, const context_t &context,
                      const values_t &values, const call_tree_t &tree, generator_state_t &state);

	void initialize(cuttle::generator_state_t &state);
}