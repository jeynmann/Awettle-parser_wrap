/**
 * @file debug.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-05-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <vector>

namespace std {

template <typename T>
ostream& operator<<(ostream& ost, vector<T> vec) {
    ost << '\n';
    for (auto& ele : vec) {
        ost << ele << ',';
    }
    return ost << '\n';
}

}  // namespace std
