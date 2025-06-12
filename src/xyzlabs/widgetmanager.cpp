
#include <utility>
#include <algorithm>

#include "xyzlabs/widgetmanager.hpp"
#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"


void WidgetManager::show(const ImVec2 &size, const ImVec2 &pos) {
    flush_new_widgets();
    remove_closed_tabs();

    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(pos);

    tabs_[0]->show(size, pos);
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
