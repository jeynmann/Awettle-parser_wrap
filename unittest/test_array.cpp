/**
 * @file test_array.cpp
 * @author zhzh (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <fstream>
#include <iostream>

#include "assert.hpp"
#include "gtest/gtest.h"
#include "rapidjson_parser.hpp"
#include "simdjson_parser.hpp"

// example of Data
struct Data {
    int32_t i32{};
    float fa32[2]{};
    const char* ccp{};
    std::string str;
};

namespace modern {
namespace parser {
// to impl ValueParser of Data
namespace rapid {
template <>
struct ValueParser<Data> : public ValueRef<Data> {
    explicit ValueParser(Data& value) : ValueRef<Data>(value) {}
    bool operator()(const RapidVal& dom) {
        ArrayParser arr_parser;
        arr_parser.insert_builtin(value.i32);
        arr_parser.insert_builtin(value.fa32[0]);
        arr_parser.insert_builtin(value.fa32[1]);
        arr_parser.insert_builtin(value.ccp);
        arr_parser.insert_builtin(value.str);
        return arr_parser(dom);
    }
};
}  // namespace rapid
// to impl ValueParser of Data
namespace simd {
template <>
struct ValueParser<Data> : public ValueRef<Data> {
    explicit ValueParser(Data& value) : ValueRef<Data>(value) {}
    bool operator()(const SimdVal& dom) {
        ArrayParser arr_parser;
        arr_parser.insert_builtin(value.i32);
        arr_parser.insert_builtin(value.fa32[0]);
        arr_parser.insert_builtin(value.fa32[1]);
        arr_parser.insert_builtin(value.ccp);
        arr_parser.insert_builtin(value.str);
        return arr_parser(dom);
    }
};
}  // namespace simd

}  // namespace parser
}  // namespace modern

class test_array_val_json_parse : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// example user doc mixed with nums and data, has a json parser method
struct Doc {
    std::vector<int> nums;
    Data data;
    bool parse(const modern::parser::rapid::RapidVal& dom) {
        modern::parser::rapid::DocParser doc_parser;
        doc_parser.insert_builtin("nums", nums);
        doc_parser.insert_builtin("data", data);  // already impls ValueParser
        return doc_parser(dom);
    }
    bool parse(const modern::parser::simd::SimdVal& dom) {
        modern::parser::simd::DocParser doc_parser;
        doc_parser.insert_builtin("nums", nums);
        doc_parser.insert_builtin("data", data);  // already impls ValueParser
        return doc_parser(dom);
    }
};

TEST(test_array_val_json_parse, simd) {
    Doc doc;
    simdjson::dom::parser dom_parser;
    simdjson::dom::element dom = dom_parser.load("test_array.json");
    DEBUG_EQ(doc.parse(dom), true);
    DEBUG_EQ(doc.nums[0], 1111);
    DEBUG_EQ(doc.nums[1], 2222);
    DEBUG_EQ(doc.nums[2], 3333);
    DEBUG_EQ(doc.data.i32, 1111);
    DEBUG_FLOAT_EQ(doc.data.fa32[0], 2222.f);
    DEBUG_FLOAT_EQ(doc.data.fa32[1], 3333.f);
    DEBUG_STR_EQ(doc.data.ccp, "ccp");
    DEBUG_EQ(doc.data.str, "str");
}

TEST(test_array_val_json_parse, rapid) {
    Doc doc;
    std::ifstream ifs("test_array.json");
    std::stringstream buff;
    buff << ifs.rdbuf();

    rapidjson::Document dom;
    dom.Parse(buff.str().data());
    DEBUG_EQ(doc.parse(dom), true);
    DEBUG_EQ(doc.nums[0], 1111);
    DEBUG_EQ(doc.nums[1], 2222);
    DEBUG_EQ(doc.nums[2], 3333);
    DEBUG_EQ(doc.data.i32, 1111);
    DEBUG_FLOAT_EQ(doc.data.fa32[0], 2222.f);
    DEBUG_FLOAT_EQ(doc.data.fa32[1], 3333.f);
    DEBUG_STR_EQ(doc.data.ccp, "ccp");
    DEBUG_EQ(doc.data.str, "str");
}
