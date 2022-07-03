/**
 * @file test_object.cpp
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

struct Data {
    int32_t i32{};
    float fa32[2]{};
    const char* ccp{};
    std::string str{};
    std::vector<int> nums{};
};

struct Doc {
    Data data;
    bool parse(const modern::parser::rapid::RapidVal& dom) {
        modern::parser::rapid::DocParser doc_parser;

        modern::parser::rapid::ArrayParser arr_parser;
        arr_parser.insert_builtin(data.i32);
        arr_parser.insert_builtin(data.fa32[0]);
        arr_parser.insert_builtin(data.fa32[1]);
        arr_parser.insert_builtin(data.ccp);
        arr_parser.insert_builtin(data.str);

        doc_parser.insert("data", arr_parser);  // insert parser directly
        doc_parser.insert_builtin("nums", data.nums);
        return doc_parser(dom);
    }
    bool parse(const modern::parser::simd::SimdVal& dom) {
        modern::parser::simd::DocParser doc_parser;

        modern::parser::simd::ArrayParser arr_parser;
        arr_parser.insert_builtin(data.i32);
        arr_parser.insert_builtin(data.fa32[0]);
        arr_parser.insert_builtin(data.fa32[1]);
        arr_parser.insert_builtin(data.ccp);
        arr_parser.insert_builtin(data.str);

        doc_parser.insert("data", arr_parser);  // insert parser directly
        doc_parser.insert_builtin("nums", data.nums);
        return doc_parser(dom);
    }
};

class test_object_val_json_parse : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST(test_object_val_json_parse, simd) {
    Doc doc;
    simdjson::dom::parser dom_parser;
    simdjson::dom::element dom = dom_parser.load("test_object.json");
    DEBUG_EQ(doc.parse(dom), true);
    DEBUG_EQ(doc.data.nums[0], 1111);
    DEBUG_EQ(doc.data.nums[1], 2222);
    DEBUG_EQ(doc.data.nums[2], 3333);
    DEBUG_EQ(doc.data.i32, 1111);
    DEBUG_FLOAT_EQ(doc.data.fa32[0], 2222.f);
    DEBUG_FLOAT_EQ(doc.data.fa32[1], 3333.f);
    DEBUG_STR_EQ(doc.data.ccp, "ccp");
    DEBUG_EQ(doc.data.str, "str");
}

TEST(test_object_val_json_parse, rapid) {
    Doc doc;
    std::ifstream ifs("test_object.json");
    std::stringstream buff;
    buff << ifs.rdbuf();

    rapidjson::Document dom;
    dom.Parse(buff.str().data());
    DEBUG_EQ(doc.parse(dom), true);
    DEBUG_EQ(doc.data.nums[0], 1111);
    DEBUG_EQ(doc.data.nums[1], 2222);
    DEBUG_EQ(doc.data.nums[2], 3333);
    DEBUG_EQ(doc.data.i32, 1111);
    DEBUG_FLOAT_EQ(doc.data.fa32[0], 2222.f);
    DEBUG_FLOAT_EQ(doc.data.fa32[1], 3333.f);
    DEBUG_STR_EQ(doc.data.ccp, "ccp");
    DEBUG_EQ(doc.data.str, "str");
}
