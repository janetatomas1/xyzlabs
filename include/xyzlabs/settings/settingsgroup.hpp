
#pragma once

#include <boost/unordered_map.hpp>

#include "xyzlabs/settings/settinginterface.hpp"
#include "xyzlabs/settings/settingbase.hpp"

namespace xyzlabs {

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

}
