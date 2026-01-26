
#pragma once

#include <memory>
#include <boost/unordered/unordered_flat_map.hpp>

#include "xyzlabs/assert.hpp"
#include "xyzlabs/object.hpp"

namespace xyzlabs {

/**
* @brief Owns and manages a collection of Object instances indexed by ID.
*
* @details
* ObjectRegistry provides exclusive ownership and lifetime management for
* dynamically allocated Object instances. Objects are stored by their unique
* numeric identifier and can be retrieved by ID and optionally cast to a
* derived type.
*
* @par Ownership
* - The registry takes exclusive ownership of all inserted objects.
* - Returned pointers are non-owning and valid until the object is removed
*   or the registry is destroyed.
*
* @par Thread Safety
* - This class is not thread-safe.
* - All access must be externally synchronized.
*
* @par Type Requirements
* - Object must be polymorphic (have a virtual destructor).
* - Object::id() must return a unique and stable identifier.
*/
class ObjectRegistry {
    boost::unordered::unordered_flat_map <uint64_t, std::unique_ptr<Object>> objects_;
public:
    ObjectRegistry() = default;

    /**
     * @brief Insert an object into the registry.
     *
     * @tparam O Concrete Object type.
     * @param obj Object to insert. Ownership is transferred to the registry.
     * @return Non-owning pointer to the inserted object.
     *
     * @pre obj != nullptr
     * @pre obj->id() is unique or intended to replace an existing object
     *
     * @post The registry owns the object exclusively.
     *
     * @warning Replaces and destroys any existing object with the same ID.
     */
    template<ObjectType O = Object>
    O* insert(std::unique_ptr<O> obj);

    /**
     * @brief Retrieve an object by ID.
     *
     * @param id Object identifier.
     * @return Pointer to the object if found, nullptr otherwise.
     *
     * @note Returned pointer is non-owning.
     */
    Object* get(uint64_t id);


    template<ObjectType O = Object>
    O* get_as(uint64_t id);

    /**
    * @brief Remove and destroy an object by ID.
    *
    * @param id Object identifier.
    *
    * @post Any previously obtained pointer to the object becomes invalid.
    */
    void remove(uint64_t id);

    /**
     * @brief Remove and destroy all objects in the registry.
     *
     * @post The registry is empty.
     */
    void clear();

    /**
     * @brief Retrieve the number of objects in the registry.
     *
     * @return Number of objects.
     */
    size_t size() const;

    /**
     * @brief Check if an object with the given ID exists in the registry.
     *
     * @param id Object identifier.
     * @return True if the object exists, false otherwise.
     */
    bool contains(uint64_t id) const;
};

template<ObjectType O>
O* ObjectRegistry::insert(std::unique_ptr<O> obj) {
    XYZ_ASSERT_MSG(obj != nullptr, "Object pointer cannot be null");
    auto ptr = obj.get();
    uint64_t id = obj->id();
    objects_[id] = std::move(obj);
    return ptr;
}

template<ObjectType O>
O* ObjectRegistry::get_as(uint64_t id) {
    return dynamic_cast<O*>(get(id));
}

}
