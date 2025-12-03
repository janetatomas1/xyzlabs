
#include "xyzlabs/globals.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/object.hpp"

namespace xyzlabs {

Object::Object(): id_(random_generator()()) {}

uint64_t Object::id() {
    return id_;
}

}
