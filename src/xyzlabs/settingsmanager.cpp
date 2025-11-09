
#include <fstream>

#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/widget.hpp"
#include "xyzlabs/xyzlabs.hpp"

namespace fs = std::filesystem;

namespace xyzlabs {

json merge(const json& a, const json& b) {
    if (!a.is_object() || !b.is_object()) {
        return a.is_null() ? b : a;
    }

    json result = a;

    for (auto it = b.begin(); it != b.end(); ++it) {
        const auto& key = it.key();
        const auto& b_value = it.value();

        if (result.contains(key)) {
            result[key] = merge(result[key], b_value);
        } else {
            result[key] = b_value;
        }
    }

    return result;
}

json load_json_safe(const std::string &file) {
    json jv;
    if(!fs::exists(fs::path(file))) {
        std::fstream config(file, std::ios::in);
        try {
            config >> jv;
        } catch(...) {}
        config.close();
    }

    return jv;
}

void save_json(json jv, const std::string &file) {
    std::fstream config(file, std::ios::out);
    config << jv.dump(2);
    config.close();
}

void save_safe(std::unique_ptr<SettingInterface> settings, const std::string &file) {
    if(!fs::exists(fs::path(file).parent_path())) {
        fs::create_directory(fs::path(file).parent_path());
    }

    auto jv0 = settings->to_json();
    auto jv1 = load_json_safe(file);
    auto merged = merge(jv0, jv1);
    save_json(merged, file);
}

template <typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_cast(std::unique_ptr<Base>&& base) {
    if (auto ptr = dynamic_cast<Derived*>(base.get())) {
        base.release();
        return std::unique_ptr<Derived>(ptr);
    }
    return nullptr;
}

class SettingsWidget: public Widget {
    std::unique_ptr<SettingsGroup> mainGroup_;
public:
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    SettingsWidget(
        std::unique_ptr<SettingsGroup> group
    ): Widget("Settings"), mainGroup_(std::move(group)) {}
};

void SettingsWidget::show(const ImVec2 &size, const ImVec2 &pos) {    
    const ImVec2 settingsWindowPos = pos + size * ImVec2{0.65f, 0.4f};
    const ImVec2 scrollRegionSize = pos + size * ImVec2{0.98f, 0.8f};
    const ImVec2 saveBtnSize = pos + size * ImVec2{0.2f, 0.07f};
    const ImVec2 discardBtnPos = pos + size * ImVec2{0.52f, 0.9f};
    const ImVec2 saveBtnPos = pos + size * ImVec2{0.75f, 0.9f};

    ImGui::BeginChild("ScrollRegion", scrollRegionSize, true, ImGuiWindowFlags_HorizontalScrollbar);

    mainGroup_->show("");

    ImGui::EndChild();

    ImGui::SetCursorPos(discardBtnPos);
    if(ImGui::Button("Discard changes", saveBtnSize)) {
        window_manager().get_current_window()->close();
    }

    ImGui::SetCursorPos(saveBtnPos);
    if(ImGui::Button("Save changes", saveBtnSize)) {
        auto group = dynamic_unique_cast<SettingsGroup, SettingInterface>(std::move(mainGroup_));
        settings_manager().receive_settings(std::move(group));
        window_manager().get_current_window()->close();
    }
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
    json jv = json::object();
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

void SettingsGroup::show(const std::string &label) {
    bool hasLeafs = false;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
    if(settings_.size() == 1) {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }

    for(auto &[key, value]: settings_) {
        if(dynamic_cast<SettingsGroup*>(value.get()) != nullptr) {
            if(ImGui::TreeNodeEx(key.c_str(), flags)) {
                value->show(key);
                ImGui::TreePop();
            }
        } else {
            hasLeafs = true;
        }
    }

    if(hasLeafs) {
        if(ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Borders)) {
            for(auto &[key, value]: settings_) {
                auto ptr = dynamic_cast<SettingBase*>(value.get());
                if(ptr != nullptr) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", ptr->label());
                    ImGui::TableNextColumn();
                    value->show(key);
                }
            }
            ImGui::EndTable();
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

SettingInterface* SettingsManager::add_setting(const std::string &path, std::unique_ptr<SettingInterface> setting) {
    auto ptr = mainGroup_->add_setting(path, std::move(setting));
    load_safe();
    return ptr;
}

void SettingsManager::open_settings(int32_t width, int32_t height) {
    if(!settingsOpen_) {
        settingsOpen_ = true;
        Window *currentWindow = window_manager().get_current_window();
        Window *window = window_manager().add_window<Window>(
            "Settings",
            width,
            height
        );

        auto ptr = dynamic_cast<SettingsGroup*>(mainGroup_->clone().release());
        std::unique_ptr<Widget> widget = std::make_unique<SettingsWidget>(
            std::unique_ptr<SettingsGroup>(ptr)
        );
        window->set_central_widget(std::move(widget));
    }
}

void SettingsManager::receive_settings(std::unique_ptr<SettingsGroup> group) {
    save_safe(std::move(group), config_file());
    event_manager().add_action([this]() {
        load_safe();
    });
}

std::string SettingsManager::config_file() {
    return (app().app_directory() / "config.json").string();
}

void SettingsManager::init() {
    mainGroup_ = std::make_unique<SettingsGroup>();
    load_safe();
}

void SettingsManager::save() {
    std::fstream config(config_file(), std::ios::out);
    config << mainGroup_->to_json().dump(2);
    config.close();
}

void SettingsManager::load() {
    json jv;
    std::fstream config(config_file(), std::ios::in);
    config >> jv;
    mainGroup_->from_json(jv);
    config.close();
}

void SettingsManager::load_safe() {
    if(!std::filesystem::exists(config_file())) {
        save_safe(std::move(mainGroup_->clone()), config_file());
    }

    load();
}
}
