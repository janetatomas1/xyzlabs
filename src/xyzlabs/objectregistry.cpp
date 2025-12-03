
#include "xyzlabs/objectregistry.hpp"

namespace xyzlabs {

void ObjectRegistry::insert(std::unique_ptr<Object> obj) {
    uint64_t id = obj->id();
    objects_[id] = std::move(obj);
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
