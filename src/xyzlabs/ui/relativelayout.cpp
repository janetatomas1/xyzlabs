
#include "xyzlabs/ui/relativelayout.hpp"
#include "xyzlabs/assert.hpp"

namespace xyzlabs {

RelativeLayout::RelativeLayout(
    const ImVec2 &size,
    const ImVec2 &position
): size_(size), position_(position) {
    XYZ_ASSERT_MSG(
        size.x <= 1.0f
        && size.y <= 1.0f
        && size.x >= 0.0f
        && size.y >= 0.0f,
        "Size must be between 0 and 1"
    );
    XYZ_ASSERT_MSG(
        position.x <= 1.0f
        && position.y <= 1.0f
        && position.x >= 0.0f
        && position.y >= 0.0f,
        "Position must be between 0 and 1"
    );
}

void RelativeLayout::set_size_relative(const ImVec2 &size) {
    XYZ_ASSERT_MSG(
        size.x <= 1.0f
        && size.y <= 1.0f
        && size.x >= 0.0f
        && size.y >= 0.0f,
        "Size must be between 0 and 1"
    );
    size_ = size;
}

void RelativeLayout::set_position_relative(const ImVec2 &position) {
    XYZ_ASSERT_MSG(
        position.x <= 1.0f
        && position.y <= 1.0f
        && position.x >= 0.0f
        && position.y >= 0.0f,
        "Position must be between 0 and 1"
    );
    position_ = position;
}

ImVec2 RelativeLayout::size_relative() {
    return size_;
}

ImVec2 RelativeLayout::position_relative() {
    return position_;
}

ImVec2 RelativeLayout::compute_size(const ImVec2 &size) {
    return size * size_;
}


ImVec2 RelativeLayout::compute_position(const ImVec2 &size, const ImVec2 &position) {
    return position + position_ * size;
}

std::pair<ImVec2, ImVec2> RelativeLayout::compute(const ImVec2 &size, const ImVec2 &position) {
    return {
        {size * size_},
        {position + position_ * size}
    };
}

}
