
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <memory>
#include <imgui.h>
#include <boost/unordered_map.hpp>
#include <boost/json.hpp>


struct Settings {
    void virtual show_input_widget() {};
    boost::json::object virtual serialize() const {
        return {};
    };
    void virtual deserialize(const boost::json::object &obj) {};
};

template<typename T>
concept SettingsType = std::derived_from<T, Settings> || std::same_as<T, Settings>;

class SettingsManager {
    boost::unordered_map<std::string, std::unique_ptr<Settings>> store_;

public:
    SettingsManager();
    template <SettingsType S>
    void register_settings(const std::string &label);
    void load(const std::string &filepath) {};
    void save(const std::string &filepath) {};
    void show_settings_widget(const ImVec2 &size);
};

template <SettingsType S>
void SettingsManager::register_settings(const std::string &label) {
    store_[label] = std::make_unique<S>();
}

#endif
