
#include "xyzlabs/objectregistry.hpp"

namespace xyzlabs {

void ObjectRegistry::remove(uint64_t id) {
    objects_.erase(id);
}

Object* ObjectRegistry::get(uint64_t id) {
    if(objects_.contains(id)) {
        return objects_[id].get();
    }

    return nullptr;
}

void ObjectRegistry::clear() {
    objects_.clear();
}

size_t ObjectRegistry::size() const {
    return objects_.size();
}

bool ObjectRegistry::contains(uint64_t id) const {
    return objects_.contains(id);
}

};
