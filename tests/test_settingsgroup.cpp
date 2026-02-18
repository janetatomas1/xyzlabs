
#include <gtest/gtest.h>
#include <xyzlabs/settings/settingsgroup.hpp>
#include <xyzlabs/settings/setting.hpp>
#include <boost/exception/all.hpp>
#include <exception>

using namespace xyzlabs;

TEST(SettingsGroup, AddAndGetSetting) {
    SettingsGroup g;
    auto s = std::make_unique<FloatSetting>("foo", 1.0f);
    auto *ptr = g.add_setting("foo", std::move(s));

    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(ptr, g.get("foo"));
}

TEST(SettingsGroup, DuplicateSetting) {
    SettingsGroup g;

    auto ptr0 = g.add_setting("foo", std::make_unique<FloatSetting>("foo", 1.0f));
    auto ptr1 = g.add_setting("foo", std::make_unique<FloatSetting>("foo", 2.0f));

    EXPECT_EQ(g.get("foo"), ptr0);
    EXPECT_EQ(nullptr, ptr1);
}

TEST(SettingsGroup, AddChildAndGetChild) {
    SettingsGroup g;

    auto child = std::make_unique<SettingsGroup>();
    auto* ptr = g.add_child("sub", std::move(child));

    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(ptr, g.get_child("sub"));
}

TEST(SettingsGroup, NestedPath) {
    SettingsGroup g;
    g.add_setting("a.b.c", std::make_unique<FloatSetting>("bar", 1.0f));

    EXPECT_NE(g.get("a.b.c"), nullptr);
    EXPECT_NE(g.get_child("a"), nullptr);
}

TEST(SettingsGroup, InvalidJsonDoesNotCrash) {
    SettingsGroup g;
    json j = { {"unexpected", 123} };

    EXPECT_NO_THROW(g.from_json(j));
}

TEST(SettingsGroup, OwnershipTransferred) {
    SettingsGroup g;
    auto s = std::make_unique<FloatSetting>("bar", 1.0f);
    auto* raw = s.get();

    g.add_setting("x", std::move(s));

    EXPECT_EQ(s.get(), nullptr);
    EXPECT_EQ(g.get("x"), raw);
}

TEST(SettingsGroup, CloneIsDeep) {
    SettingsGroup g;
    g.add_setting("a", std::make_unique<FloatSetting>("bar", 1.0f));

    auto copy = g.clone();

    EXPECT_NE(copy.get(), &g);
    EXPECT_NE(
        copy->get_child("a"),
        g.get_child("a")
    );
}
