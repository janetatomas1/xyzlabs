
#include "xyzlabs/ui/button.hpp"

namespace xyzlabs {

Button::Button(const std::string& title): Widget(title) {}

bool Button::draw(const ImVec2& size, const ImVec2& pos) {
    const auto [btnSize, btnPos] = layout().compute(size, pos);
    ImGui::SetCursorPos(btnPos);
    return ImGui::Button(window_id().c_str(), btnSize);
}

bool Button::operator()(const ImVec2& size, const ImVec2& pos) {
    return draw(size, pos);
}

}
