
#pragma once

#include <spdlog/spdlog.h>
#include <format>
#include <stdexcept>

namespace utils {
    std::string standardize(const std::string &ss) {
        std::string s = ss;
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
        s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); }), s.end());
        return s;
    }
}
