//#define BOOST_TEST_DYN_LINK
//
//#include <iostream>
//#include <boost/test/unit_test.hpp>
//#include "generator_presenters_map_methods.hpp"
//#include "generator_defaults.hpp"
//
//using namespace cuttle;
//
//struct base_presenters_map_fixture {
//    generator_presenters_map_t presenters_map;
//};
//
//BOOST_FIXTURE_TEST_SUITE(presenters_map_add_suite, base_presenters_map_fixture)
//
//    BOOST_AUTO_TEST_CASE(case1) {
//        auto func1 = [](int argi, bool is_func, generator_presenter_params_t &params) {
//            return std::string("function1");
//        };
//        auto func2 = [](int argi, bool is_func, generator_presenter_params_t &params) {
//            return std::string("function2");
//        };
//        auto func3 = [](int argi, bool is_func, generator_presenter_params_t &params) {
//            return false;
//        };
//        add(presenters_map, "+", func1, func2, func3);
//        BOOST_CHECK_EQUAL(presenters_map["+"].left_separator, func1);
//        BOOST_CHECK_EQUAL(presenters_map["+"].right_separator, func2);
//        BOOST_CHECK_EQUAL(presenters_map["+"].skip, func3);
//    }
//
//#ifndef _WIN32
//
//    BOOST_AUTO_TEST_CASE(case2) {
//        auto func1 = [](int argi, bool is_func, generator_presenter_params_t &params) {
//            return std::string("function1");
//        };
//        generator_separator_func_t *func2 = nullptr;
//        auto func3 = [](int argi, bool is_func, generator_presenter_params_t &params) {
//            return false;
//        };
//        add(presenters_map, "+", func1, func2, func3);
//        BOOST_CHECK_EQUAL(presenters_map["+"].left_separator, func1);
//        generator_separator_func_t *func = presenters_map["+"].right_separator;
//        std::cout << (long long)func << " " << (long long)generator::defaults::basic::presenter_right_separator << std::endl;
//        BOOST_CHECK((long long)func == (long long)generator::defaults::basic::presenter_right_separator);
//        BOOST_CHECK_EQUAL(presenters_map["+"].skip, func3);
//        BOOST_CHECK_EQUAL("", presenters_map["+"].right_separator(2, true));
//    }
//
//    BOOST_AUTO_TEST_CASE(case3) {
//        auto func1 = nullptr;
//        auto func2 = nullptr;
//        auto func3 = [](int argi, bool is_func, generator_presenter_params_t &params) {
//            return false;
//        };
//        add(presenters_map, "+", func1, func2, func3);
//        BOOST_CHECK_EQUAL(presenters_map["+"].left_separator, generator::defaults::basic::presenter_left_separator);
//        BOOST_CHECK_EQUAL(presenters_map["+"].right_separator, generator::defaults::basic::presenter_right_separator);
//        BOOST_CHECK_EQUAL(presenters_map["+"].skip, func3);
//    }
//
//    BOOST_AUTO_TEST_CASE(case4) {
//        add(presenters_map, "+");
//        BOOST_CHECK_EQUAL(presenters_map["+"].left_separator, generator::defaults::basic::presenter_left_separator);
//        BOOST_CHECK_EQUAL(presenters_map["+"].right_separator, generator::defaults::basic::presenter_right_separator);
//        BOOST_CHECK_EQUAL(presenters_map["+"].skip, generator::defaults::basic::presenter_skip);
//    }
//
//#endif
//
//BOOST_AUTO_TEST_SUITE_END()