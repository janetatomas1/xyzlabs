
#include <utility>

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

    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);
    ImGui::Begin(constants::MAIN_WINDOW_ID.c_str(), NULL, DEFAULT_FLAGS);

    for(auto &t: tabs_) {
        t->show(size, pos);
    }

    ImGui::End();

}

void WidgetManager::add_widget(std::unique_ptr<Widget> widget) {
    newWidgets_.push_back(std::move(widget));
}
