
#include "xyzlabs/objectregistry.hpp"

namespace xyzlabs {

Object* ObjectRegistry::insert(std::unique_ptr<Object> obj) {
    uint64_t id = obj->id();
    objects_[id] = std::move(obj);
    return objects_[id].get();
}

Object* ObjectRegistry::get(uint64_t id) {
    if(objects_.contains(id)) {
        return objects_[id].get();
    }

    return nullptr;
}

void ObjectRegistry::remove(uint64_t id) {
    objects_.erase(id);
}

};
