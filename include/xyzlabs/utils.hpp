
#ifndef UTILS_HPP
#define UTILS_HPP

#include <spdlog/spdlog.h>
#include <format>
#include <stdexcept>

namespace utils {
    // auxiliary function to log success or raise exception in case of fatal error
    void handle_event(const char *message, bool success) {
        if(success) {
            spdlog::info("{}: SUCCESS", message);
        } else {
            throw std::runtime_error(std::format("{} : FAIL", message));
        }
    }
}

#endif
