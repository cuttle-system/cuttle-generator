#include <iostream>
#include <string>
#include "test.hpp"
#include "test_generator.hpp"
#include "context_methods.hpp"
#include "generator_methods.hpp"
#include "tokenizer_config.hpp"

inline void test_generates_basic_code() {
	using namespace cuttle;

	context_t context;
	initialize(context);

	add(context, "+", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "-", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "foo", { function_type::prefix, 3 }, FUNCTION_ID_UNKNOWN);
	add(context, "!", { function_type::postfix, 1 }, FUNCTION_ID_UNKNOWN);

	tokenizer_config_t tokenizer_config = { {},{{"'", {"'"}}},{},{},false };

	{
		values_t values = { {"foo", value_type::func_name}, {"1", value_type::number}, {"2", value_type::number},{ "3", value_type::number } };
		call_tree_t tree = { { {1, 2, 3}, {}, {}, {} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "foo 1 2 3", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true, true }), "Used");
	}
	{
		values_t values = { { "+", value_type::func_name },{ "1", value_type::number },{ "2", value_type::number } };
		call_tree_t tree = { { { 1, 2 },{},{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "1 + 2", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true }), "Used");
	}
	{
		values_t values = { { "1", value_type::number },{ "2", value_type::number },{ "-", value_type::func_name } };
		call_tree_t tree = { { {},{},{ 0, 1 } } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 2, "Index");
		AssertEqual(state.output, "1 - 2", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true }), "Used");
	}
	{
		values_t values = { { "!", value_type::func_name },{ "5", value_type::number } };
		call_tree_t tree = { { { 1 },{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "5 !", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true }), "Used");
	}
	{
		values_t values = { { "foo", value_type::func_name },{ "1", value_type::number },{ "2", value_type::number },{ "foo bar", value_type::string } };
		call_tree_t tree = { { { 1, 2, 3 },{},{},{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "foo 1 2 'foo bar'", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true, true }), "Used");
	}
}

inline void test_generates_nested_functions_code() {
	using namespace cuttle;

	context_t context;
	initialize(context);

	add(context, "+", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "-", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "*", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "foo", { function_type::prefix, 3 }, FUNCTION_ID_UNKNOWN);
	add(context, "!", { function_type::postfix, 1 }, FUNCTION_ID_UNKNOWN);

	tokenizer_config_t tokenizer_config = { {},{ { "'",{ "'" } } },{},{},false };

	{
		values_t values = { { "*", value_type::func_name },{ "+", value_type::func_name },{ "2", value_type::number },{ "1", value_type::number },{ "5", value_type::number } };
		call_tree_t tree = { { { 2, 4 },{ 0, 3 },{},{},{} } };
		generator_state_t state;
		int index = 1;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 1, "Index");
		AssertEqual(state.output, "2 * 5 + 1", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true, true, true }), "Used");
	}
	{
		values_t values = { { "foo", value_type::func_name },{ "10", value_type::number },{ "+", value_type::func_name },{ "2", value_type::number },{ "3", value_type::number },{ "!", value_type::func_name },{ "5", value_type::number } };
		call_tree_t tree = { { { 1, 2, 3 },{},{ 4, 5 },{},{},{ 6 },{} } };
		generator_state_t state;
		int index = 0;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 0, "Index");
		AssertEqual(state.output, "foo 10 3 + 5 ! 2", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true, true, true, true, true }), "Used");
	}
}

inline void test_generates_zero_length_code() {
	using namespace cuttle;

	context_t context;
	initialize(context);

	add(context, "+", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "-", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "*", { function_type::infix, 2 }, FUNCTION_ID_UNKNOWN);
	add(context, "!", { function_type::postfix, 1 }, FUNCTION_ID_UNKNOWN);

	tokenizer_config_t tokenizer_config = { {},{ { "'",{ "'" } } },{},{},false };

	{
		values_t values = { { "foo", value_type::func_name },{ "+", value_type::func_name },{ "bar", value_type::func_name } };
		call_tree_t tree = { { {},{ 0, 2 },{} } };
		generator_state_t state;
		int index = 1;
		initialize(state, tree.src.size());

		generate(tokenizer_config, context, index, values, tree, state);
		AssertEqual(index, 1, "Index");
		AssertEqual(state.output, "foo + bar", "Output");
		AssertEqual(state.used, (std::vector<bool>{ true, true, true }), "Used");
	}
}

void run_generator_tests() {
    TESTCASE
	test_generates_basic_code();
	test_generates_nested_functions_code();
	test_generates_zero_length_code();
}