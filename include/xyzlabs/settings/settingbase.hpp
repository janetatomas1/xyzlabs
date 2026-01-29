#pragma once

#include "xyzlabs/settings/settinginterface.hpp"

namespace xyzlabs {

/**
 * @brief Base class for concrete (leaf) settings.
 *
 * SettingBase provides shared functionality for all non-group settings.
 * It stores a human-readable label used for UI presentation and
 * implements common behavior expected by SettingsGroup when rendering
 * leaf nodes.
 *
 * This class does not store any actual setting value; that responsibility
 * is delegated to derived classes (e.g. Setting<T>).
 *
 * SettingsGroup relies on runtime type identification to distinguish
 * between:
 *  - SettingsGroup instances (composite nodes), and
 *  - SettingBase-derived instances (leaf nodes).
 */
class SettingBase : public SettingInterface {
protected:
    /**
     * @brief Human-readable label for UI display.
     */
    std::string label_;

public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~SettingBase() = default;

    /**
     * @brief Construct a leaf setting with the given label.
     *
     * @param label Display label for this setting.
     */
    SettingBase(const std::string &label)
        : label_(label) {}

    /**
     * @brief Retrieve the UI label for this setting.
     *
     * Used by SettingsGroup when rendering tables and controls.
     *
     * @return Null-terminated string containing the label.
     */
    virtual const char* label() { return label_.c_str(); }
};

} // namespace xyzlabs
