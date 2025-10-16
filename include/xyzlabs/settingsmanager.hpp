
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <memory>
#include <imgui.h>
#include <boost/unordered_map.hpp>
#include <nlohmann/json.hpp>

#include "xyzlabs/widget.hpp"

using json = nlohmann::json;

struct Settings {
    void virtual show_input_widget() {};
    json virtual serialize() const {
        return {};
    };
    void virtual deserialize(const json &obj) {};
};

template<typename T>
concept SettingsType = std::derived_from<T, Settings> || std::same_as<T, Settings>;

struct SettingsWidget: public Widget {
    boost::unordered_map<std::string, std::unique_ptr<Settings>> store_;
    void show(const ImVec2& size, const ImVec2& pos) override;
};

class SettingsManager {
    json cache_;
    boost::unordered_map<std::string, std::unique_ptr<Settings>> store_;
    bool settingsOpen_ = false;

public:
    SettingsManager();
    template <SettingsType S>
    void register_settings(const std::string &label);
    void load(const std::string &filepath) {};
    void save(const std::string &filepath) {};
    void show_settings_window(const ImVec2 &size);
    bool settings_open() const {
        return settingsOpen_;
    }
    void set_settings_open(bool value) {
        settingsOpen_ = value;
    }
    const std::string config_file();
    void init();
    void save();
    void save_safe();
    json serialize();
    void deserialize(const json& jv);
    template <SettingsType S>
    S fetch_settings(const std::string &label);
    void load();
    void load_safe();
    void propagate();
};

template <SettingsType S>
void SettingsManager::register_settings(const std::string &label) {
    store_[label] = std::make_unique<S>();
}

template <SettingsType S>
S SettingsManager::fetch_settings(const std::string &label) {
    S *ptr = (S*)store_[label].get();
    S instance = *ptr;
    return instance;
}

#endif
