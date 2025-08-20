
#include "xyzlabs/settingsmanager.hpp"

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, const std::unique_ptr<Settings> &ptr) {
    jv = std::move(ptr->serialize());
}

SettingsManager::SettingsManager() {

}

void SettingsManager::show_settings_widget(const ImVec2 &size) {
    
    for(auto &ref: store_) {
        ref.second->show_input_widget();
        ImGui::Separator();
    }
};
