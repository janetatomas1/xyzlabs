
#include "xyzlabs/objectregistry.hpp"

namespace xyzlabs {

void ObjectRegistry::remove(uint64_t id) {
    objects_.erase(id);
}

};
