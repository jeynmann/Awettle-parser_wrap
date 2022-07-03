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
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "rapidjson/document.h"
#include "zh_parser.hpp"

namespace modern {
namespace parser {
namespace rapid {

using ParserTrait = std::function<bool(const rapidjson::Value&)>;
using RapidVal = rapidjson::Value;
using RapidCMemIter = rapidjson::Value::ConstMemberIterator;
using RapidCValIter = rapidjson::Value::ConstValueIterator;

/**
 * @brief
 * dont using rapidjson internal helper;
 */
#ifndef NO_RAPIDJSON_TYPE_HELPTER

template <typename ValueType, typename T>
using TypeHelper = rapidjson::internal::TypeHelper<ValueType, T>;

#else

template <typename ValueType, typename T>
struct TypeHelper {};

template <typename ValueType>
struct TypeHelper<ValueType, bool> {
    static bool Is(const ValueType& v) { return v.IsBool(); }
    static bool Get(const ValueType& v) { return v.GetBool(); }
    static ValueType& Set(ValueType& v, bool data) { return v.SetBool(data); }
    static ValueType& Set(ValueType& v, bool data, typename RapidVal::AllocatorType&) { return v.SetBool(data); }
};

template <typename ValueType>
struct TypeHelper<ValueType, int> {
    static bool Is(const ValueType& v) { return v.IsInt(); }
    static int Get(const ValueType& v) { return v.GetInt(); }
    static ValueType& Set(ValueType& v, int data) { return v.SetInt(data); }
    static ValueType& Set(ValueType& v, int data, typename RapidVal::AllocatorType&) { return v.SetInt(data); }
};

template <typename ValueType>
struct TypeHelper<ValueType, unsigned> {
    static bool Is(const ValueType& v) { return v.IsUint(); }
    static unsigned Get(const ValueType& v) { return v.GetUint(); }
    static ValueType& Set(ValueType& v, unsigned data) { return v.SetUint(data); }
    static ValueType& Set(ValueType& v, unsigned data, typename RapidVal::AllocatorType&) { return v.SetUint(data); }
};

#ifdef _MSC_VER
RAPIDJSON_STATIC_ASSERT(sizeof(long) == sizeof(int));
template <typename ValueType>
struct TypeHelper<ValueType, long> {
    static bool Is(const ValueType& v) { return v.IsInt(); }
    static long Get(const ValueType& v) { return v.GetInt(); }
    static ValueType& Set(ValueType& v, long data) { return v.SetInt(data); }
    static ValueType& Set(ValueType& v, long data, typename RapidVal::AllocatorType&) { return v.SetInt(data); }
};

RAPIDJSON_STATIC_ASSERT(sizeof(unsigned long) == sizeof(unsigned));
template <typename ValueType>
struct TypeHelper<ValueType, unsigned long> {
    static bool Is(const ValueType& v) { return v.IsUint(); }
    static unsigned long Get(const ValueType& v) { return v.GetUint(); }
    static ValueType& Set(ValueType& v, unsigned long data) { return v.SetUint(data); }
    static ValueType& Set(ValueType& v, unsigned long data, typename RapidVal::AllocatorType&) { return v.SetUint(data); }
};
#endif

template <typename ValueType>
struct TypeHelper<ValueType, int64_t> {
    static bool Is(const ValueType& v) { return v.IsInt64(); }
    static int64_t Get(const ValueType& v) { return v.GetInt64(); }
    static ValueType& Set(ValueType& v, int64_t data) { return v.SetInt64(data); }
    static ValueType& Set(ValueType& v, int64_t data, typename RapidVal::AllocatorType&) { return v.SetInt64(data); }
};

template <typename ValueType>
struct TypeHelper<ValueType, uint64_t> {
    static bool Is(const ValueType& v) { return v.IsUint64(); }
    static uint64_t Get(const ValueType& v) { return v.GetUint64(); }
    static ValueType& Set(ValueType& v, uint64_t data) { return v.SetUint64(data); }
    static ValueType& Set(ValueType& v, uint64_t data, typename RapidVal::AllocatorType&) { return v.SetUint64(data); }
};

template <typename ValueType>
struct TypeHelper<ValueType, double> {
    static bool Is(const ValueType& v) { return v.IsDouble(); }
    static double Get(const ValueType& v) { return v.GetDouble(); }
    static ValueType& Set(ValueType& v, double data) { return v.SetDouble(data); }
    static ValueType& Set(ValueType& v, double data, typename RapidVal::AllocatorType&) { return v.SetDouble(data); }
};

template <typename ValueType>
struct TypeHelper<ValueType, float> {
    static bool Is(const ValueType& v) { return v.IsFloat(); }
    static float Get(const ValueType& v) { return v.GetFloat(); }
    static ValueType& Set(ValueType& v, float data) { return v.SetFloat(data); }
    static ValueType& Set(ValueType& v, float data, typename RapidVal::AllocatorType&) { return v.SetFloat(data); }
};

template <typename ValueType>
struct TypeHelper<ValueType, const typename RapidVal::Ch*> {
    typedef const typename RapidVal::Ch* StringType;
    static bool Is(const ValueType& v) { return v.IsString(); }
    static StringType Get(const ValueType& v) { return v.GetString(); }
    static ValueType& Set(ValueType& v, const StringType data) { return v.SetString(typename RapidVal::StringRefType(data)); }
    static ValueType& Set(ValueType& v, const StringType data, typename RapidVal::AllocatorType& a) {
        return v.SetString(data, a);
    }
};

#if RAPIDJSON_HAS_STDSTRING
template <typename ValueType>
struct TypeHelper<ValueType, std::basic_string<typename RapidVal::Ch>> {
    typedef std::basic_string<typename RapidVal::Ch> StringType;
    static bool Is(const ValueType& v) { return v.IsString(); }
    static StringType Get(const ValueType& v) { return StringType(v.GetString(), v.GetStringLength()); }
    static ValueType& Set(ValueType& v, const StringType& data, typename RapidVal::AllocatorType& a) {
        return v.SetString(data, a);
    }
};
#endif

template <typename ValueType>
struct TypeHelper<ValueType, typename RapidVal::Array> {
    typedef typename RapidVal::Array ArrayType;
    static bool Is(const ValueType& v) { return v.IsArray(); }
    static ArrayType Get(ValueType& v) { return v.GetArray(); }
    static ValueType& Set(ValueType& v, ArrayType data) { return v = data; }
    static ValueType& Set(ValueType& v, ArrayType data, typename RapidVal::AllocatorType&) { return v = data; }
};

template <typename ValueType>
struct TypeHelper<ValueType, typename RapidVal::ConstArray> {
    typedef typename RapidVal::ConstArray ArrayType;
    static bool Is(const ValueType& v) { return v.IsArray(); }
    static ArrayType Get(const ValueType& v) { return v.GetArray(); }
};

template <typename ValueType>
struct TypeHelper<ValueType, typename RapidVal::Object> {
    typedef typename RapidVal::Object ObjectType;
    static bool Is(const ValueType& v) { return v.IsObject(); }
    static ObjectType Get(ValueType& v) { return v.GetObject(); }
    static ValueType& Set(ValueType& v, ObjectType data) { return v = data; }
    static ValueType& Set(ValueType& v, ObjectType data, typename RapidVal::AllocatorType&) { return v = data; }
};

template <typename ValueType>
struct TypeHelper<ValueType, typename RapidVal::ConstObject> {
    typedef typename RapidVal::ConstObject ObjectType;
    static bool Is(const ValueType& v) { return v.IsObject(); }
    static ObjectType Get(const ValueType& v) { return v.GetObject(); }
};

#endif

// todo: support tuple

template <typename T>
struct ParseHelper {
    constexpr ParseHelper() {}
    bool is(const RapidVal& rapidval) const { return TypeHelper<RapidVal, T>::Is(rapidval); }
    T get(const RapidVal& rapidval) const { return TypeHelper<RapidVal, T>::Get(rapidval); }
};

template <>
struct ParseHelper<std::string> {
    constexpr ParseHelper() {}
    bool is(const RapidVal& rapidval) const { return TypeHelper<RapidVal, const char*>::Is(rapidval); }
    std::string get(const RapidVal& rapidval) const { return TypeHelper<RapidVal, const char*>::Get(rapidval); }
};

template <typename __V>
struct ValueParser : public ValueRef<__V> {
    using super_type = ValueRef<__V>;
    using value_type = typename super_type::value_type;
    using super_type::value;
    explicit ValueParser(value_type& value) : super_type(value) {}
    bool operator()(const RapidVal& dom) {
        if (ParseHelper<value_type>().is(dom)) {
            value = ParseHelper<value_type>().get(dom);
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
    bool operator()(const RapidVal& dom) {
        if (ParseHelper<RapidVal::Array>().is(dom)) {
            const size_t M = std::min((size_t)dom.Size(), N);
            RapidCValIter sub_iter = dom.Begin();
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
    bool operator()(const RapidVal& dom) {
        if (ParseHelper<RapidVal::Array>().is(dom)) {
            const size_t M = (size_t)dom.Size();
            RapidCValIter sub_iter = dom.Begin();
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

    bool operator()(const RapidVal& dom) {
        if (ParseHelper<RapidVal::ConstArray>().is(dom)) {
            const size_t N = value.size();
            const size_t M = std::min((size_t)dom.Size(), N);
            RapidCValIter sub_iter = dom.Begin();
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

    bool operator()(const RapidVal& dom) {
        if (ParseHelper<RapidVal::ConstObject>().is(dom)) {
            const auto val_end = value.end();
            const size_t N = value.size();
            size_t i = 0;

            const RapidCMemIter sub_end = dom.MemberEnd();
            RapidCMemIter sub_iter = dom.MemberBegin();
            for (; i != N && sub_iter != sub_end; ++sub_iter) {
                /** @note treat key as string */
                auto val_iter = value.find(sub_iter->name.GetString());
                if (val_iter != val_end) {
                    val_iter->second(sub_iter->value);
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
        value.emplace(std::move(key), std::forward<__P>(parser));
    }
    template <typename __P>
    void insert(const key_type& key, __P&& parser) {
        value.emplace(key, std::forward<__P>(parser));
    }
    void clear() { value.clear(); }
    void reset() { value_type{}.swap(value); }

    value_type value;
};

using DocParser = ObjectParser;

}  // namespace rapid
}  // namespace parser
}  // namespace modern
