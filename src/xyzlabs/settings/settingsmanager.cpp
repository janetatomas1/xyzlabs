
#include <fstream>

#include "xyzlabs/settings/settingsmanager.hpp"
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

SettingsManager::SettingsManager(XYZLabs *app) : app_(app) {}

SettingInterface* SettingsManager::add_setting(const std::string &path, std::unique_ptr<SettingInterface> setting) {
    auto ptr = mainGroup_->add_setting(path, std::move(setting));
    load_safe();
    return ptr;
}

void SettingsManager::receive_settings(std::unique_ptr<SettingsGroup> group) {
    save_safe(std::move(group), config_file());
    app()->event_manager().add_action([this]() {
        load_safe();
    });
}


std::unique_ptr<SettingsGroup> SettingsManager::clone_settings() {
    auto ptr = mainGroup_->clone().release();
    auto cast = dynamic_cast<SettingsGroup*>(ptr);
    return std::unique_ptr<SettingsGroup>(cast);
}

std::string SettingsManager::config_file() {
    return (app()->app_directory() / "config.json").string();
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

XYZLabs *SettingsManager::app() {
    return app_;
}

}
