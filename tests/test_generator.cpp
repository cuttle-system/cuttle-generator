#define BOOST_TEST_DYN_LINK

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "context_methods.hpp"
#include "generator_methods.hpp"

using namespace cuttle;

struct generates_basic_code_suite_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};

    void setup() {
        initialize(context);

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "foo", {function_type::prefix, 3}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_basic_code_suite, generates_basic_code_suite_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"1",   value_type::number},
                           {"2",   value_type::number},
                           {"3",   value_type::number}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true, true};
        unsigned int index = 0;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 0u);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 3");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"+", value_type::func_name},
                           {"1", value_type::number},
                           {"2", value_type::number}};
        call_tree_t tree = {{{1, 2}, {}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true};
        unsigned int index = 0;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 0u);
        BOOST_CHECK_EQUAL(state.output, "1 + 2");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

    BOOST_AUTO_TEST_CASE(case3) {
        values_t values = {{"1", value_type::number},
                           {"2", value_type::number},
                           {"-", value_type::func_name}};
        call_tree_t tree = {{{}, {}, {0, 1}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true};
        unsigned int index = 0;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 2u);
        BOOST_CHECK_EQUAL(state.output, "1 - 2");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

    BOOST_AUTO_TEST_CASE(case4) {
        values_t values = {{"!", value_type::func_name},
                           {"5", value_type::number}};
        call_tree_t tree = {{{1}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true};
        unsigned int index = 0;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 0u);
        BOOST_CHECK_EQUAL(state.output, "5 !");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

    BOOST_AUTO_TEST_CASE(case5) {
        values_t values = {{"foo",     value_type::func_name},
                           {"1",       value_type::number},
                           {"2",       value_type::number},
                           {"foo bar", value_type::string}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true, true};
        unsigned int index = 0;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 0u);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 'foo bar'");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

BOOST_AUTO_TEST_SUITE_END()

struct generates_nested_functions_code_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};

    void setup() {
        initialize(context);

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "foo", {function_type::prefix, 3}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_nested_functions_code, generates_nested_functions_code_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"*", value_type::func_name},
                           {"+", value_type::func_name},
                           {"2", value_type::number},
                           {"1", value_type::number},
                           {"5", value_type::number}};
        call_tree_t tree = {{{2, 4}, {0, 3}, {}, {}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true, true, true};
        unsigned int index = 1;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 1u);
        BOOST_CHECK_EQUAL(state.output, "2 * 5 + 1");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"foo", value_type::func_name},
                           {"10",  value_type::number},
                           {"+",   value_type::func_name},
                           {"2",   value_type::number},
                           {"3",   value_type::number},
                           {"!",   value_type::func_name},
                           {"5",   value_type::number}};
        call_tree_t tree = {{{1, 2, 3}, {}, {4, 5}, {}, {}, {6}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true, true, true, true, true};
        unsigned int index = 0;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 0u);
        BOOST_CHECK_EQUAL(state.output, "foo 10 3 + 5 ! 2");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

BOOST_AUTO_TEST_SUITE_END()


struct generates_zero_length_code_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};

    void setup() {
        initialize(context);

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_zero_length_code, generates_zero_length_code_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name}};
        call_tree_t tree = {{{}, {0, 2}, {}}};
        generator_state_t state;
        std::vector<bool> used_expected {true, true, true};
        unsigned int index = 1;
        initialize(state, tree.src.size());

        generate(tokenizer_config, context, index, values, tree, state);
        BOOST_CHECK_EQUAL(index, 1u);
        BOOST_CHECK_EQUAL(state.output, "foo + bar");
        BOOST_CHECK_EQUAL_COLLECTIONS(
                state.used.begin(), state.used.end(),
                used_expected.begin(), used_expected.end());
    }

BOOST_AUTO_TEST_SUITE_END()

