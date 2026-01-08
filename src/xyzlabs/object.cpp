
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/object.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

Object::Object(): id_(XYZLabs::random_generator()()) {}

uint64_t Object::id() {
    return id_;
}

}
