/**
 * @file util.hpp
 * @author zhzh name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "log.hpp"

#define DEBUG(a, b) \
    LOG(info) << "\"" #a " == " #b << "\" : " << ((a) == (b)) << "\n"
#define DEBUG_STR(a, b) \
    LOG(info) << "\"" #a " == " #b << "\" : " << (std::string(a) == (b)) << "\n"

#ifdef DEBUG_UT  // these macros will not block ut case
#define DEBUG_EQ(a, b) DEBUG(a, b)
#define DEBUG_FLOAT_EQ(a, b) DEBUG(a, b)
#define DEBUG_DOUBLE_EQ(a, b) DEBUG(a, b)
#define DEBUG_STR_EQ(a, b) DEBUG_STR(a, b)
#define DEBUG_NE(a, b) DEBUG(a, b)
#define DEBUG_STR_NE(a, b) DEBUG_STR(a, b)
#else
#define DEBUG_EQ(a, b) ASSERT_EQ(a, b)
#define DEBUG_FLOAT_EQ(a, b) ASSERT_FLOAT_EQ(a, b)
#define DEBUG_DOUBLE_EQ(a, b) ASSERT_DOUBLE_EQ(a, b)
#define DEBUG_STR_EQ(a, b) ASSERT_STREQ(a, b)
#define DEBUG_NE(a, b) ASSERT_NE(a, b)
#define DEBUG_STR_NE(a, b) ASSERT_STRNE(a, b)
#endif
