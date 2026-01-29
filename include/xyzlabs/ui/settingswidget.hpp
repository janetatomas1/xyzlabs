
#pragma once

#include "xyzlabs/ui/widget.hpp"
#include "xyzlabs/settings/setting.hpp"
#include "xyzlabs/settings/settingsmanager.hpp"
#include "xyzlabs/window.hpp"

namespace xyzlabs {

/**
    * @brief Widget providing a UI for editing application settings.
    *
    * SettingsWidget presents a temporary, editable copy of the application's
    * settings using ImGui. Changes are made on a cloned SettingsGroup and are
    * only committed when explicitly accepted.
    *
    * Typical lifecycle:
    *  - On construction, settings are cloned from the SettingsManager
    *  - The cloned settings are rendered and edited
    *  - User either accepts (save) or rejects (discard) changes
    *
    * This design allows safe editing without modifying live settings until
    * confirmation.
    */
class SettingsWidget : public Widget {
public:
    /**
        * @brief Construct a SettingsWidget.
        *
        * Clones the current settings tree from the provided SettingsManager.
        *
        * @param settingsManager Reference to the global SettingsManager
        * @param title           Widget title
        * @param parent          Optional parent widget
        * @param window          Owning window
        */
    SettingsWidget(
        SettingsManager& settingsManager,
        const std::string& title,
        Widget* parent,
        Window* window
    );

    /**
        * @brief Render the settings UI.
        *
        * Displays all settings contained in the cloned SettingsGroup
        * inside a scrollable region, along with Save and Discard buttons.
        *
        * @param size Available widget size
        * @param pos  Widget position
        */
    void show(const ImVec2& size, const ImVec2& pos) override;

    /**
        * @brief Callback invoked when the user accepts changes.
        *
        * Intended to be overridden by derived classes. Typically used to
        * commit the modified settings back to the SettingsManager.
        */
    virtual void accept_callback() {}

    /**
        * @brief Callback invoked when the user discards changes.
        *
        * Intended to be overridden by derived classes. Typically used to
        * reset or close the settings UI without saving.
        */
    virtual void reject_callback() {}

    /**
        * @brief Access the associated SettingsManager.
        *
        * @return Reference to the settings manager
        */
    SettingsManager& settings_manager();

    /**
        * @brief Initialize the widget.
        *
        * Can be overridden for additional setup logic.
        */
    void init() override;

private:
    /// Vertical padding used for settings rows
    float rowSize = 10.0f;

    /// Editable clone of the settings tree
    std::unique_ptr<SettingsGroup> mainGroup_;

    /// Reference to the global settings manager
    SettingsManager& settingsManager_;
};

}
