
#include "xyzlabs/ui/styledbutton.hpp"

namespace xyzlabs {

StyledButton::StyledButton(const std::string& title) : Widget(title) {}

bool StyledButton::draw(const ImVec2& size, const ImVec2& pos) {
    const auto [btnSize, btnPos] = layout().compute(size, pos);
    ImGui::PushStyleColor(ImGuiCol_Button, btnColor_);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btnHoverColor_);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, btnActiveColor_);
    ImGui::PushStyleColor(ImGuiCol_Text, btnTextColor_);

    ImGui::SetCursorPos(btnPos);
    bool result = ImGui::Button(
        window_id().c_str(),
        btnSize
    );

    ImGui::PopStyleColor(4);
    return result;
}

bool StyledButton::operator()(const ImVec2& size, const ImVec2& pos) {
    return draw(size, pos);
}

ImVec4& StyledButton::color() {
    return btnColor_;
}

ImVec4& StyledButton::hover_color() {
    return btnHoverColor_;
}

ImVec4& StyledButton::active_color() {
    return btnActiveColor_;
}

ImVec4& StyledButton::text_color() {
    return btnTextColor_;
}

}
