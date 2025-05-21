
#include <utility>
#include <algorithm>

#include "xyzlabs/widgetmanager.hpp"
#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"


void WidgetManager::show(ImVec2 &size, ImVec2 &pos) {
    flush_new_widgets();
    remove_closed_tabs();

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

bool WidgetManager::disable_widget_closing(IDType id) {
    for(auto &w: tabs_) {
        if(w->id() == id) {
            w->disable_closing();
            return true;
        }
    }

    return false;
}
