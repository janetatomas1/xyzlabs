
#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <memory>
#include <boost/unordered_map.hpp>
#include <nlohmann/json.hpp>

#include "xyzlabs/settings.hpp"

using json = nlohmann::json;

struct SettingsGroup {
    boost::unordered_map<std::string, std::unique_ptr<SettingsInterface>> settings_;
    boost::unordered_map<std::string, SettingsGroup> groups_;

    SettingsInterface* add_setting(const std::string &path, std::unique_ptr<SettingsInterface> ptr);
    SettingsInterface* get(const std::string &path);
    std::pair<SettingsGroup*, std::string> find_group(const std::string &path);
    SettingsGroup() = default;
};

class SettingsManager {
    bool settingsOpen_ = false;
    SettingsGroup mainGroup_;

public:
    SettingsInterface* add_setting(const std::string &path, std::unique_ptr<SettingsInterface> ptr);
    SettingsInterface* get(const std::string &path);
    void open_settings();
    void close_settings();
    void init();
};

#endif
