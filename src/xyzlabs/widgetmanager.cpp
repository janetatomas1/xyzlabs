
#include <utility>
#include <algorithm>

#include "widgetmanager.hpp"
#include "introwidget.hpp"
#include "constants.hpp"


void WidgetManager::show(ImVec2 &size, ImVec2 &pos) {
    if(!newWidgets_.empty()) {
        for(auto &w: newWidgets_) {
            tabs_.push_back(std::move(w));
        }
        newWidgets_.clear();

    }

    tabs_.erase(
        std::remove_if(tabs_.begin(), tabs_.end(),
            [](auto &t) {
                return !t->is_open();
            }
        ), tabs_.end()
    );

    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);
    ImGui::Begin(constants::MAIN_WINDOW_ID.c_str(), NULL, DEFAULT_FLAGS);

    float tabWidth = size.x / tabs_.size();

    if(ImGui::BeginTabBar(constants::TAB_BAR_ID.c_str())) {
        ImGui::SetNextItemWidth(size.x / tabs_.size());
        for(auto &t: tabs_) {
            ImGui::SetNextItemWidth(tabWidth);
            if(ImGui::BeginTabItem(t->title_id().c_str(), t->is_open_ptr(), t->tab_item_flags())) {
                t->show(size, pos);
                ImGui::EndTabItem();
                t->deselect();
            }
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndTabBar();    
    }
    ImGui::End();

}

void WidgetManager::add_widget(std::unique_ptr<Widget> widget) {
    newWidgets_.push_back(std::move(widget));
}
