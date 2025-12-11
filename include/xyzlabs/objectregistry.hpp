
#pragma once

#include <memory>
#include <boost/unordered/unordered_flat_map.hpp>

#include "xyzlabs/object.hpp"
#include "xyzlabs/object.hpp"

namespace xyzlabs {

class ObjectRegistry {
    boost::unordered::unordered_flat_map <uint64_t, std::unique_ptr<Object>> objects_;
public:
    ObjectRegistry() = default;
    template<ObjectType O = Object>
    O* insert(std::unique_ptr<O> obj);
    template<ObjectType O = Object>
    O* get(uint64_t id);
    void remove(uint64_t id);
};

template<ObjectType O>
O* ObjectRegistry::insert(std::unique_ptr<O> obj) {
    uint64_t id = obj->id();
    objects_[id] = std::move(obj);
    return objects_[id].get();
}

template<ObjectType O>
O* ObjectRegistry::get(uint64_t id) {
    if(objects_.contains(id)) {
        return objects_[id].get();
    }
    return nullptr;
}

}
