
#pragma once

#include <imgui.h>
#include <utility>

#include "xyzlabs/operators.hpp"

namespace xyzlabs {

class RelativeLayout {
    ImVec2 size_ = {1.0f, 1.0f};
    ImVec2 position_ = {0.0f, 0.0f};

public:
    RelativeLayout() = default;
    RelativeLayout(const ImVec2 &size, const ImVec2 &position);
    void set_size_relative(const ImVec2 &size);
    void set_position_relative(const ImVec2 &position);
    ImVec2 size_relative();
    ImVec2 position_relative();
    ImVec2 compute_size(const ImVec2 &size);
    ImVec2 compute_position(
        const ImVec2 &size,
        const ImVec2 &position
    );
    std::pair<ImVec2, ImVec2> compute(const ImVec2 &size, const ImVec2 &position);
};

};
