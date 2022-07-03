/**
 * @file test_builtin.cpp
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
#include <sstream>

#include "assert.hpp"
#include "gtest/gtest.h"
#include "rapidjson_parser.hpp"
#include "simdjson_parser.hpp"

// example data with a json parser method
struct Data {
    int32_t i32{};
    float fa32[2]{};
    const char* ccp{};
    std::string str{};
    bool parse(const modern::parser::rapid::RapidVal& dom) {
        modern::parser::rapid::DocParser doc_parser;
        doc_parser.insert_builtin("i32", i32);
        doc_parser.insert_builtin("fa32", fa32);
        doc_parser.insert_builtin("ccp", ccp);
        doc_parser.insert_builtin("str", str);
        return doc_parser(dom);
    }
    bool parse(const modern::parser::simd::SimdVal& dom) {
        modern::parser::simd::DocParser doc_parser;
        doc_parser.insert_builtin("i32", i32);
        doc_parser.insert_builtin("fa32", fa32);
        doc_parser.insert_builtin("ccp", ccp);
        doc_parser.insert_builtin("str", str);
        return doc_parser(dom);
    }
};

class test_builtin_val_json_parse : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST(test_builtin_val_json_parse, simd) {
    Data data;
    simdjson::dom::parser dom_parser;
    simdjson::dom::element dom = dom_parser.load("test_builtin.json");
    DEBUG_EQ(data.parse(dom), true);
    DEBUG_EQ(data.i32, 1111);
    DEBUG_FLOAT_EQ(data.fa32[0], 2222.f);
    DEBUG_FLOAT_EQ(data.fa32[1], 3333.f);
    DEBUG_STR_EQ(data.ccp, "ccp");
    DEBUG_EQ(data.str, "str");
}

TEST(test_builtin_val_json_parse, rapid) {
    Data data;
    std::ifstream ifs("test_builtin.json");
    std::stringstream buff;
    buff << ifs.rdbuf();

    rapidjson::Document dom;
    dom.Parse(buff.str().data());
    DEBUG_EQ(data.parse(dom), true);
    DEBUG_EQ(data.i32, 1111);
    DEBUG_FLOAT_EQ(data.fa32[0], 2222.f);
    DEBUG_FLOAT_EQ(data.fa32[1], 3333.f);
    DEBUG_STR_EQ(data.ccp, "ccp");
    DEBUG_EQ(data.str, "str");
}
