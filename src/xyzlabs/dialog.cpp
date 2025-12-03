
#include "xyzlabs/dialog.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/globals.hpp"

#include <spdlog/spdlog.h>

namespace xyzlabs { 

Dialog::Dialog(
    const std::string &content,
    const std::string &acceptText,
    const std::string &rejectText,
    const std::string &title): 
    Widget(title),
    content_(content),
    acceptText_(acceptText),
    rejectText_(rejectText) {
    layout().set_size_relative({0.35f, 0.3f});
    layout().set_position_relative({0.35f, 0.3f});
}

void Dialog::display(const ImVec2 &size, const ImVec2 &position) {
    ImGui::SetNextWindowFocus();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, color_);
    Widget::display(size, position);
    ImGui::PopStyleColor();
}

void Dialog::show(const ImVec2 &size, const ImVec2 &position) {
    auto [contentSize, contentPos] = contentLayout_.compute(size, {0.0f, 0.0f});
    auto [rejectBtnSize, rejectBtnPos] = rejectBtnLayout_.compute(size, {0.0f, 0.0f});
    auto [acceptBtnSize, acceptBtnPos] = acceptBtnLayout_.compute(size, {0.0f, 0.0f});

    ImGui::SetCursorPos(contentPos);
    ImGui::BeginChild("TextBox", contentSize, true, ImGuiWindowFlags_NoScrollbar);
    ImGui::TextWrapped("%s", content_.c_str());
    ImGui::EndChild();

    ImGui::SetCursorPos(rejectBtnPos);
    if(ImGui::Button(rejectText_.c_str(), rejectBtnSize)) {
        reject();
    }

    ImGui::SetCursorPos(acceptBtnPos);
    if(ImGui::Button(acceptText_.c_str(), acceptBtnSize)) {
        accept();
    }
}

void Dialog::accept() {
    acceptAction_();
}

void Dialog::reject() {
    rejectAction_();
}

void Dialog::set_content(const std::string &text) {
    event_manager().add_action([this, &text]() mutable {
        content_ = text;
    });
}

void Dialog::set_accept_text(const std::string &text) {
    event_manager().add_action([this, &text]() mutable {
        acceptText_ = text;
    });
}

void Dialog::set_reject_text(const std::string &text) {
    event_manager().add_action([this, &text]() mutable {
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
    event_manager().add_action([this, act = std::move(act)]() mutable {
        acceptAction_ = std::move(act);
    });
}

void Dialog::set_reject_action(action act) {
    event_manager().add_action([this, act = std::move(act)]() mutable {
        rejectAction_ = std::move(act);
    });
}

ImVec4 Dialog::background_color() {
    return color_;
}

void Dialog::set_background_color(const ImVec4 &color) {
    event_manager().add_action([this, color]() mutable {
        color_ = color;
    });
}

void Dialog::set_reject_active(bool value) {
    event_manager().add_action([this, value]() mutable {
        rejectActive_ = value;
    });
}

}
