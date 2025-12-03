
#pragma once

#include <memory>
#include <boost/unordered/unordered_flat_map.hpp>

#include "xyzlabs/object.hpp"

namespace xyzlabs {

class ObjectRegistry {
    boost::unordered::unordered_flat_map <uint64_t, std::unique_ptr<Object>> objects_;
public:
    ObjectRegistry() = default;
    void insert(std::unique_ptr<Object> obj);
    Object* get(uint64_t id);
    void remove(uint64_t id);
};

}
