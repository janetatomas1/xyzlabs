#include <xyzlabs/ui/relativelayout.hpp>
#include <gtest/gtest.h>

using namespace xyzlabs;

TEST(RelativeLayoutTest, TestRelativeSize) {
    RelativeLayout layout = {
        {0.5f, 0.3f},
        {0.2f, 0.4f}
    };
    EXPECT_EQ(layout.size_relative().x, 0.5f);
    EXPECT_EQ(layout.size_relative().y, 0.3f);
}

TEST(RelativeLayoutTest, TestRelativePosition) {
    RelativeLayout layout = {
        {0.5f, 0.3f},
        {0.2f, 0.4f}
    };
    EXPECT_EQ(layout.position_relative().x, 0.2f);
    EXPECT_EQ(layout.position_relative().y, 0.4f);
}

TEST(RelativeLayoutTest, TestComputeSize) {
    RelativeLayout layout = {
        {0.8f, 0.3f},
        {0.2f, 0.1f}
    };
    auto sizeAbsolute = ImVec2{1000, 8000};
    auto sizeRelative = layout.compute_size(sizeAbsolute);
    EXPECT_NEAR(sizeRelative.x, (sizeAbsolute * layout.size_relative()).x, 0.001);
    EXPECT_NEAR(sizeRelative.y, (sizeAbsolute * layout.size_relative()).y, 0.001);
}

TEST(RelativeLayoutTest, TestComputePosition) {
    RelativeLayout layout = {
        {0.5f, 0.3f},
        {0.4f, 0.4f}
    };
    auto positionAbsolute = ImVec2{200, 300};
    auto sizeAbsolute = ImVec2{1500, 900};
    auto positionRelative = layout.compute_position(sizeAbsolute, positionAbsolute);
    EXPECT_NEAR(positionRelative.x, (positionAbsolute + sizeAbsolute * layout.position_relative()).x, 0.001);
    EXPECT_NEAR(positionRelative.y, (positionAbsolute + sizeAbsolute * layout.position_relative()).y, 0.001);
}

TEST(RelativeLayoutTest, TestCompute) {
    RelativeLayout layout = {
        {0.5f, 0.3f},
        {0.2f, 0.4f}
    };
    auto positionAbsolute = ImVec2{1000, 8000};
    auto sizeAbsolute = ImVec2{1000, 8000};
    auto [sizeRelative, positionRelative] = layout.compute(sizeAbsolute, positionAbsolute);
    EXPECT_NEAR(positionRelative.x, (positionAbsolute + sizeAbsolute * layout.position_relative()).x, 0.001);
    EXPECT_NEAR(positionRelative.y, (positionAbsolute + sizeAbsolute * layout.position_relative()).y, 0.001);
    EXPECT_NEAR(sizeRelative.x, (sizeAbsolute * layout.size_relative()).x, 0.001);
    EXPECT_NEAR(sizeRelative.y, (sizeAbsolute * layout.size_relative()).y, 0.001);
}
