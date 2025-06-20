
#include <utility>
#include <algorithm>

#include "xyzlabs/widgetmanager.hpp"
#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"


void WidgetManager::show(const ImVec2 &size, const ImVec2 &pos) {
    flush_new_widgets();
    remove_closed_widgets();

    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(pos);
    
    ImVec2 toolBarSize = {size.x * 0.2f, size.y * 0.5f};
    ImVec2 btnSize = {size.x * 0.14f, size.y * 0.03f};

    if(simulationRunning_) {
        // widgets_[currentWidget_]->show(size, pos);
    }

    if (ImGui::BeginMenu("File")) {
        ImGui::MenuItem("Open");
        ImGui::MenuItem("Save");
        ImGui::EndMenu();
    }
    ImGui::SetNextWindowSize(toolBarSize);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin(constants::TOOLBAR_ID.c_str(), NULL, ImGuiWindowFlags_NoFocusOnAppearing);

    if(ImGui::Button(constants::EXIT_BTN_TITLE.c_str(), btnSize)) {
        XYZLabs::instance().close();
    }
    
    ImGui::Button(constants::APP_SETTINGS_BTN_TITLE.c_str(), btnSize);
    ImGui::Button(constants::SIMULATION_SETTINGS_BTN_TITLE.c_str(), btnSize);
    ImGui::Button(constants::CLOSE_CURRENT_SIMULATION_BTN_TITLE.c_str(), btnSize);

    if(simulationRunning_) {
        if(ImGui::Button(constants::STOP_SIMULATION_BTN_TITLE.c_str(), btnSize)) {
            simulationRunning_ = false;
        }
    } else {
        if(ImGui::Button(constants::START_SIMULATION_BTN_TITLE.c_str(), btnSize)) {
            simulationRunning_ = true;
        }
    }

    if(widgets_.size() > 1) {
        display_radio_buttons();
    }
    ImGui::End();
}

bool WidgetManager::disable_widget_closing(IDType id) {
    for(auto &w: widgets_) {
        if(w->id() == id) {
            w->disable_closing();
            return true;
        }
    }

    return false;
}
