cmake_minimum_required(VERSION 2.8)

add_library(pwned_jsoncpp jsoncpp.cpp jsoncpp.hpp)

include(FindPkgConfig)

enable_testing()
add_executable(pwned_jsoncpp_tests jsoncpp.cpp tests.cpp)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
target_link_libraries(pwned_jsoncpp_tests gtest jsoncpp pthread)
add_test(NAME pwned_jsoncpp_tests COMMAND pwned_jsoncpp_tests)
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

