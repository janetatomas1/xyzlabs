
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/widget.hpp"

class SettingsWidget: public Widget {
    SettingsGroup *mainGroup_;
public:
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    SettingsWidget(
        SettingsGroup *group
    ): Widget("Settings"), mainGroup_(group) {}
};

void SettingsWidget::show(const ImVec2 &size, const ImVec2 &pos) {    
    const ImVec2 settingsWindowPos = {size.x * 0.65f, size.y * 0.4f};
    const ImVec2 scrollRegionSize = {size.x * 0.98f, size.y * 0.8f};
    const ImVec2 saveBtnSize = {size.x * 0.2f, size.y * 0.07f};
    const ImVec2 discardBtnPos = {size.x * 0.52f, size.y * 0.9f};
    const ImVec2 saveBtnPos = {size.x * 0.75f, size.y * 0.9f};

    ImGui::BeginChild("ScrollRegion", scrollRegionSize, true, ImGuiWindowFlags_HorizontalScrollbar);
    for(auto &ref: mainGroup_->settings_) {
        if(ImGui::TreeNode(ref.first.c_str())) {
            ref.second->show();
            ImGui::TreePop();
        }
    }
    ImGui::EndChild();

    ImGui::SetCursorPos(discardBtnPos);
    if(ImGui::Button("Discard changes", saveBtnSize)) {}

    ImGui::SetCursorPos(saveBtnPos);
    if(ImGui::Button("Save changes", saveBtnSize)) {}
}

SettingsInterface* SettingsGroup::add_setting(const std::string &path, std::unique_ptr<SettingsInterface> setting) {
}

std::pair<SettingsGroup*, std::string> SettingsGroup::find_group(const std::string &path) {
    auto idx = path.find(".");
    
    if(idx == std::string::npos) {
        return {this, ""};
    } else {
        std::string prefix = path.substr(0, idx);
        std::string suffix = path.substr(idx + 1);
        if(groups_.contains(prefix)) {
            return groups_[prefix].find_group(suffix);
        } else {
            return {this, path};
        }
    }
}

void SettingsManager::init() {}

SettingsInterface* SettingsManager::add_setting(const std::string &path, std::unique_ptr<SettingsInterface> ptr) {
    return mainGroup_.add_setting(path, std::move(ptr));
}

void SettingsManager::open_settings() {
    if(!settingsOpen_) {
        settingsOpen_ = true;
        Window *currentWindow = window_manager().get_current_window();
        Window *window = window_manager().add_window<Window>(
            "Settings", currentWindow->width() * 0.8
            , currentWindow->height() * 0.8
        );
        window->set_central_widget<SettingsWidget>(&mainGroup_);
    }
}
