/**
 * @file rapidjson_parser.hpp
 * @author zhzh (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-04-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <stdint.h>

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "simdjson.h"
#include "zh_parser.hpp"

namespace modern {
namespace parser {
namespace simd {

using ParserTrait = std::function<bool(const simdjson::dom::element&)>;
using SimdVal = simdjson::dom::element;
using SimdArr = simdjson::dom::array;
using SimdObj = simdjson::dom::object;
using SimdErr = simdjson::error_code;

template <typename T>
struct TypeHelper {
    using value_type = T;
    using parse_type = T;
};

template <>
struct TypeHelper<int32_t> {
    using value_type = int32_t;
    using parse_type = int64_t;
};

template <>
struct TypeHelper<uint32_t> {
    using value_type = uint32_t;
    using parse_type = uint64_t;
};

template <>
struct TypeHelper<float> {
    using value_type = float;
    using parse_type = double;
};

template <>
struct TypeHelper<std::string> {
    using value_type = std::string;
    using parse_type = const char*;
};

template <typename T>
struct ParseHelper : public TypeHelper<T> {
    using super_type = TypeHelper<T>;
    using value_type = typename TypeHelper<T>::value_type;
    using parse_type = typename TypeHelper<T>::parse_type;
    bool is(const SimdVal& dom) const { return dom.is<parse_type>(); }
    value_type get(const SimdVal& dom) const {
        auto [_val, _err] = dom.get<parse_type>();
        return value_type(_val);
    }
};

template <typename __V>
struct ValueParser : public ValueRef<__V> {
    using super_type = ValueRef<__V>;
    using value_type = typename super_type::value_type;
    using parse_type = typename TypeHelper<__V>::parse_type;
    using super_type::value;
    explicit ValueParser(value_type& value) : super_type(value) {}
    bool operator()(const SimdVal& dom) {
        parse_type _val;
        if (dom.get(_val) == SimdErr::SUCCESS) {
            value = value_type(_val);
            return true;
        }
        return false;
    }
};

template <typename __V, const size_t N>
struct ValueParser<__V[N]> : public ValueRef<__V[N]> {
    using super_type = ValueRef<__V[N]>;
    using value_type = typename super_type::value_type;
    using super_type::value;
    explicit ValueParser(value_type& value) : super_type(value) {}
    bool operator()(const SimdVal& dom) {
        SimdArr simd_arr;
        if (dom.get(simd_arr) == SimdErr::SUCCESS) {
            auto sub_iter = simd_arr.begin();
            const size_t M = std::min((size_t)simd_arr.size(), N);
            size_t i = 0;
            for (; i != M && ValueParser<__V>{value[i]}(*sub_iter); ++i, ++sub_iter) {
            }
            return i == M;
        }
        return false;
    }
};

template <typename __V>
struct ValueParser<std::vector<__V>> : public ValueRef<std::vector<__V>> {
    using super_type = ValueRef<std::vector<__V>>;
    using value_type = typename super_type::value_type;
    using super_type::value;
    explicit ValueParser(value_type& value) : super_type(value) {}
    bool operator()(const SimdVal& dom) {
        SimdArr simd_arr;
        if (dom.get(simd_arr) == SimdErr::SUCCESS) {
            auto sub_iter = simd_arr.begin();
            const size_t M = (size_t)simd_arr.size();
            size_t i = 0;
            value.resize(M);
            for (; i != M && ValueParser<__V>{value[i]}(*sub_iter); ++i, ++sub_iter) {
            }
            return i == M;
        }
        return false;
    }
};

struct ArrayParser {
    using value_type = std::vector<ParserTrait>;

    ArrayParser() = default;
    ArrayParser(std::initializer_list<ParserTrait> init_list) : value{init_list} {}

    ArrayParser& operator=(std::initializer_list<ParserTrait> init_list) {
        value = init_list;
        return *this;
    }

    bool operator()(const SimdVal& dom) {
        SimdArr simd_arr;
        if (dom.get(simd_arr) == SimdErr::SUCCESS) {
            auto sub_iter = simd_arr.begin();
            const size_t N = value.size();
            const size_t M = std::min((size_t)simd_arr.size(), N);
            size_t i = 0;
            for (; i != M && value[i](*sub_iter); ++i, ++sub_iter) {
            }
            return i == M;
        }
        return false;
    }

    void assign(std::initializer_list<ParserTrait> init_list) { value = init_list; }
    template <typename __V>
    void insert_builtin(__V& val) {
        value.emplace_back(ValueParser<__V>(val));
    }
    void insert(ParserTrait&& parser) { value.emplace_back(std::move(parser)); }
    void insert(const ParserTrait& parser) { value.emplace_back(parser); }
    void clear() { value.clear(); }
    void reset() { value_type{}.swap(value); }

    value_type value;
};

struct ObjectParser {
    using key_type = KstrValueRef<size_t>::key_type;
    using value_type = std::unordered_map<key_type, ParserTrait>;
    using pair_type = value_type::value_type;

    ObjectParser() = default;
    ObjectParser(std::initializer_list<pair_type> init_list) : value{init_list} {}
    ObjectParser& operator=(std::initializer_list<pair_type> init_list) {
        value = init_list;
        return *this;
    }

    bool operator()(const SimdVal& dom) {
        SimdObj simd_obj;
        if (dom.get(simd_obj) == SimdErr::SUCCESS) {
            const auto val_end = value.end();
            const size_t N = value.size();
            size_t i = 0;

            const auto sub_end = simd_obj.end();
            auto sub_iter = simd_obj.begin();
            for (; i != N && sub_iter != sub_end; ++sub_iter) {
                auto val_iter = value.find(key_type{sub_iter.key()});
                if (val_iter != val_end) {
                    val_iter->second(sub_iter.value());
                    ++i;
                }
            }
            return i == N;
        }
        return false;
    }

    void assign(std::initializer_list<pair_type> init_list) { value = init_list; }
    template <typename __V>
    void insert_builtin(key_type&& key, __V& val) {
        value.emplace(std::move(key), ValueParser<__V>(val));
    }
    template <typename __V>
    void insert_builtin(const key_type& key, __V& val) {
        value.emplace(key, ValueParser<__V>(val));
    }
    template <typename __P>
    void insert(key_type&& key, __P&& parser) {
        value.emplace(key, std::forward<__P>(parser));
    }
    template <typename __P>
    void insert(const key_type& key, __P&& parser) {
        value.emplace(std::move(key), std::forward<__P>(parser));
    }
    void clear() { value.clear(); }
    void reset() { value_type{}.swap(value); }

    value_type value;
};

using DocParser = ObjectParser;

}  // namespace simd
}  // namespace parser
}  // namespace modern
