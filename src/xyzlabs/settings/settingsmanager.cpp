
#include <fstream>
#include <imgui.h>
#include "xyzlabs/settings/settingsmanager.hpp"

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
    if(fs::exists(fs::path(file))) {
        std::fstream config(file, std::ios::in);
        config >> jv;
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
    auto jv0 = settings->to_json();
    auto jv1 = load_json_safe(file);
    auto merged = merge(jv0, jv1);
    save_json(merged, file);
}

SettingsManager::SettingsManager() {}

SettingInterface* SettingsManager::add_setting(const std::string &path, std::unique_ptr<SettingInterface> setting) {
    auto ptr = mainGroup_->add_setting(path, std::move(setting));
    load_safe();
    return ptr;
}

void SettingsManager::receive_settings(std::unique_ptr<SettingsGroup> group) {
    save_safe(std::move(group), config_file());
    load_safe();
}

std::unique_ptr<SettingsGroup> SettingsManager::clone_settings() {
    auto ptr = mainGroup_->clone().release();
    auto cast = dynamic_cast<SettingsGroup*>(ptr);
    return std::unique_ptr<SettingsGroup>(cast);
}

void SettingsManager::init(const std::string &configFile) {
    mainGroup_ = std::make_unique<SettingsGroup>();
    load_safe();
}

std::string SettingsManager::config_file() {
    return configFile_;
}

void SettingsManager::set_config_file(const std::string &configFile) {
    configFile_ = configFile;
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
    const auto jv = load_json_safe(configFile_);
    mainGroup_->from_json(jv);
}

}
