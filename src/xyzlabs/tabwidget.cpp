
#include <fmt/format.h>
#include <string>

#include "xyzlabs/tabwidget.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/window.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

TabWidget::TabWidget(
    const std::string &title,
    Widget *parent,
    Window *window
): Widget(title, parent, window) {
    tabBarID_ = fmt::format("{}##{}", title, XYZLabs::random_generator().random());
}

Widget* TabWidget::add_tab_internal(std::unique_ptr<Widget> tab, size_t position) {
    Widget *ptr = tab.get();
    event_manager().add_action([this, position, widget = std::move(tab)]() mutable {
        auto size = btnLayout_.size_relative();
        widget->set_window(window());
        widget->set_parent(this);
        window()->make_context_current();
        widget->init();
        if(position < tabs_.size()) {
            tabs_.insert(tabs_.begin() + position, std::move(widget));
            currentTab_ = position;
        } else {
            tabs_.push_back(std::move(widget));
            currentTab_ = tabs_.size() - 1;
        }
        if(tabs_.size() > 1) {
            size.x = size.x * (tabs_.size() - 1);
        }
        size.x = size.x / tabs_.size();
        btnLayout_.set_size_relative(size);
        recompute_tab_width();
    });
    return ptr;
}

Widget* TabWidget::set_tab_internal(std::unique_ptr<Widget> tab, size_t position) {
    if(tabs_.size() > position) {
        Widget *ptr = tab.get();
        event_manager().add_action([this, position, tab = std::move(tab)]() mutable {
            window()->make_context_current();
            tab->init();
            tab->set_parent(this);
            tab->set_window(window());
            tabs_[position]->destroy();
            tabs_[position] = std::move(tab);
        });
        return ptr;
    }

    return nullptr;
}

void TabWidget::render_full_tabbar() {
    for(size_t i=0;i < tabs_.size();i++) {
        auto &tab = tabs_[i];

        ImGui::SetCursorPos(btnPosition);
        if(ImGui::Button(tab->window_id().c_str(), btnSize)) {
            auto mousePos = ImGui::GetMousePos();
            if(mousePos.x > closeBtnPosition.x) {
                remove_tab(currentTab_);
                if(currentTab_ == tabs_.size() - 1) {
                    currentTab_ -= 1;
                }
            } else {
                currentTab_ = i;
            }
        }

        ImGui::SetCursorPos(closeBtnPosition);
        if(ImGui::Button(fmt::format("x##{}", i).c_str(), closeBtnSize)) {
        }
        btnPosition.x += btnSize.x;
        closeBtnPosition.x += btnSize.x;
    }
}

void TabWidget::render_only_current() {
    for(size_t i=0;i < tabs_.size();i++) {
        auto &tab = tabs_[i];

        ImGui::SetCursorPos(btnPosition);
        if(ImGui::Button(tab->window_id().c_str(), btnSize)) {
            auto mousePos = ImGui::GetMousePos();
            if(currentTab_ == i && mousePos.x > closeBtnPosition.x) {
                remove_tab(currentTab_);
                if(currentTab_ == tabs_.size() - 1) {
                    currentTab_ -= 1;
                }
            } else {
                currentTab_ = i;
            }
        }

        if(currentTab_ == i) {
            ImGui::SetCursorPos(closeBtnPosition);
            ImGui::Button(fmt::format("x##{}", i).c_str(), closeBtnSize);
        }
        btnPosition.x += btnSize.x;
        closeBtnPosition.x += btnSize.x;
    }
}

void TabWidget::show(const ImVec2 &size, const ImVec2 &position) {
    btnSize = btnLayout_.compute_size(size);
    btnPosition = btnLayout_.compute_position(size, position);
    closeBtnSize = {30.0f, btnSize.y};
    closeBtnPosition = {btnSize.x - 30, 0.0f};

    const bool displayAllCloseBtns = btnSize.x > closeBtnSize.x * 10.0f;

    if(displayAllCloseBtns) {
        render_full_tabbar();
    } else {
        render_only_current();
    }

    if(currentTab_ < tabs_.size()) {
        auto [tabSize, tabPosition] = tabLayout_.compute(size, position);
        auto &currentTab = tabs_[currentTab_];
        currentTab->show(tabSize, tabPosition);
    }
}

Widget* TabWidget::get_tab(size_t idx) {
    return tabs_[idx].get();
}

Widget* TabWidget::get_tab_id(uint64_t id) {
    Widget *ptr = nullptr;
    for(auto &tab: tabs_) {
        if(tab->id() == id) {
            ptr = tab.get();
        }
    }

    return ptr;
}

Widget* TabWidget::get_current_tab() {
    return tabs_[currentTab_].get();
}

size_t TabWidget::get_current_index() {
    return currentTab_;
}

size_t TabWidget::count() {
    return tabs_.size();
}

void TabWidget::remove_tab(size_t idx) {
    event_manager().add_action([this, idx] () {
        tabs_[idx]->destroy();
        tabs_.erase(tabs_.begin() + idx);
        recompute_tab_width();
        if(currentTab_ != 0) {
            currentTab_ = currentTab_ - 1;
        }
    });
}

void TabWidget::remove_tab_id(uint64_t id) {
    event_manager().add_action([this, id]() {
        auto it = std::find_if(tabs_.begin(), tabs_.end(),
            [id](const auto& w) {
                return w->id() == id;
            }
        );
        size_t index = std::distance(tabs_.begin(), it);
        remove_tab(index);
    });
}

void TabWidget::recompute_tab_width() {
    event_manager().add_action([this]() mutable {
        auto size = btnLayout_.size_relative();
        if(tabs_.empty()) {
            size.x = 1;
        } else {
            size.x = 1.0f / tabs_.size();
        }
        btnLayout_.set_size_relative(size);
    });
}

}
