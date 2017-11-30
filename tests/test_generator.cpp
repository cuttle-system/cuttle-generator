#include <iostream>
#include <string>
#include "test.hpp"
#include "test_generator.hpp"
#include "context.hpp"
#include "generator.hpp"

inline void test_generates_basic_code() {
	using namespace cuttle;

	context_t context;
	initialize(context);

	add(context, "+", { INFIX_FUNCTION, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "-", { INFIX_FUNCTION, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "foo", { PREFIX_FUNCTION, 3 }, FUNCTION_ID_UNKNOWN);
	add(context, "!", { POSTFIX_FUNCTION, 1 }, FUNCTION_ID_UNKNOWN);

	{
		values_t values = { {"foo", TYPE_FUNCTION_NAME}, {"1", TYPE_NUMBER}, {"2", TYPE_NUMBER},{ "3", TYPE_NUMBER } };
		call_tree_t tree = { { {1, 2, 3}, {}, {}, {} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "foo 1 2 3", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true, true }), "Used");
	}
	{
		values_t values = { { "+", TYPE_FUNCTION_NAME },{ "1", TYPE_NUMBER },{ "2", TYPE_NUMBER } };
		call_tree_t tree = { { { 1, 2 },{},{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "1 + 2", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true }), "Used");
	}
	{
		values_t values = { { "1", TYPE_NUMBER },{ "2", TYPE_NUMBER },{ "-", TYPE_FUNCTION_NAME } };
		call_tree_t tree = { { {},{},{ 0, 1 } } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(context, index, values, tree, state);
		AssertEqual(index, 2, "Index");
		AssertEqual(state.output, "1 - 2", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true }), "Used");
	}
	{
		values_t values = { { "!", TYPE_FUNCTION_NAME },{ "5", TYPE_NUMBER } };
		call_tree_t tree = { { { 1 },{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "5 !", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true }), "Used");
	}
	{
		// TODO
		values_t values = { { "foo", TYPE_FUNCTION_NAME },{ "1", TYPE_NUMBER },{ "2", TYPE_NUMBER },{ "foo bar", TYPE_STRING } };
		call_tree_t tree = { { { 1, 2, 3 },{},{},{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "foo 1 2 'foo bar'", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true, true }), "Used");
	}
}

void run_generator_tests() {
    TESTCASE
	test_generates_basic_code();
}