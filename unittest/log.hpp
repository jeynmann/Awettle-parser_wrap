/**
 * @file log.hpp
 * @author zhzh (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#ifndef LOG
#define LOG(level)                                         \
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "]" \
              << "(" << #level << ")"
#endif
#ifndef LINE
#define LINE(level) \
    std::cout << "================" << #level << "================" << '\n'
#endif
