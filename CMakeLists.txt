cmake_minimum_required(VERSION 3.0.2)

project(CuttleGenerator VERSION 0.1 LANGUAGES CXX)

include(CMakeToolsHelpers OPTIONAL)
include(cmake/prepend_path.cmake)

file(GLOB SRCS "src/*.cpp")
file(GLOB TEST_SRCS "tests/*.cpp")

file(GLOB HEADERS "src/*.hpp")
file(GLOB TEST_HEADERS "tests/*.hpp")

set(DEPS_PARSER_SRCS_TEST_lst
	"context.cpp"
	"function_priority.cpp")
prepend_path(DEPS_PARSER_SRCS_TEST "deps/cuttle-parser/src/" ${DEPS_PARSER_SRCS_TEST_lst})

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE
  "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14")

if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")
  set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -Wextra")
elseif(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
	#set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
	#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_library(cuttle_generator SHARED ${SRCS} ${HEADERS})
add_executable(cuttle_generator_tests ${TEST_SRCS} ${DEPS_PARSER_SRCS_TEST} ${HEADERS} ${TEST_HEADERS})

target_include_directories(cuttle_generator PRIVATE "src/" "deps/cuttle-parser/src/" "deps/cuttle-translator/src/")
target_include_directories(cuttle_generator_tests PRIVATE "src/" "tests/" "deps/cuttle-parser/src/" "deps/cuttle-translator/src/")

target_link_libraries(cuttle_generator_tests cuttle_generator)
