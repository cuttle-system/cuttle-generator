#include "value.hpp"

bool cuttle::operator==(const cuttle::value_t& left, const cuttle::value_t& right)
{
	return (left.value == right.value && left.type == right.type);
}