
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

};
