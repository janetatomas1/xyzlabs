
#include "xyzlabs/relativelayout.hpp"

namespace xyzlabs {

void RelativeLayout::set_size_relative(const ImVec2 &size) {
    size_ = size;
}

void RelativeLayout::set_position_relative(const ImVec2 &position) {
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
