#include "presenter.hpp"

std::string present_formatted_characters(const cuttle::tokenizer_config_t &config, std::string value) {
    std::string acc;
    for (auto ch : value) {
        auto it = config.formatted_characters_output.find(std::string("") + ch);
        if (it != config.formatted_characters_output.end()) {
            acc += it->second;
        } else if (ch == '\n') {
            acc += "\\n";
        } else {
            acc += ch;
        }
    }
    return acc;
}

std::string cuttle::present(const cuttle::tokenizer_config_t& config, const cuttle::value_t& value) {
	using namespace cuttle;
	
	auto formatted_string_delimiter = config.formatted_string.begin();
	auto normal_string_delimiter = config.normal_string.begin();
    auto macro_p_delimiter = config.macro_p.begin();
    auto macro_pf_delimiter = config.macro_pf.begin();
    auto macro_ps_delimiter = config.macro_ps.begin();

	switch (value.type) {
		case value_type::string:
			if (formatted_string_delimiter == config.formatted_string.end()) {
				return normal_string_delimiter->first + value.value + *normal_string_delimiter->second.begin();
			}
			return formatted_string_delimiter->first + present_formatted_characters(config, value.value) + *formatted_string_delimiter->second.begin();
	    case value_type::macro_p:
	        return macro_p_delimiter->first + value.value + *macro_p_delimiter->second.begin();
        case value_type::macro_pf:
            return macro_pf_delimiter->first + value.value + *macro_pf_delimiter->second.begin();
        case value_type::macro_ps:
            return macro_ps_delimiter->first + value.value + *macro_ps_delimiter->second.begin();
		default:
			return value.value;
	}
}