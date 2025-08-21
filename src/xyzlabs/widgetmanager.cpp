
#include <utility>
#include <algorithm>

#include "xyzlabs/widgetmanager.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"

constexpr size_t MAIN_WINDOW_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
constexpr size_t TOOLBAR_WINDOW_FLAGS = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

void AppSettings::show_input_widget() {
    if(ImGui::BeginTable("##main_window_settings", 2, ImGuiTableFlags_Borders)) {
        ImGui::TableHeadersRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Main window");

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Font size");
        ImGui::TableNextColumn();
        ImGui::SliderFloat("##main_windows_float_size", &mainWindowFontScale, 0.0, 3.0f);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Background color");
        ImGui::TableNextColumn();
        ImGui::ColorEdit4("Color with Alpha", (float*)&mainWindowColor);

        ImGui::EndTable();
        ImGui::Dummy({0.0f, 10.0f});
    }

    if(ImGui::BeginTable("##toolbar_window_settings", 2, ImGuiTableFlags_Borders)) {
        ImGui::TableHeadersRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Toolbar window");

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Font size");
        ImGui::TableNextColumn();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Background color");

        ImGui::EndTable();
        ImGui::Dummy({0.0f, 10.0f});
    }
}

json AppSettings::serialize() const {
    return json{
        {"main_window_color", mainWindowColor},
        {"main_window_font", mainWindowFontScale},
        {"toolbar_window_color", toolbarWindowColor},
        {"toolbar_window_font", toolbarWindowFontScale},
    };
}

void AppSettings::deserialize(const json& obj) {
    obj.at("main_window_color").get_to(mainWindowColor);
    obj.at("main_window_font").get_to(mainWindowFontScale);
    obj.at("toolbar_window_color").get_to(toolbarWindowColor);
    obj.at("toolbar_window_font").get_to(toolbarWindowFontScale);
}

void WidgetManager::init() {
    XYZLabs::instance()
    .settings_manager()
    .register_settings<AppSettings>(constants::MAIN_APP_SETTINGS_LABEL);
}

void WidgetManager::show(const ImVec2 &size) {
    flush_new_widgets();
    remove_closed_widgets();
    show_toolbar(size);
    
    const ImVec2 pos = {0, 0};
    const auto ratio = toolbar_window_ratio();
    const ImVec2 mainWindowSize = {size.x * (1 - ratio), size.y};
    const ImVec2 mainWindowPos = {size.x * ratio, 0.0f};

    if(ImGui::Begin(constants::MAIN_WINDOW_ID.c_str(), NULL, MAIN_WINDOW_FLAGS)) {
        ImGui::SetWindowSize(mainWindowSize);
        ImGui::SetWindowPos(mainWindowPos);
        widgets_[currentWidget_]->show(size, pos);
        ImGui::End();
    }

    XYZLabs::instance().settings_manager().show_settings_widget(size);
}

void WidgetManager::show_toolbar(const ImVec2 &size) {    
    if(ImGui::Begin(constants::TOOLBAR_ID.c_str(), NULL, TOOLBAR_WINDOW_FLAGS)) {
        toolbarOpen_ = !ImGui::IsWindowCollapsed();

        ImVec2 toolBarSize = {size.x * toolbar_window_ratio(), size.y};
        ImVec2 btnSize = {size.x * 0.14f, size.y * 0.03f};

        ImGui::SetWindowSize(toolBarSize);
        ImGui::SetWindowPos({0, 0});

        if(ImGui::Button(constants::EXIT_BTN_TITLE.c_str(), btnSize)) {
            XYZLabs::instance().close();
        }

        if(settingsOpen_) {
            if(ImGui::Button(constants::CLOSE_SETTINGS_BTN_TITLE.c_str(), btnSize)) {
                settingsOpen_ = false;
            }            
        } else {
            if(ImGui::Button(constants::APP_SETTINGS_BTN_TITLE.c_str(), btnSize)) {
                settingsOpen_ = true;
            }
        }

        if(ImGui::Button(constants::CLOSE_CURRENT_SIMULATION_BTN_TITLE.c_str(), btnSize)) {
            close_current_widget();
        }

        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Separator();

        if(widgets_.size() > 1) {
            display_radio_buttons();
        }

        ImGui::Dummy(ImVec2(0, 30));

        if(widgets_.size() > 1) {
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 10));
        }

        widgets_[currentWidget_]->show_toolbar(toolBarSize, ImGui::GetCursorPos());
        ImGui::End();
    }
}

void WidgetManager::close_current_widget() {
    if(currentWidget_ != 0) {
        widgets_[currentWidget_]->close();
        widgets_[currentWidget_]->destroy();
        spdlog::info("Closed widget. Title: {}, id: {}", widgets_[currentWidget_]->title(), widgets_[currentWidget_]->id());
    }
}

void WidgetManager::show_settings_window(const ImVec2 &size) {
}
