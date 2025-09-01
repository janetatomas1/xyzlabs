
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
        {"main_window_font", mainWindowFontScale}
    };
}

void AppSettings::deserialize(const json& obj) {
    obj.at("main_window_color").get_to(mainWindowColor);
    obj.at("main_window_font").get_to(mainWindowFontScale);
}

void WidgetManager::init() {
    XYZLabs::instance()
    .settings_manager()
    .register_settings<AppSettings>(constants::MAIN_APP_SETTINGS_LABEL);

    auto settingsReloadCallback = [this](std::unique_ptr<Event> event) {
        spdlog::info("abcd");
        reload_settings();
    };

    auto callbackPtr = std::make_unique<std::function<void(std::unique_ptr<Event>)>>(
        std::move(settingsReloadCallback)
    );

    XYZLabs::instance().event_manager().subscribe(constants::MAIN_APP_SETTINGS_LABEL, std::move(callbackPtr));
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
        ImGui::SetWindowFontScale(settings_.mainWindowFontScale);
        ImGui::SetWindowSize(mainWindowSize);
        ImGui::SetWindowPos(mainWindowPos);
        widgets_[currentWidget_]->show(size, pos);
        ImGui::End();
    }

    XYZLabs::instance().settings_manager().show_settings_window(size);
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

        bool settingsOpen = XYZLabs::instance()
        .settings_manager()
        .settings_open();
        
        if(settingsOpen) {
            ImGui::BeginDisabled();
        }

        if(ImGui::Button(constants::APP_SETTINGS_BTN_TITLE.c_str(), btnSize)) {
            XYZLabs::instance().settings_manager().set_settings_open(true);
        }

        if(settingsOpen) {
            ImGui::EndDisabled();
        }

        if(widgets_.size() == 1) {
            ImGui::BeginDisabled();
        }
        
        if(ImGui::Button(constants::CLOSE_CURRENT_WIDGET_BTN_TITLE.c_str(), btnSize)) {
            close_current_widget();
        }

        if(widgets_.size() == 1) {
            ImGui::EndDisabled();
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

void WidgetManager::reload_settings() {
    settings_ = XYZLabs::instance()
    .settings_manager()
    .fetch_settings<AppSettings>(
        constants::MAIN_APP_SETTINGS_LABEL
    );

    auto [r, g, b, a] = settings_.mainWindowColor;
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(r * 255, g * 255, b * 255, 255));
    
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = settings_.mainWindowFontScale;
}
