
#pragma once

#include <imgui.h>

#include "xyzlabs/widget.hpp"

namespace xyzlabs {

class FloatingWidget: public Widget {
    ImVec2 positionRatio_ = {0.4f, 0.4f};
    ImVec2 sizeRatio_ = {0.3f, 0.3f};
    int flags_;

public:
    FloatingWidget(
        const std::string title = "",
        int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove,
        const ImVec2 &sizeRatio = {0.4f, 0.4f},
        const ImVec2 &positionRatio = {0.3f, 0.3f}
    ): Widget(title), flags_(flags), sizeRatio_(sizeRatio), positionRatio_(positionRatio) {};
    virtual ~FloatingWidget() = default;
    virtual void show(const ImVec2 &size, const ImVec2 &pos);
    virtual void show();
    void set_size(const ImVec2 &size);
    void set_position(const ImVec2 &pos);
    void set_flags(int flags);
};

};
