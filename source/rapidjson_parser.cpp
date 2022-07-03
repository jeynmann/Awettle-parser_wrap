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

#include "rapidjson_parser.hpp"

namespace modern {
namespace parser {
namespace rapid {

using I32Parser = ValueParser<int32_t>;
using U32Parser = ValueParser<uint32_t>;
using I64Parser = ValueParser<int64_t>;
using U64Parser = ValueParser<uint64_t>;
using F32Parser = ValueParser<float>;
using F64Parser = ValueParser<double>;
using STRParser = ValueParser<std::string>;
using CCPParser = ValueParser<const char*>;

}  // namespace rapid
}  // namespace parser

}  // namespace modern