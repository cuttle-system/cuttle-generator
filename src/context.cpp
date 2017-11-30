#include <string>
#include "context.hpp"
#include "function.hpp"
#include "function_priority.hpp"

void cuttle::initialize(cuttle::context_t &context) {
	using namespace cuttle;

	initialize(context.funcs_prior);
	initialize(context.funcs);
}

void cuttle::initialize(cuttle::funcs_t &funcs) {
	using namespace cuttle;

	function_t
		function_unknown = { PREFIX_FUNCTION, 0 },
		function_last = { PREFIX_FUNCTION, 0 };

	funcs.resize(2);

	funcs[FUNCTION_ID_UNKNOWN] = function_unknown;
	funcs[FUNCTION_ID_LAST] = function_last;
}

void cuttle::add(cuttle::context_t &context, std::string name, cuttle::function_t func, cuttle::function_id_t after) {
	using namespace cuttle;

	function_id_t id = add(context.funcs, context.funcname_to_id, name, func);
	add(context.funcs_prior, id, after);
}

cuttle::function_id_t cuttle::add(cuttle::funcs_t &funcs, cuttle::funcname_to_id_t &funcname_to_id, std::string name, cuttle::function_t func) {
	function_id_t id = funcs.size();
	funcname_to_id[name] = id;
	funcs.push_back(func);
	return id;
}