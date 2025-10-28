
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <nlohmann/json.hpp>
#include <array>

using json = nlohmann::json;

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
    virtual const char* label() { return nullptr; };
};

template<typename T>
class Setting: public SettingInterface {
protected:
    T value_;
    std::string label_;

public:
    Setting(const std::string &label, T value):
        SettingInterface(),
        value_(value), label_(label) {};
    T* get() { return &value_; };
    T& get_ref() { return value_; };
    json to_json() const override;
    void from_json(const json& j) override;
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
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

template <typename T>
concept SettingType = std::derived_from<T, SettingInterface>;

struct TextSetting: public Setting<std::string> {
public:
    TextSetting(const std::string &label, const std::string &value): Setting(label, value) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

struct MultilineTextSetting: public Setting<std::string> {
public:
    MultilineTextSetting(const std::string &label, const std::string &value): Setting(label, value) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

struct ColorSetting: public Setting<std::array<float, 4>> {
public:
    ColorSetting(const std::string &label, std::array<float, 4> value): Setting(label, value) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

struct FloatSetting: public Setting<float> {
    float step, step_fast;
    FloatSetting(
        const std::string &label,
        float value, float step = 0.0f,
        float step_fast = 0.0f
    ): Setting(label, value), step(step), step_fast(step_fast) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

struct FloatSliderSetting: public Setting<float> {
    float min, max;
    FloatSliderSetting(
        const std::string &label,
        float value, float step = 0.0f,
        float max = 100.0f
    ): Setting(label, value), min(min), max(max) {};
    void show(const std::string &label) override;
    std::unique_ptr<SettingInterface> clone() const override;
};

#endif
