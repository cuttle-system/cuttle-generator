#define BOOST_TEST_DYN_LINK

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "context_methods.hpp"
#include "generator_methods.hpp"
#include "generator_presenters_map_methods.hpp"

using namespace cuttle;

struct base_generator_fixture {
    context_t context;
    generator_state_t state;
    generator_config_t generator_config;

    virtual void setup() {
        initialize(context);
        initialize(state);
    }
};

struct generates_basic_code_suite_fixture : public base_generator_fixture {
    tokenizer_config_t tokenizer_config = {{{"\"", {"\""}}}, {{"'", {"'"}}}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, false};

    void setup() override {
        base_generator_fixture::setup();

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

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 3");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"+", value_type::func_name},
                           {"1", value_type::number},
                           {"2", value_type::number}};
        call_tree_t tree = {{{1, 2}, {}, {}, {0}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "1 + 2");
    }

    BOOST_AUTO_TEST_CASE(case3) {
        values_t values = {{"1", value_type::number},
                           {"2", value_type::number},
                           {"-", value_type::func_name}};
        call_tree_t tree = {{{}, {}, {0, 1}, {2}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "1 - 2");
    }

    BOOST_AUTO_TEST_CASE(case4) {
        values_t values = {{"!", value_type::func_name},
                           {"5", value_type::number}};
        call_tree_t tree = {{{1}, {}, {0}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "5 !");
    }

    BOOST_AUTO_TEST_CASE(case5) {
        values_t values = {{"foo",     value_type::func_name},
                           {"1",       value_type::number},
                           {"2",       value_type::number},
                           {"foo bar", value_type::string}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 \"foo bar\"");
    }

    BOOST_AUTO_TEST_CASE(case6) {
        values_t values = {{"foo",     value_type::func_name},
                           {"1",       value_type::number},
                           {"2",       value_type::number},
                           {"foo \nbar", value_type::string}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 \"foo \\nbar\"");
    }

    BOOST_AUTO_TEST_CASE(case7) {
        tokenizer_config.formatted_characters = {{"~n", "\n"}};
        tokenizer_config.formatted_characters_output = {{"\n", "~n"}};
        values_t values = {{"foo",     value_type::func_name},
                           {"1",       value_type::number},
                           {"2",       value_type::number},
                           {"foo \nbar", value_type::string}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 1 2 \"foo ~nbar\"");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generates_nested_functions_code_suite_fixture : public base_generator_fixture {
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, false};

    void setup() override {
        base_generator_fixture::setup();

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "foo", {function_type::prefix, 3}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_nested_functions_code_suite, generates_nested_functions_code_suite_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"*", value_type::func_name},
                           {"+", value_type::func_name},
                           {"2", value_type::number},
                           {"1", value_type::number},
                           {"5", value_type::number}};
        call_tree_t tree = {{{2, 4}, {0, 3}, {}, {}, {}, {1}}};

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

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo 10 3 + 5 ! 2");
    }

BOOST_AUTO_TEST_SUITE_END()


struct generates_zero_length_code_suite_fixture : public base_generator_fixture {
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, false};

    void setup() override {
        base_generator_fixture::setup();

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_zero_length_code_suite, generates_zero_length_code_suite_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name}};
        call_tree_t tree = {{{}, {0, 2}, {}, {1}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo + bar");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {};
        call_tree_t tree = {{{}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generates_unknown_function_names_with_zero_args_suite_fixture : public base_generator_fixture {
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, false};

    void setup() override {
        base_generator_fixture::setup();

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "!", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generates_unknown_function_names_with_zero_args_suite, generates_unknown_function_names_with_zero_args_suite_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name},
                           {"!",   value_type::func_name}};
        call_tree_t tree = {{{}, {0, 3}, {}, {2}, {1}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo + bar !");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generate_merges_certain_functions_up_in_hierarchy_suite_fixture : public base_generator_fixture {
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, false};
    generator_config_t generator_config = {{}, {"!", "join"}};

    void setup() override {
        base_generator_fixture::setup();

        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
    }
};

BOOST_FIXTURE_TEST_SUITE(generate_merges_certain_functions_up_in_hierarchy_suite,
        generate_merges_certain_functions_up_in_hierarchy_suite_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name},
                           {"!",   value_type::func_name}};
        call_tree_t tree = {{{}, {0, 3}, {}, {2}, {1}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo + bar");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"join", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"baz2", value_type::func_name},
                           {"baz3", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};

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

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1\nbaz2\nbaz3\nbaz4\nbaz5\n1 + 2\nbaz6");
    }

BOOST_AUTO_TEST_SUITE_END()

struct generate_properly_uses_separators_config_suite_fixture : public base_generator_fixture {
    tokenizer_config_t tokenizer_config = {{}, {{"'", {"'"}}}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, false};
    generator_config_t generator_config = {{}, {"!", "join"}};

    void setup() override {
        base_generator_fixture::setup();

        add(context, "quux", {function_type::prefix, 3}, FUNCTION_ID_UNKNOWN);
        add(context, "+", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "rem", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "-", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "*", {function_type::infix, 2}, FUNCTION_ID_UNKNOWN);
        add(context, "++", {function_type::postfix, 1}, FUNCTION_ID_UNKNOWN);


        add(generator_config.presenters_map, "+",
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                if (argi == 0) return std::string("");
                return std::string("\n");
            }, [](int argi, bool is_func, generator_presenter_params_t &params) {
                return std::string("");
            }, [](int argi, bool is_func, generator_presenter_params_t &params) {
                return false;
            }
        );

        add(generator_config.presenters_map, "-",
            nullptr,
            nullptr,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                return is_func;
            }
        );

        add(generator_config.presenters_map, "++",
            nullptr,
            nullptr,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                return is_func;
            }
        );

        add(generator_config.presenters_map, "quux",
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                if (argi == 0) return std::string("");
                return std::string("   ");
            },
            nullptr,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                return is_func;
            }
        );
    }
};

