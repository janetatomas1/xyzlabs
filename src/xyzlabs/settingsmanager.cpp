
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/widget.hpp"

class SettingsWidget: public Widget {
    std::unique_ptr<SettingInterface> mainGroup_;
public:
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    SettingsWidget(
        std::unique_ptr<SettingInterface> group
    ): Widget("Settings"), mainGroup_(std::move(group)) {}
};

    void SettingsWidget::show(const ImVec2 &size, const ImVec2 &pos) {    
    const ImVec2 settingsWindowPos = {size.x * 0.65f, size.y * 0.4f};
    const ImVec2 scrollRegionSize = {size.x * 0.98f, size.y * 0.8f};
    const ImVec2 saveBtnSize = {size.x * 0.2f, size.y * 0.07f};
    const ImVec2 discardBtnPos = {size.x * 0.52f, size.y * 0.9f};
    const ImVec2 saveBtnPos = {size.x * 0.75f, size.y * 0.9f};

    ImGui::BeginChild("ScrollRegion", scrollRegionSize, true, ImGuiWindowFlags_HorizontalScrollbar);

    mainGroup_->show();

    ImGui::EndChild();

    ImGui::SetCursorPos(discardBtnPos);
    if(ImGui::Button("Discard changes", saveBtnSize)) {}

    ImGui::SetCursorPos(saveBtnPos);
    if(ImGui::Button("Save changes", saveBtnSize)) {}
}

SettingInterface* SettingsGroup::get_child(const std::string &path) {
    auto idx = path.find(".");
    auto prefix = idx == std::string::npos ? path.substr(0, idx) : path;
    auto suffix = idx == std::string::npos ? path.substr(idx + 1, path.size() - idx - 1) : "";

    if(settings_.contains(prefix)) {
        if(suffix.empty()) {
            return settings_[prefix].get();
        } else {
            return settings_[prefix]->get_child(suffix);
        }
    } else {
        return nullptr;
    }
}

SettingInterface* SettingsGroup::add_child(const std::string &path, std::unique_ptr<SettingInterface> child) {
    auto idx = path.find(".");
    auto prefix = idx == std::string::npos ? path : path.substr(0, idx);
    auto suffix = idx == std::string::npos ? "" : path.substr(idx + 1, path.size() - idx - 1);

    if(settings_.contains(prefix)) {
        if(suffix.empty()) {
            return nullptr;
        } else {
            return settings_[prefix]->add_child(suffix, std::move(child));
        }
    } else {
        if(suffix.empty()) {
            settings_[prefix] = std::move(child);
            return settings_[prefix].get();
        } else {
            settings_[prefix] = std::move(std::make_unique<SettingsGroup>());
            return settings_[prefix]->add_child(suffix, std::move(child));
        }
    }
}

json SettingsGroup::to_json() const {
    json jv;
    for(const auto &[key, value]: settings_) {
        jv[key] = value->to_json();
    }

    return jv;
}

void SettingsGroup::from_json(const json& jv) {
    for(auto &[key, value]: settings_) {
        if(jv.contains(key)) {
            value->from_json(jv[key]);
        }
    }
}

void SettingsGroup::show() {
    for(auto &[key, value]: settings_) {
        if(ImGui::TreeNode(key.c_str())) {
            value->show();
            ImGui::TreePop();
        }
    }
}

std::unique_ptr<SettingInterface> SettingsGroup::clone() const {
    std::unique_ptr<SettingInterface> ptr = std::make_unique<SettingsGroup>();

    for(const auto &[key, value]: settings_) {
        ptr->add_child(key, value->clone());
    }

    return ptr;
}

SettingInterface* SettingsGroup::add_setting(const std::string &path, std::unique_ptr<SettingInterface> setting) {
    return add_child(path, std::move(setting));
}

void SettingsManager::init() {
    mainGroup_ = std::make_unique<SettingsGroup>();
}

SettingInterface* SettingsManager::add_setting(const std::string &path, std::unique_ptr<SettingInterface> ptr) {
    return mainGroup_->add_setting(path, std::move(ptr));
}

void SettingsManager::open_settings() {
    if(!settingsOpen_) {
        settingsOpen_ = true;
        Window *currentWindow = window_manager().get_current_window();
        Window *window = window_manager().add_window<Window>(
            "Settings",
            currentWindow->width() * 0.8,
            currentWindow->height() * 0.8
        );

        std::unique_ptr<Widget> widget = std::make_unique<SettingsWidget>(std::move(mainGroup_->clone()));
        window->set_central_widget(std::move(widget));
    }
}
