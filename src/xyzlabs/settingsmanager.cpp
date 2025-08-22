
#include <fmt/format.h>
#include <filesystem>
#include <fstream>

#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/constants.hpp"

SettingsManager::SettingsManager() {}

void SettingsManager::show_settings_window(const ImVec2 &size) {
    ImVec2 settingsWindowSize = {size.x * 0.5f, size.y * 0.5f};
    ImVec2 settingsWindowPos = {settingsWindowSize.x * 0.65f, settingsWindowSize.y * 0.4f};

    const ImVec2 scrollRegionSize = {settingsWindowSize.x * 0.98f, settingsWindowSize.y * 0.8f};
    const ImVec2 saveBtnSize = {settingsWindowSize.x * 0.2f, settingsWindowSize.y * 0.07f};
    const ImVec2 discardBtnPos = {settingsWindowSize.x * 0.52f, settingsWindowSize.y * 0.9f};
    const ImVec2 saveBtnPos = {settingsWindowSize.x * 0.75f, settingsWindowSize.y * 0.9f};

    ImGui::SetNextWindowPos(settingsWindowPos);
    ImGui::SetNextWindowSize(settingsWindowSize);

    if(settingsOpen_ && ImGui::Begin(constants::SETTINGS_WINDOW_TITLE.c_str())) {
        ImGui::BeginChild("ScrollRegion", scrollRegionSize, true, ImGuiWindowFlags_HorizontalScrollbar);
        for(auto &ref: store_) {
            if(ImGui::TreeNode(ref.first.c_str())) {
                ref.second->show_input_widget();
                ImGui::TreePop();
            }
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(discardBtnPos);

        if(ImGui::Button(constants::DISCARD_SETTINGS_BTN_TITLE.c_str(), saveBtnSize)) {
            settingsOpen_ = false;
        }

        ImGui::SetCursorPos(saveBtnPos);

        if(ImGui::Button(constants::SAVE_SETTINGS_BTN_TITLE.c_str(), saveBtnSize)) {
            save();
            XYZLabs::instance().widget_manager().enable_settings_reload();
            settingsOpen_ = false;
        }

        ImGui::End();
    }
};

const std::string SettingsManager::config_file() {
    return XYZLabs::instance().app_directory() / "config.json";
}

void SettingsManager::init() {
    auto appDirectory = XYZLabs::instance().create_app_directory();
    auto configFile = config_file();
}

void SettingsManager::save() {
    std::fstream config(config_file(), std::ios::out);
    config << serialize().dump(2);
}

json SettingsManager::serialize() {
    json jv;

    for(const auto& ref: store_) {
        jv[ref.first] = ref.second->serialize();
    }

    return jv;
}
