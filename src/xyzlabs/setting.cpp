
#include <imgui.h>
#include <imgui_stdlib.h>
#include <fmt/format.h>


#include "xyzlabs/setting.hpp"

void TextSetting::show(const std::string &label) {
    ImGui::InputText(fmt::format("##{}", label_).c_str(), &value_);
}

std::unique_ptr<SettingInterface> TextSetting::clone() const {
    return std::make_unique<TextSetting>(label_, value_);
}

void MultilineTextSetting::show(const std::string &label) {
    ImGui::InputTextMultiline(fmt::format("##{}", label_).c_str(), &value_);
}

std::unique_ptr<SettingInterface> MultilineTextSetting::clone() const {
    return std::make_unique<MultilineTextSetting>(label_, value_);
}

void ColorSetting::show(const std::string &label) {
    ImGui::ColorEdit4("", &value_[0]);
}

std::unique_ptr<SettingInterface> ColorSetting::clone() const {
    return std::make_unique<ColorSetting>(label_, value_);
}
