
#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SettingInterface {
protected:
public:
    virtual ~SettingInterface() = default;
    virtual json to_json() const = 0;
    virtual void from_json(const nlohmann::json& j) = 0;
    virtual void show() = 0;
    virtual std::unique_ptr<SettingInterface> clone() = 0;
};

template<typename T>
class Setting: public SettingInterface {
protected:
    T value_;
    std::string label_;

public:
    Setting(const std::string &label, T &value):
        SettingInterface(label),
        value_(value) {};
    T* get() { return &value_; };
    T& get_ref() { return value_; };
    json to_json() const override;
    void from_json(const json& j) override;
    void show() override;
    
    const std::string label() { return label_; };
};

template<typename T>
json Setting<T>::to_json() const {
    if constexpr(std::is_arithmetic_v<T> || std::is_same_v<T, std::string>) {
        return value_;
    }
}

template<typename T>
void Setting<T>::from_json(const json& j) {
    value_ = j.get<T>();
}

template<typename T>
void Setting<T>::show() {
}

#endif
