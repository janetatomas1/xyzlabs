
#include "xyzlabs/floatingwidget.hpp"
#include "xyzlabs/operators.hpp"

namespace xyzlabs {

void FloatingWidget::show(const ImVec2 &size, const ImVec2 &pos) {};

void FloatingWidget::show() {
    const auto parentSize = ImGui::GetContentRegionAvail();
    const auto size = parentSize * sizeRatio_;
    const auto position = parentSize * positionRatio_;

    if(ImGui::Begin(title().c_str(), nullptr, flags_)) { 
        ImGui::SetWindowSize(size);
        ImGui::SetWindowPos(position);
        show(size, position);
    }
    ImGui::End();
}

}
