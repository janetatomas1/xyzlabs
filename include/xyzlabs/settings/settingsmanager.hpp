
#pragma once

#include <memory>
#include <boost/unordered_map.hpp>
#include <nlohmann/json.hpp>

#include "xyzlabs/settings/setting.hpp"

namespace xyzlabs {

using json = nlohmann::json;

class XYZLabs;

template <typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_cast(std::unique_ptr<Base>&& base) {
    if (auto ptr = dynamic_cast<Derived*>(base.get())) {
        base.release();
        return std::unique_ptr<Derived>(ptr);
    }
    return nullptr;
}

class SettingsGroup: public SettingInterface {
    boost::unordered_map<std::string, std::unique_ptr<SettingInterface>> settings_;

public:
    SettingInterface* add_setting(const std::string &path, std::unique_ptr<SettingInterface> ptr);
    SettingInterface* get(const std::string &path);
    SettingInterface* add_child(const std::string &path, std::unique_ptr<SettingInterface> child);
    SettingInterface* get_child(const std::string &path) override;
    json to_json() const override;
    void from_json(const json& j) override;
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
    SettingsGroup() = default;
};

class SettingsManager {
    bool settingsOpen_ = false;
    std::unique_ptr<SettingsGroup> mainGroup_;
    XYZLabs *app_;

public:
    SettingsManager(XYZLabs *app);
    template<SettingType S, typename... Args>
    SettingInterface* add_setting(const std::string &path, const std::string &label, Args... args);
    SettingInterface* add_setting(const std::string &path, std::unique_ptr<SettingInterface> ptr);
    SettingInterface* get(const std::string &path);
    std::unique_ptr<SettingsGroup> clone_settings();
    void receive_settings(std::unique_ptr<SettingsGroup> group);
    void init();
    std::string config_file();
    void save();
    void load();
    void load_safe();
    XYZLabs *app();
};

template<SettingType S, typename... Args>
SettingInterface* SettingsManager::add_setting(const std::string &path, const std::string &label, Args... args) {
    std::unique_ptr<SettingInterface> setting = std::make_unique<S>(label, std::forward<Args>(args)...);
    return add_setting(path, std::move(setting));
}

}
