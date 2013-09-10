cmake_minimum_required(VERSION 2.8)

add_library(pwned_leveldb leveldb.cpp leveldb.hpp)

enable_testing()
add_executable(pwned_leveldb_tests leveldb.cpp tests.cpp)
target_link_libraries(pwned_leveldb_tests gtest leveldb pthread)
add_test(NAME pwned_leveldb_tests COMMAND pwned_leveldb_tests)
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

