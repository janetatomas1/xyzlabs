
#pragma once

#include <cinttypes>
#include <concepts>

namespace xyzlabs {

/**
* @brief Base class representing a uniquely identifiable object.
*
* @details
* Object provides a polymorphic base for all objects stored in
* ObjectRegistry. Each object has a unique numeric identifier
* generated at construction using the project-wide random generator.
*
* Derived classes should implement additional data and functionality,
* but must not override the ID uniqueness invariant.
*
* @par Ownership
* Objects are managed externally, usually via std::unique_ptr in ObjectRegistry.
* The base class itself does not manage ownership.
*
* @par Type Requirements
* Polymorphic (virtual destructor) to allow safe dynamic_cast.
*
* @par Thread Safety
* The base class is not thread-safe. Synchronization is the responsibility
* of the caller if objects are accessed concurrently.
*/
class Object {
    uint64_t id_;
public:
    /**
    * @brief Constructs a new Object with a unique ID.
    *
    * @details The ID is generated using the project-wide random generator.
    * For testing purposes, a deterministic ID can be passed explicitly.
    */
    Object();
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

    Object(Object&&) = default;
    Object& operator=(Object&&) = default;

    virtual ~Object() = default;
    uint64_t id() const;
};

/**
 * @brief Concept representing valid types for ObjectRegistry.
 *
 * @tparam T Type to check.
 *
 * @details
 * A type satisfies ObjectType if it is either Object itself or publicly
 * derives from Object. This is used to constrain ObjectRegistry template
 * parameters and ensure type safety.
 *
 * @note Object must be polymorphic for dynamic_cast to work correctly
 */
template <typename T>
concept ObjectType = std::derived_from<T, Object> || std::same_as<T, Object>;

} // namespace xyzlabs
