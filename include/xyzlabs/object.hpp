
#pragma once

#include <cinttypes>
#include <concepts>

namespace xyzlabs {

class Object {
    uint64_t id_;
public:
    Object();
    virtual ~Object() = default;
    uint64_t id();
};

template <typename T>
concept ObjectType = std::derived_from<T, Object> || std::same_as<T, Object>;

}
