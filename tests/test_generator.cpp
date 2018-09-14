#define BOOST_TEST_DYN_LINK

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "context_methods.hpp"
#include "generator_methods.hpp"

using namespace cuttle;

struct generates_basic_code_suite_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};
    generator_config_t generator_config;

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
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 3");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"+", value_type::func_name},
                           {"1", value_type::number},
                           {"2", value_type::number}};
        call_tree_t tree = {{{1, 2}, {}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "1 + 2");
    }

    BOOST_AUTO_TEST_CASE(case3) {
        values_t values = {{"1", value_type::number},
                           {"2", value_type::number},
                           {"-", value_type::func_name}};
        call_tree_t tree = {{{}, {}, {0, 1}, {2}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "1 - 2");
    }

    BOOST_AUTO_TEST_CASE(case4) {
        values_t values = {{"!", value_type::func_name},
                           {"5", value_type::number}};
        call_tree_t tree = {{{1}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "5 !");
    }

    BOOST_AUTO_TEST_CASE(case5) {
        values_t values = {{"foo",     value_type::func_name},
                           {"1",       value_type::number},
                           {"2",       value_type::number},
                           {"foo bar", value_type::string}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 'foo bar'");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generates_nested_functions_code_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};
    generator_config_t generator_config;

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
        call_tree_t tree = {{{2, 4}, {0, 3}, {}, {}, {}, {1}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "2 * 5 + 1");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"foo", value_type::func_name},
                           {"10",  value_type::number},
                           {"+",   value_type::func_name},
                           {"2",   value_type::number},
                           {"3",   value_type::number},
                           {"!",   value_type::func_name},
                           {"5",   value_type::number}};
        call_tree_t tree = {{{1, 2, 3}, {}, {4, 5}, {}, {}, {6}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 10 3 + 5 ! 2");
    }

BOOST_AUTO_TEST_SUITE_END()


struct generates_zero_length_code_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};
    generator_config_t generator_config;

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
        call_tree_t tree = {{{}, {0, 2}, {}, {1}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo + bar");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {};
        call_tree_t tree = {{{}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generates_unknown_function_names_with_zero_args_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};
    generator_config_t generator_config;

    void setup() {
        initialize(context);

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_unknown_function_names_with_zero_args_code, generates_unknown_function_names_with_zero_args_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name},
                           {"!",   value_type::func_name}};
        call_tree_t tree = {{{}, {0, 3}, {}, {2}, {1}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo + bar !");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generate_merges_certain_functions_up_in_hierarchy_fixture {
    context_t context;
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, false};
    generator_config_t generator_config = {{"!", "join"}};

    void setup() {
        initialize(context);

        add(context, "join", {function_type::prefix, 3}, FUNCTION_ID_UNKNOWN);
        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generate_merges_certain_functions_up_in_hierarchy_code,
        generate_merges_certain_functions_up_in_hierarchy_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name},
                           {"!",   value_type::func_name}};
        call_tree_t tree = {{{}, {0, 3}, {}, {2}, {1}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo + bar");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"join", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"baz2", value_type::func_name},
                           {"baz3", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1\nbaz2\nbaz3");
    }

    BOOST_AUTO_TEST_CASE(case3) {
        values_t values = {{"join", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"join", value_type::func_name},
                           {"baz2", value_type::func_name},
                           {"baz3", value_type::func_name},
                           {"baz4", value_type::func_name},
                           {"baz5", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 6}, {}, {3, 4, 5}, {}, {}, {}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1\nbaz2\nbaz3\nbaz4\nbaz5");
    }

    BOOST_AUTO_TEST_CASE(case4) {
        values_t values = {{"join", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"join", value_type::func_name},
                           {"baz2", value_type::func_name},
                           {"baz3", value_type::func_name},
                           {"join", value_type::func_name},
                           {"baz4", value_type::func_name},
                           {"baz5", value_type::func_name},
                           {"+", value_type::func_name},
                           {"1", value_type::number},
                           {"2", value_type::number},
                           {"baz6", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 11}, {}, {3, 4, 5}, {}, {}, {6, 7, 8}, {}, {}, {9, 10}, {}, {}, {}, {0}}};
        generator_state_t state;
        initialize(state);

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1\nbaz2\nbaz3\nbaz4\nbaz5\n1 + 2\nbaz6");
    }

BOOST_AUTO_TEST_SUITE_END()



