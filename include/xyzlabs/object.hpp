
#pragma once

#include <cinttypes>

namespace xyzlabs {

class Object {
    uint64_t id_;
public:
    Object();
    virtual ~Object() = default;
    uint64_t id();
};

}
