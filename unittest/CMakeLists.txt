
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)

enable_testing()

include_directories(..)
include(GoogleTest)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

add_executable(test_builtin test_builtin.cpp)
target_include_directories(
    test_builtin
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_INSTALL_PREFIX}/include
)
target_link_libraries(test_builtin gtest_main gtest simdjson pthread)
configure_file(test_builtin.json test_builtin.json)
gtest_discover_tests(test_builtin)

add_executable(test_array test_array.cpp)
target_include_directories(
    test_array
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_INSTALL_PREFIX}/include
)
target_link_libraries(test_array gtest_main gtest simdjson pthread)
configure_file(test_array.json test_array.json)
gtest_discover_tests(test_array)

add_executable(test_object test_object.cpp)
target_include_directories(
    test_object
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_INSTALL_PREFIX}/include
)
target_link_libraries(test_object gtest_main gtest simdjson pthread)
configure_file(test_object.json test_object.json)
gtest_discover_tests(test_object)
