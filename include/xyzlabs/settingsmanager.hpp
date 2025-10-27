
#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <memory>
#include <boost/unordered_map.hpp>
#include <nlohmann/json.hpp>

#include "xyzlabs/settings.hpp"

using json = nlohmann::json;

class SettingsGroup: public SettingInterface {
    boost::unordered_map<std::string, std::unique_ptr<SettingInterface>> settings_;


public:
    SettingInterface* add_setting(const std::string &path, std::unique_ptr<SettingInterface> ptr);
    SettingInterface* get(const std::string &path);
    SettingInterface* add_child(const std::string &path, std::unique_ptr<SettingInterface> child);
    SettingInterface* get_child(const std::string &path) override;
    json to_json() const override;
    void from_json(const json& j) override;
    void show() override;
    std::unique_ptr<SettingInterface> clone() const override;
    SettingsGroup() = default;
};

class SettingsManager {
    bool settingsOpen_ = false;
    std::unique_ptr<SettingsGroup> mainGroup_;

public:
    SettingInterface* add_setting(const std::string &path, std::unique_ptr<SettingInterface> ptr);
    SettingInterface* get(const std::string &path);
    void open_settings();
    void close_settings();
    void init();
};

#endif
