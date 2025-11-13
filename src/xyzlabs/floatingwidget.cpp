
#include "xyzlabs/floatingwidget.hpp"
#include "xyzlabs/operators.hpp"
#include <spdlog/spdlog.h>

namespace xyzlabs {

void FloatingWidget::show(const ImVec2 &size, const ImVec2 &pos) {};

void FloatingWidget::show() {
    const auto parentSize = ImGui::GetContentRegionAvail();
    const auto size = parentSize * sizeRatio_;
    const auto position = parentSize * positionRatio_;

    auto ctx = ImGui::GetCurrentContext();
    if(ImGui::Begin(title().c_str())) { 
        ImGui::SetWindowSize(size);
        ImGui::SetWindowPos(position);
        show(size, position);
    }
    ImGui::End();
}

void FloatingWidget::set_size(const ImVec2 &size) {
    sizeRatio_ = size;
}

void FloatingWidget::set_position(const ImVec2 &pos) {
    positionRatio_ = pos;
}

void FloatingWidget::set_flags(int flags) {
    flags_ = flags;
}

void Dialog::show(const ImVec2 &size, const ImVec2 &pos) {
    ImGui::Button("abcdddd");
}

}
