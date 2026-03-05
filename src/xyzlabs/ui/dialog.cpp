
#include <cstdlib>
#include <fmt/format.h>

#include "xyzlabs/ui/dialog.hpp"
#include "xyzlabs/event/eventmanager.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/utils/operators.hpp"

namespace xyzlabs {

Dialog::Dialog(
    const std::string &content,
    const std::string &acceptText,
    const std::string &rejectText,
    const std::string &title,
    Widget *parent,
    Window *window):
    Widget(title, parent, window),
    content_(content),
    rejectButton_(rejectText),
    acceptButton_(acceptText) {
    layout().set_size_relative({0.4f, 0.3f});
    layout().set_position_relative({0.3f, 0.1f});
}

void Dialog::show(const ImVec2 &size, const ImVec2 &position) {
    auto [outerSize, outerPos] = layout().compute(size, position);
    auto [contentSize, contentPos] = contentLayout_.compute(outerSize, {0.0f, 0.0f});

    auto drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(outerPos, outerPos + outerSize, ImGui::GetColorU32(ImGuiCol_WindowBg));

    ImGui::SetCursorPos(outerPos);
    ImGui::BeginChild(fmt::format("##outer{}", id()).c_str(), outerSize, true);

    ImGui::SetCursorPos(contentPos);
    ImGui::TextWrapped("%s", content_.c_str());

    if(rejectActive_ && rejectButton_(outerSize, {0.0f, 0.0f})) {
        reject();
    }

    if(acceptButton_(outerSize, {0.0f, 0.0f})) {
        accept();
    }
    ImGui::EndChild();
}

void Dialog::accept() {
    acceptAction_();
}

void Dialog::reject() {
    rejectAction_();
}

void Dialog::set_content(const std::string &text) {
    content_ = text;
}

std::string Dialog::content() {
    return content_;
}

void Dialog::set_accept_action(action act) {
    acceptAction_ = std::move(act);
}

void Dialog::set_reject_action(action act) {
    rejectAction_ = std::move(act);
}

ImVec4& Dialog::background_color() {
    return color_;
}

void Dialog::set_reject_active(bool value) {
    rejectActive_ = value;
}

RelativeLayout &Dialog::content_layout() {
    return contentLayout_;
}

Button& Dialog::accept_button() {
    return acceptButton_;
}

Button& Dialog::reject_button() {
    return rejectButton_;
}

}
