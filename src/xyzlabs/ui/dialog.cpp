
#include <cstdlib>
#include <fmt/format.h>

#include "xyzlabs/ui/dialog.hpp"
#include "xyzlabs/event/eventmanager.hpp"
#include "xyzlabs/xyzlabs.hpp"

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
    acceptText_(acceptText),
    rejectText_(rejectText) {
    layout().set_size_relative({0.4f, 0.3f});
    layout().set_position_relative({0.3f, 0.1f});
}

void Dialog::show(const ImVec2 &size, const ImVec2 &position) {
    auto [outerSize, outerPos] = layout().compute(size, position);

    auto [contentSize, contentPos] = contentLayout_.compute(outerSize, {0.0f, 0.0f});
    auto [rejectBtnSize, rejectBtnPos] = rejectBtnLayout_.compute(outerSize, {0.0f, 0.0f});

    auto &acceptLayout = rejectActive_ ? acceptBtnLayout_ : acceptBtnAloneLayout_;
    auto [acceptBtnSize, acceptBtnPos] = acceptLayout.compute(outerSize, {0.0f, 0.0f});

    auto drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(outerPos, outerPos + outerSize, ImGui::GetColorU32(ImGuiCol_WindowBg));

    ImGui::SetCursorPos(outerPos);
    ImGui::BeginChild(fmt::format("##outer{}", id()).c_str(), outerSize, true);

    ImGui::SetCursorPos(contentPos);
    ImGui::TextWrapped("%s", content_.c_str());

    ImGui::SetCursorPos(rejectBtnPos);
    if(rejectActive_ && ImGui::Button(rejectText_.c_str(), rejectBtnSize)) {
        reject();
    }

    ImGui::SetCursorPos(acceptBtnPos);
    if(ImGui::Button(acceptText_.c_str(), acceptBtnSize)) {
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
    app()->event_manager().add_action([this, &text]() mutable {
        content_ = text;
    });
}

void Dialog::set_accept_text(const std::string &text) {
    app()->event_manager().add_action([this, &text]() mutable {
        acceptText_ = text;
    });
}

void Dialog::set_reject_text(const std::string &text) {
    app()->event_manager().add_action([this, &text]() mutable {
        rejectText_ = text;
    });
}

std::string Dialog::content() {
    return content_;
}

std::string Dialog::accept_text() {
    return acceptText_;
}

std::string Dialog::reject_text() {
    return rejectText_;
}

void Dialog::set_accept_action(action act) {
    app()->event_manager().add_action([this, act = std::move(act)]() mutable {
        acceptAction_ = std::move(act);
    });
}

void Dialog::set_reject_action(action act) {
    app()->event_manager().add_action([this, act = std::move(act)]() mutable {
        rejectAction_ = std::move(act);
    });
}

ImVec4 Dialog::background_color() {
    return color_;
}

void Dialog::set_background_color(const ImVec4 &color) {
    app()->event_manager().add_action([this, color]() mutable {
        color_ = color;
    });
}

void Dialog::set_reject_active(bool value) {
    app()->event_manager().add_action([this, value]() mutable {
        rejectActive_ = value;
    });
}

RelativeLayout &Dialog::accept_btn_layout() {
    return acceptBtnLayout_;
}

RelativeLayout &Dialog::reject_btn_layout() {
    return rejectBtnLayout_;
}

RelativeLayout &Dialog::accept_btn_alone_layout() {
    return acceptBtnAloneLayout_;
}

RelativeLayout &Dialog::content_layout() {
    return contentLayout_;
}

}
