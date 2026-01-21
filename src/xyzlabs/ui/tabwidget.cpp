
#include <fmt/format.h>
#include <string>

#include "xyzlabs/ui/tabwidget.hpp"
#include "xyzlabs/event/eventmanager.hpp"
#include "xyzlabs/utils/randomgenerator.hpp"
#include "xyzlabs/window.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace xyzlabs {

void TabWidget::recompute_tab_ids() {
    tabIds_.clear();
    for(size_t i = 0; i < tabs_.size(); ++i) {
        tabIds_.push_back(fmt::format("{}##{}", tabs_[i]->title(), i));
    }
}

TabWidget::TabWidget(
    const std::string &title,
    Widget *parent,
    Window *window
): Widget(title, parent, window) {
    tabBarId_ = fmt::format("##{}", XYZLabs::random_generator()());
}

Widget* TabWidget::add_tab_internal(std::unique_ptr<Widget> tab, size_t position) {
    Widget *ptr = tab.get();
    app()->event_manager().add_action([this, position, widget = std::move(tab)]() mutable {
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
        recompute_tab_ids();
    });
    return ptr;
}

Widget* TabWidget::set_tab_internal(std::unique_ptr<Widget> tab, size_t position) {
    if(tabs_.size() > position) {
        Widget *ptr = tab.get();
        app()->event_manager().add_action([this, position, tab = std::move(tab)]() mutable {
            window()->make_context_current();
            tab->init();
            tab->set_parent(this);
            tab->set_window(window());
            tabs_[position]->destroy();
            tabs_[position] = std::move(tab);
            recompute_tab_ids();
        });
        return ptr;
    }

    return nullptr;
}

void TabWidget::show(const ImVec2 &size, const ImVec2 &position) {
    auto [tabBarSize, tabBarPosition] = tabBarLayout_.compute(size, position);
    auto resizeWidget = tabBarOpen_ && !renderTabBarOverWidget_;

    ImVec2 mainWidgetSize = resizeWidget ? ImVec2{size.x - tabBarSize.x, size.y} : size;
    ImVec2 mainWidgetPosition = resizeWidget ? ImVec2{position.x + tabBarSize.x, position.y} : position;

    ImGui::SetCursorPos(mainWidgetPosition);

    tabs_[currentTab_]->show(mainWidgetSize, mainWidgetPosition);

    if(tabBarOpen_) {
        ImGui::SetCursorPos(tabBarPosition);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.05f));
        ImGui::BeginChild(tabBarId_.c_str(), tabBarSize, true);

        for(size_t i=0;i < tabs_.size();i++) {
            if(currentTab_ == i) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
            }
            if(ImGui::Button(
                fmt::format("{}##{}", tabs_[i]->title(),i).c_str(),
                ImVec2(tabBarSize.x * 0.9f, 30)
            )) {
                app()->event_manager().add_action([this, i](){
                    currentTab_ = i;
                });
            }

            if(currentTab_ == i) {
                ImGui::PopStyleColor(1);
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleColor(1);
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
    app()->event_manager().add_action([this, idx] () {
        tabs_[idx]->destroy();
        tabs_.erase(tabs_.begin() + idx);
        if(currentTab_ != 0) {
            currentTab_ = currentTab_ - 1;
        }
        recompute_tab_ids();
    });
}

void TabWidget::remove_tab_id(uint64_t id) {
    app()->event_manager().add_action([this, id]() {
        auto it = std::find_if(tabs_.begin(), tabs_.end(),
            [id](const auto& w) {
                return w->id() == id;
            }
        );
        size_t index = std::distance(tabs_.begin(), it);
        remove_tab(index);
    });
}

bool TabWidget::is_tabbar_open() const {
    return tabBarOpen_;
}

void TabWidget::set_tabbar_open(bool open) {
    tabBarOpen_ = open;
}

bool TabWidget::is_render_over_widget() const {
    return renderTabBarOverWidget_;
}

void TabWidget::set_render_over_widget(bool render) {
    renderTabBarOverWidget_ = render;
}

}
