
#include <gtest/gtest.h>
#include <xyzlabs/objectregistry.hpp>

using namespace xyzlabs;

TEST(ObjectRegistryTest, RegisterObject) {
    ObjectRegistry registry;
    auto obj = std::make_unique<Object>();
    EXPECT_NE(registry.insert(std::move(obj)), nullptr);
    EXPECT_EQ(registry.size(), 1);
}

TEST(ObjectRegistryTest, RemoveObject) {
    ObjectRegistry registry;
    auto obj = std::make_unique<Object>();
    registry.remove(obj->id());
    EXPECT_EQ(registry.size(), 0);
    EXPECT_NO_THROW(registry.remove(0));
}

TEST(ObjectRegistryTest, ClearRegistry) {
    ObjectRegistry registry;
    auto obj1 = std::make_unique<Object>();
    auto obj2 = std::make_unique<Object>();
    registry.insert(std::move(obj1));
    registry.insert(std::move(obj2));
    registry.clear();
    EXPECT_EQ(registry.size(), 0);
}

TEST(ObjectRegistryTest, Get) {
    ObjectRegistry registry;
    auto obj = registry.insert(std::make_unique<Object>());
    EXPECT_EQ(registry.get(obj->id()), obj);
}

TEST(ObjectRegistryTest, InsertOwnership) {
    ObjectRegistry registry;
    auto obj = std::make_unique<Object>();
    registry.insert(std::move(obj));
    EXPECT_EQ(obj, nullptr);
}

TEST(ObjectRegistryTest, Contains) {
    ObjectRegistry registry;
    auto obj = registry.insert(std::make_unique<Object>());
    EXPECT_TRUE(registry.contains(obj->id()));
    EXPECT_FALSE(registry.contains(obj->id() + 1));
}

TEST(ObjectRegistryTest, GetUnknownID) {
    ObjectRegistry registry;
    EXPECT_EQ(registry.get(1), nullptr);
}

TEST(ObjectRegistryTest, GetMultipleCalls) {
    ObjectRegistry registry;
    auto obj = registry.insert(std::make_unique<Object>());
    EXPECT_EQ(registry.get(obj->id()), registry.get(obj->id()));
}

TEST(ObjectRegistryTest, GetAs) {
    struct A: public Object {};
    struct B: public Object {};

    ObjectRegistry registry;
    auto obj = registry.insert(std::make_unique<A>());
    EXPECT_EQ(registry.get_as<A>(obj->id()), obj);
    EXPECT_EQ(registry.get_as<B>(obj->id()), nullptr);
    EXPECT_EQ(registry.get_as<Object>(obj->id()), obj);
}

TEST(ObjectRegistryTest, Mutation) {
    struct A: public Object {
        size_t x;
    };

    ObjectRegistry registry;
    auto obj = registry.insert(std::make_unique<A>());
    obj->x = 42;

    EXPECT_EQ(registry.get_as<A>(obj->id())->x, obj->x);
}
