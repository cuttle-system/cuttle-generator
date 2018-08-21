include_directories("../src" ${Boost_INCLUDE_DIR})
link_libraries(cuttle-generator ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

file(GLOB_RECURSE TEST_SRCS "*.cpp")

add_executable(cuttle-generator-tests ${TEST_SRCS})