cmake_minimum_required(VERSION 3.14)

set(PARSER_WRAP_VERSION_MAJOR 0)
set(PARSER_WRAP_VERSION_MINOR 1)
set(PARSER_WRAP_PATCH_VER 0)
set(PARSER_WRAP_LIB_VER "${PARSER_WRAP_VERSION_MAJOR}.${PARSER_WRAP_VERSION_MINOR}.${PARSER_WRAP_PATCH_VER}")

project(
    parser_wrap
    VERSION
        ${PARSER_WRAP_VERSION_MAJOR}.${PARSER_WRAP_VERSION_MINOR}.${PARSER_WRAP_PATCH_VER}
    LANGUAGES
        CXX
    DESCRIPTION
        "current version is an wrapper of rapidjson/simdjson"
)

option(PARSER_WRAP_BUILD_SHARED "Build parser_wrap shared libraries(gtest)." OFF)
option(PARSER_WRAP_BUILD_STATIC "Build parser_wrap static libraries(gtest)." ON)
option(PARSER_WRAP_BUILD_TESTS "Build parser_wrap unittests(gtest)." ON)
option(PARSER_WRAP_BUILD_ASAN "Build parser_wrap with address sanitizer (gcc/clang)" OFF)
option(PARSER_WRAP_BUILD_UBSAN "Build parser_wrap with undefined behavior sanitizer (gcc/clang)" OFF)

# Git repo
set(PARSER_WRAP_GIT_REPO "https://github.com/")

# Useful dir
set( PARSER_WRAP_DEP_INC_DIR ${CMAKE_INSTALL_PREFIX}/include)
set( PARSER_WRAP_DEP_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set( PARSER_WRAP_CON_DIR ${CMAKE_BINARY_DIR})
set( PARSER_WRAP_INC_DIR ${CMAKE_SOURCE_DIR}/include)
set( PARSER_WRAP_SRC_DIR ${CMAKE_SOURCE_DIR}/source)

# Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# cxx flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    set(EXTRA_CXX_FLAGS -Weffc++ -Wswitch-default -Wfloat-equal -Wconversion -Wsign-conversion)
    # CXX_FLAGS std
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
        message(FATAL_ERROR "GCC < 5.0 doesn't support c++17")
    endif()
    # CXX_FLAGS asan
    if (PARSER_WRAP_BUILD_ASAN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    endif()
    # CXX_FLAGS ubsan
    if (PARSER_WRAP_BUILD_UBSAN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-missing-field-initializers")
    set(EXTRA_CXX_FLAGS -Weffc++ -Wswitch-default -Wfloat-equal -Wconversion -Wimplicit-fallthrough)
    # CXX_FLAGS std
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.0")
        message(FATAL_ERROR "CLANG < 4.0 doesn't support c++17")
    endif()
    # CXX_FLAGS asan
    if (PARSER_WRAP_BUILD_ASAN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    endif()
    # CXX_FLAGS ubsan
    if (PARSER_WRAP_BUILD_UBSAN)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        endif()
    endif()
endif()

# depend
find_package(RapidJSON REQUIRED)
find_package(simdjson REQUIRED)

# output
if(PARSER_WRAP_BUILD_STATIC)
    add_library(
        parser_wrap STATIC
        "${PARSER_WRAP_SRC_DIR}/rapidjson_parser.cpp"
        "${PARSER_WRAP_SRC_DIR}/simdjson_parser.cpp"
    )
    # link
    target_link_directories(parser_wrap PUBLIC ${PARSER_WRAP_DEP_LIB_DIR})
    target_link_libraries(parser_wrap PUBLIC simdjson)
    
    install(
        TARGETS parser_wrap
        LIBRARY DESTINATION lib
    )
endif()

if(PARSER_WRAP_BUILD_SHARED)
    add_library(
        parser_wrap SHARED
        "${PARSER_WRAP_SRC_DIR}/rapidjson_parser.cpp"
        "${PARSER_WRAP_SRC_DIR}/simdjson_parser.cpp"
    )
    add_library(parser_wrap::parser_wrap ALIAS parser_wrap)

    include(simdjson)

    install(
        TARGETS parser_wrap
        LIBRARY DESTINATION lib
    )
endif()

if(PARSER_WRAP_BUILD_STATIC OR PARSER_WRAP_BUILD_SHARED)
    # properties
    set_target_properties(
        parser_wrap PROPERTIES
        VERSION ${PARSER_WRAP_VERSION_MAJOR}
        SOVERSION ${PARSER_WRAP_VERSION_MAJOR}
    )

    # include
    target_include_directories(
        parser_wrap
        PUBLIC
            "${PARSER_WRAP_DEP_INC_DIR}"
            "${PARSER_WRAP_INC_DIR}"
    )
endif()

if(PARSER_WRAP_BUILD_TESTS)
    add_subdirectory(unittest)
endif()

################################################################################
#include(CMakePackageConfigHelper)
include(GNUInstallDirs)

# output headers
install(
    FILES
        "${PARSER_WRAP_INC_DIR}/zh_parser.hpp "
        "${PARSER_WRAP_INC_DIR}/rapidjson_parser.hpp "
        "${PARSER_WRAP_INC_DIR}/simdjson_parser.hpp "
    DESTINATION include
    COMPONENT parser_wrap_Development
)

#install(
#    TARGETS parser_wrap
#    EXPORT parser_wrapTargets
#    RUNTIME COMPONENT parser_wrap_Runtime
#    LIBRARY COMPONENT parser_wrap_Runtime
#    NAMELINK_COMPONENT parser_wrap_Development
#    ARCHIVE COMPONENT  parser_wrap_Development
#    INCLUDES DESTINATION ${PROJECT_NAME}
#)

# output Configure
configure_file(
    ${PROJECT_NAME}-config.cmake.in
    ${PROJECT_NAME}-config.cmake
    @ONLY
)

#write_basic_package_version_file(
#    ${PROJECT_NAME}-config-version.cmake
#    COMPATIBILITY SameMinorVersion
#)

set(
    PARSER_WRAP_CMAKE_DIR
        "${CMAKE_INSTALL_LIBDIR}/CMAKE/parser_wrap"
    CACHE STRING
        "CMake package config location relative to the install prefix"
)
mark_as_advanced(PARSER_WRAP_CMAKE_DIR)

install(
    FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.CMAKE   
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.CMAKE   
    DESTINATION 
        ${PARSER_WRAP_CMAKE_DIR}
    COMPONENT
        parser_wrap_Development
)
