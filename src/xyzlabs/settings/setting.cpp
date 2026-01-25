
#include <imgui.h>
#include <imgui_stdlib.h>
#include <fmt/format.h>

#include "xyzlabs/settings/setting.hpp"

namespace xyzlabs {

std::string format_label(const std::string& l) {
    return fmt::format("##{}", l);
}

void TextSetting::show(const std::string &label) {
    ImGui::InputText(format_label(label_).c_str(), &value_);
}

std::unique_ptr<SettingInterface> TextSetting::clone() const {
    return std::make_unique<TextSetting>(label_, value_);
}

void MultilineTextSetting::show(const std::string &label) {
    ImGui::InputTextMultiline(format_label(label_).c_str(), &value_);
}

std::unique_ptr<SettingInterface> MultilineTextSetting::clone() const {
    return std::make_unique<MultilineTextSetting>(label_, value_);
}

void ColorSetting::show(const std::string &label) {
    ImGui::ColorEdit4(format_label(label_).c_str(), &value_[0]);
}

std::unique_ptr<SettingInterface> ColorSetting::clone() const {
    return std::make_unique<ColorSetting>(label_, value_);
}

json FloatSetting::to_json() const {
    return {
        {"step", step},
        {"step_fast", step_fast},
        {"value", value_},
    };
}

void FloatSetting::from_json(const json &jv) {
    step = jv["step"];
    step_fast = jv["step_fast"];
    value_ = jv["value"];
}

void FloatSetting::show(const std::string &label) {
    ImGui::InputFloat(format_label(label_).c_str(), &value_, step, step_fast);
}

std::unique_ptr<SettingInterface> FloatSetting::clone() const {
    return std::make_unique<FloatSetting>(label_, value_, step, step_fast);
}

json FloatSliderSetting::to_json() const {
    return {
        {"min", min},
        {"max", max},
        {"value", value_},
    };
}

void FloatSliderSetting::from_json(const json &jv) {
    min = jv["min"];
    max = jv["max"];
    value_ = jv["value"];
}

void FloatSliderSetting::show(const std::string &label) {
    ImGui::SliderFloat(format_label(label_).c_str(), &value_, min, max);
}

std::unique_ptr<SettingInterface> FloatSliderSetting::clone() const {
    return std::make_unique<FloatSliderSetting>(label_, value_, min, max);
}

}
