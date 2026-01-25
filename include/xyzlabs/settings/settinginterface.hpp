
#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace xyzlabs {

class SettingInterface {
protected:
public:
    virtual ~SettingInterface() = default;
    virtual SettingInterface* add_child(const std::string &key, std::unique_ptr<SettingInterface> child) { return nullptr; };
    virtual json to_json() const = 0;
    virtual void from_json(const nlohmann::json& j) = 0;
    virtual void show(const std::string &label) = 0;
    virtual std::unique_ptr<SettingInterface> clone() const = 0;
    virtual SettingInterface* get_child(const std::string &path) { return nullptr; }
};

}
