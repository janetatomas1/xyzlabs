
#include <imgui.h>
#include <exception>

#include "xyzlabs/settings/settingsgroup.hpp"

namespace xyzlabs {

SettingInterface* SettingsGroup::get_child(const std::string &path) {
    auto idx = path.find(".");

    auto prefix = idx == std::string::npos ? path : path.substr(0, idx);
    auto suffix = idx == std::string::npos ? "" : path.substr(idx + 1, path.size() - idx - 1);

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

SettingInterface* SettingsGroup::get(const std::string &path) {
    return get_child(path);
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

}
