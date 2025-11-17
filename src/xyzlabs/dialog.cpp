
#include <format>

#include "xyzlabs/dialog.hpp"

namespace xyzlabs {

Dialog::Dialog(const std::string &text, const std::string title, const std::string &acceptBtnText):
    FloatingWidget(title), text_(text) {};

void Dialog::show(const ImVec2 &size, const ImVec2 &pos) {
    const ImVec2 childPos = size * 0.2f + pos;
    const ImVec2 childSize = size * ImVec2{0.6f, 0.2f};

    ImGui::SetCursorPos(childPos);
    if (ImGui::BeginChild(std::format("##{}", id()).c_str(), childSize, false)) {
        ImGui::Text("%s", text_.c_str());
    }
    ImGui::EndChild();

    const ImVec2 acceptBtnPos = size * ImVec2{0.7, 0.7} + pos;
    const ImVec2 acceptBtnSize = size * 0.1;
    ImGui::SetCursorPos(acceptBtnPos);
    if(ImGui::Button(acceptBtnText_.c_str(), acceptBtnSize)) {
        accept_callback();
    }
}

void Dialog::set_text(const std::string &text) {
    text_ = text;
}

void Dialog::set_accept_btn_text(const std::string &text) {
    acceptBtnText_ = text;
}

}
