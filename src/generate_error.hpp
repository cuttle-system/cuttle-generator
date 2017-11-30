#pragma once

#include <stdexcept>

namespace cuttle {
	class generate_error : public std::runtime_error
	{
	public:
		generate_error() : runtime_error("Unknown generate error") {}
		generate_error(std::string msg) : runtime_error(msg.c_str()) {}
	};
}