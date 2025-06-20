
#include <utility>
#include <algorithm>

#include "xyzlabs/widgetmanager.hpp"
#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"

constexpr size_t MAIN_WINDOW_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
constexpr size_t TOOLBAR_WINDOW_FLAGS = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

void WidgetManager::show(const ImVec2 &size) {
    flush_new_widgets();
    remove_closed_widgets();
    show_toolbar(size);
    
    const ImVec2 pos = {0, 0};
    const auto ratio = toolbar_window_ratio();
    const ImVec2 mainWindowSize = {size.x * (1 - ratio), size.y};
    const ImVec2 mainWindowPos = {size.x * ratio, 0.0f};

    ImGui::Begin(constants::MAIN_WINDOW_ID.c_str(), NULL, MAIN_WINDOW_FLAGS);

    ImGui::SetWindowSize(mainWindowSize);
    ImGui::SetWindowPos(mainWindowPos);

    if(simulationRunning_) {
        widgets_[currentWidget_]->show(size, pos);
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

void WidgetManager::show_toolbar(const ImVec2 &size) {    
    ImGui::Begin(constants::TOOLBAR_ID.c_str(), NULL, TOOLBAR_WINDOW_FLAGS);

    toolbarOpen_ = !ImGui::IsWindowCollapsed();

    ImVec2 toolBarSize = {size.x * toolbar_window_ratio(), size.y};
    ImVec2 btnSize = {size.x * 0.14f, size.y * 0.03f};

    ImGui::SetWindowSize(toolBarSize);
    ImGui::SetWindowPos({0, 0});

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