BOOST_FIXTURE_TEST_SUITE(generate_properly_uses_separators_config_suite,
                         generate_properly_uses_separators_config_suite_fixture)

    BOOST_AUTO_TEST_CASE(case1) {
        values_t values = {{"foo", value_type::func_name},
                           {"+",   value_type::func_name},
                           {"bar", value_type::func_name}};
        call_tree_t tree = {{{}, {0, 2}, {}, {1}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo\n+\nbar");
    }

    BOOST_AUTO_TEST_CASE(case2) {
        values_t values = {{"foo", value_type::func_name},
                           {"-",   value_type::func_name},
                           {"bar", value_type::func_name}};
        call_tree_t tree = {{{}, {0, 2}, {}, {1}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "foo bar");
    }

    BOOST_AUTO_TEST_CASE(case3) {
        values_t values = {{"quux", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"join", value_type::func_name},
                           {"baz2", value_type::func_name},
                           {"baz3", value_type::func_name},
                           {"join", value_type::func_name},
                           {"baz4", value_type::func_name},
                           {"baz5", value_type::func_name},
                           {"rem", value_type::func_name},
                           {"1", value_type::number},
                           {"2", value_type::number},
                           {"baz6", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 11}, {}, {3, 4, 5}, {}, {}, {6, 7, 8}, {}, {}, {9, 10}, {}, {}, {}, {0}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1   baz2   baz3   baz4   baz5   1 rem 2   baz6");
    }

    BOOST_AUTO_TEST_CASE(case4) {
        values_t values = {{"join", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"baz2", value_type::func_name},
                           {"baz3", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 3}, {}, {}, {}, {0}}};

        add(generator_config.presenters_map, GENERATOR_ROOT_FUNCTION_NAME,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                if (argi == 0) return std::string("");
                return std::string(" ");
            },
            nullptr,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                return is_func;
            }
        );

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1 baz2 baz3");
    }

    BOOST_AUTO_TEST_CASE(case5) {
        values_t values = {{"join", value_type::func_name},
                           {"baz1", value_type::func_name},
                           {"+", value_type::func_name},
                           {"1", value_type::number},
                           {"2", value_type::number},
                           {"baz3", value_type::func_name}};
        call_tree_t tree = {{{1, 2, 5}, {}, {3, 4}, {}, {}, {}, {0}}};

        add(generator_config.presenters_map, GENERATOR_ROOT_FUNCTION_NAME,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                if (argi == 0) return std::string("");
                return std::string(" ");
            },
            nullptr,
            [](int argi, bool is_func, generator_presenter_params_t &params) {
                return is_func;
            }
        );

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1 1\n+\n2 baz3");
    }

    BOOST_AUTO_TEST_CASE(case6) {
        values_t values = {{"baz1", value_type::func_name},
                           {"++", value_type::func_name}};
        call_tree_t tree = {{{}, {0}, {1}}};

        generate(tokenizer_config, generator_config, context, values, tree, state);
        BOOST_CHECK_EQUAL(state.output, "baz1");
    }

BOOST_AUTO_TEST_SUITE_END()
