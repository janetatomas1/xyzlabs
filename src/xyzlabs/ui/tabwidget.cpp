
#include <fmt/format.h>
#include <string>

#include "xyzlabs/ui/tabwidget.hpp"
#include "xyzlabs/event/eventmanager.hpp"
#include "xyzlabs/utils/randomgenerator.hpp"
#include "xyzlabs/window.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/utils/operators.hpp"

namespace xyzlabs {

void TabWidget::recompute_tab_ids() {
    tabIds_.clear();
    checkedIds_.clear();
    for(size_t i = 0; i < tabs_.size(); ++i) {
        tabIds_.push_back(fmt::format("{}##{}", tabs_[i]->title(), i));
        checkedIds_.push_back(fmt::format("##checkbox{}", i));
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
            checked_.insert(checked_.begin() + position, false);
        } else {
            tabs_.push_back(std::move(widget));
            currentTab_ = tabs_.size() - 1;
            checked_.push_back(false);
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
    for(size_t i = 0; i < tabs_.size(); i++) {
        if(tabs_[i] == nullptr) {
            spdlog::info("TabWidget index, size: {} {}", i, tabs_.size());
        }
    }

    auto [tabBarSize, tabBarPosition] = tabBarLayout_.compute(size, position);
    auto [scrollBarSize, scrollbarPosition] = scrollBarLayout_.compute(tabBarSize, tabBarPosition);

    ImGui::SetCursorPos(position);
    tabs_[currentTab_]->show(size, position);

    auto mousePos = ImGui::GetMousePos();
    if(mousePos.x <= position.x + tabBarSize.x
        && mousePos.y <= position.y + tabBarSize.y
        && mousePos.x >= position.x
        && mousePos.y >= position.y
    ) {
        ImGui::SetCursorPos(tabBarPosition);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.05f));
        ImGui::BeginChild(tabBarId_.c_str(), tabBarSize, ImGuiWindowFlags_NoScrollbar, true);
        ImGui::BeginChild(scrollbarId_.c_str(), scrollBarSize,  true);

        const auto btnSize = tabButtonSize_ * tabBarSize;

        for(size_t i=0;i < tabs_.size();i++) {
            if(currentTab_ == i) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
            }

            if(ImGui::Button(
                fmt::format("{}##{}", tabs_[i]->title(),i).c_str(),
                btnSize
            )) {
                app()->event_manager().add_action([this, i](){
                    currentTab_ = i;
                });
            }

            ImGui::SameLine();
            auto cursor = ImGui::GetCursorPos();
            cursor.y += btnSize.y * 0.3;
            ImGui::SetCursorPos(cursor);
            ImGui::Checkbox(checkedIds_[i].c_str(), (bool*)&checked_[i]);

            if(currentTab_ == i) {
                ImGui::PopStyleColor(1);
            }
        }
        ImGui::EndChild();

        const auto [closeBtnSize, closeBtnPos] = closeBtnLayout_.compute(tabBarSize, tabBarPosition);
        ImGui::SetCursorPos(closeBtnPos);
        if(ImGui::Button("Close checked tabs", closeBtnSize)) {
            app()->event_manager().add_action([this]() {
                std::vector<uint64_t> ids;
                for (size_t i=0;i < tabs_.size();i++){
                    if(checked_[i]) {
                        ids.push_back(tabs_[i]->id());
                    }
                }
                std::erase_if(checked_, [](const bool &x) {
                    return x;
                });
                std::erase_if(tabs_, [&ids](const std::unique_ptr<Widget>& w) {
                    return std::find(ids.begin(), ids.end(), w->id()) != ids.end();
                });

                recompute_tab_ids();
                currentTab_ = std::min(currentTab_, tabs_.size() - 1);
            });
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
        checked_.erase(checked_.begin() + idx);
    });
}

void TabWidget::remove_tab_id(uint64_t id) {
    auto it = std::find_if(tabs_.begin(), tabs_.end(),
        [id](const auto& w) {
            return w->id() == id;
        }
    );
    size_t index = std::distance(tabs_.begin(), it);
    remove_tab(index);
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
