
#pragma once

#include <array>
#include "xyzlabs/settings/settinginterface.hpp"
#include "xyzlabs/settings/settingbase.hpp"

using json = nlohmann::json;

namespace xyzlabs {

/**
* @brief Generic typed leaf setting.
*
* Setting<T> represents a concrete, leaf-level setting holding
* a value of type @p T. It provides default JSON serialization
* and cloning behavior.
*
* UI rendering is intentionally left empty and must be implemented
* by derived types if user interaction is required.
*
* @tparam T Stored value type. Must be compatible with
*           nlohmann::json serialization.
*/
template<typename T>
class Setting: public SettingBase {
protected:
    /**
    * @brief Stored value of the setting.
    */
    T value_;

public:
    /**
    * @brief Construct a setting with an initial value.
    *
    * @param label Human-readable label for UI display.
    * @param value Initial value of the setting.
    */
    Setting(const std::string &label, T value):
        SettingBase(label),
        value_(value) {};
    /**
    * @brief Access the stored value as a pointer.
    *
    * Intended for UI libraries that require pointer-based APIs.
    */
    T* get() { return &value_; };

    /**
     * @brief Access the stored value by reference.
     */
    T& get_ref() { return value_; };

    /**
     * @brief Serialize the setting value to JSON.
     *
     * Default implementation serializes the raw value.
     */
    json to_json() const override;

    /**
     * @brief Deserialize the setting value from JSON.
     *
     * Default implementation deserializes the raw value.
     */
    void from_json(const json& j) override;


    /**
     * @brief Render the setting in the UI.
     *
     * Default implementation does nothing. Derived classes
     * are expected to override this.
     */
    void show(const std::string &label) override;

    /**
     * @brief Create a deep copy of this setting.
     */
    std::unique_ptr<SettingInterface> clone() const override;

    /**
     * @brief Retrieve the label used for UI display.
     */
    const char* label() override { return label_.c_str(); };
};

template<typename T>
json Setting<T>::to_json() const {
    return value_;
}

template<typename T>
void Setting<T>::from_json(const json& j) {
    value_ = j.get<T>();
}

template<typename T>
void Setting<T>::show(const std::string &label) {}

template<typename T>
std::unique_ptr<SettingInterface> Setting<T>::clone() const {
    return std::make_unique<Setting<T>>(label_, value_);
}

/**
 * @brief Concept restricting setting types to SettingBase derivatives.
 */
template <typename T>
concept SettingType = std::derived_from<T, SettingBase>;

// -----------------------------------------------------------------------------
// Concrete setting types
// -----------------------------------------------------------------------------

/**
 * @brief Single-line text setting.
 *
 * Rendered using ImGui::InputText.
 */
struct TextSetting: public Setting<std::string> {
public:
    TextSetting(const std::string &label, const std::string &value): Setting(label, value) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

/**
 * @brief Multi-line text setting.
 *
 * Rendered using ImGui::InputTextMultiline.
 */
struct MultilineTextSetting: public Setting<std::string> {
public:
    MultilineTextSetting(const std::string &label, const std::string &value): Setting(label, value) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

/**
 * @brief RGBA color setting.
 *
 * Stored as an array of four floats and rendered using
 * ImGui::ColorEdit4.
 */
struct ColorSetting: public Setting<std::array<float, 4>> {
public:
    ColorSetting(const std::string &label, std::array<float, 4> value): Setting(label, value) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

/**
 * @brief Floating-point input setting with optional step sizes.
 *
 * Serialized as an object containing:
 *  - value
 *  - step
 *  - step_fast
 *
 * Rendered using ImGui::InputFloat.
 */
struct FloatSetting: public Setting<float> {
    float step, step_fast;
    FloatSetting(
        const std::string &label,
        float value, float step = 0.0f,
        float step_fast = 0.0f
    ): Setting(label, value), step(step), step_fast(step_fast) {};
    json to_json() const override;
    void from_json(const json &jv) override;
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

/**
 * @brief Floating-point slider setting.
 *
 * Serialized as an object containing:
 *  - value
 *  - min
 *  - max
 *
 * Rendered using ImGui::SliderFloat.
 */
struct FloatSliderSetting: public Setting<float> {
    float min = 0.0f, max = 100.0f;
    FloatSliderSetting(
        const std::string &label,
        float value, float step = 0.0f,
        float min = 0.0f, float max = 100.0f
    ): Setting(label, value), min(min), max(max) {};
    json to_json() const override;
    void from_json(const json &jv) override;
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

}
