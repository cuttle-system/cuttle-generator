#include "presenter.hpp"

std::string cuttle::present(const cuttle::tokenizer_config_t& config, const cuttle::value_t& value) {
	using namespace cuttle;
	
	auto formatted_string_delimiter = config.formatted_string.begin();
	auto normal_string_delimiter = config.normal_string.begin();

	switch (value.type) {
		case value_type::string:
			if (normal_string_delimiter == config.normal_string.end()) {
				return formatted_string_delimiter->first + value.value + *formatted_string_delimiter->second.begin();
			}
			return normal_string_delimiter->first + value.value + *normal_string_delimiter->second.begin();
		default:
			return value.value;
	}
}