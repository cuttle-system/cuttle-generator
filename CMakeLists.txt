cmake_minimum_required(VERSION 3.0.2)
project(CuttleGenerator VERSION 0.2 LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CMakeToolsHelpers OPTIONAL)
include(add_dependency)

set(CMAKE_BUILD_TYPE Debug)

set(BOOST_ROOT ../boost)
find_package(Boost 1.65.1 REQUIRED COMPONENTS unit_test_framework)

file(COPY ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} DESTINATION ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 14)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG
            "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE
            "${CMAKE_CXX_FLAGS_RELEASE} -Wextra -O3")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
endif()

add_dependency(cuttle-parser)

include_directories(
        "../cuttle-parser/src"
        "../cuttle-translator/src")

add_subdirectory(src)
add_subdirectory(tests)
