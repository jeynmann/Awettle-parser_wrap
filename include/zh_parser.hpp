/**
 * @file zh_parser.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

namespace modern {
namespace parser {

#ifndef MODERN_PARSER_KEY_TYPE
#define MODERN_PARSER_KEY_TYPE std::string
#endif

template <typename _V>
struct ValueRef {
    using value_type = _V;
    explicit ValueRef(value_type& value) : value(value) {}
    value_type& value;
};

template <typename _K, typename _V>
struct KValueRef {
    using key_type = _K;
    using value_type = _V;
    explicit KValueRef(value_type& value) : value(value) {}
    KValueRef(const key_type& key, value_type& value) : key(key), value(value) {}
    KValueRef(key_type&& key, value_type& value) : key(std::move(key)), value(value) {}
    key_type key;
    value_type& value;
};

template <typename _V>
using KstrValueRef = KValueRef<MODERN_PARSER_KEY_TYPE, _V>;

/**
 * @brief
 * u can using Type::Json::BUILT_IN both as enum and type;
 * @example
 * Type::Json t = Type::BUILT_IN;
 * @example
 * declare:
 * void fun(const Type::Object&);
 * void fun(const Type::Array&);
 * call:
 * fun(Type::OBJECT);  // this will call 'fun(const Type::Object&)'
 * fun(Type::ARRAY);   // this will call 'fun(const Type::Array&)'
 */
struct Type {
    enum class Json {
        BUILT_IN = 0,
        ARRAY,
        OBJECT,
        INVALID,
    };
    static constexpr struct BuiltIn {
        constexpr operator Json() { return Json::BUILT_IN; }
    } BUILT_IN{};
    static constexpr struct Array {
        constexpr operator Json() { return Json::ARRAY; }
    } ARRAY{};
    static constexpr struct Object {
        constexpr operator Json() { return Json::OBJECT; }
    } OBJECT{};
};

}  // namespace parser
}  // namespace modern
