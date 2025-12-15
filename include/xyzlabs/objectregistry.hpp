
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
    Object* get(uint64_t id);
    template<ObjectType O = Object>
    O* get_as(uint64_t id);
    void remove(uint64_t id);
};

template<ObjectType O>
O* ObjectRegistry::insert(std::unique_ptr<O> obj) {
    auto ptr = obj.get();
    uint64_t id = obj->id();
    std::unique_ptr<Object> proxy = std::move(obj);
    objects_[id] = std::move(proxy);
    return ptr;
}

template<ObjectType O>
O* ObjectRegistry::get_as(uint64_t id) {
    return dynamic_cast<O*>(get(id));
}


}
