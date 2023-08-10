#ifndef EYPS_UTIL_H
#define EYPS_UTIL_H

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <string_view>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"


template <typename DATA_TYPE>
constexpr inline DATA_TYPE ReverseEndianness(DATA_TYPE data) noexcept
{
    auto start = reinterpret_cast<unsigned char*>(&data);
    auto end = start + sizeof(DATA_TYPE);
    std::reverse(start, end);
    return data;
}

inline void jsonParse(rapidjson::Document& doc, std::string_view& data) {
    doc.Parse(data.data());
}

#endif //EYPS_UTIL_H
