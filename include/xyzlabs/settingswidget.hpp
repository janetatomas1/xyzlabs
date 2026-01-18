
#pragma once

#include "xyzlabs/widget.hpp"
#include "xyzlabs/setting.hpp"
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/window.hpp"

namespace xyzlabs {

class SettingsWidget : public Widget {
public:
    SettingsWidget(
        SettingsManager& settingsManager,
        const std::string& title,
        Widget *parent,
        Window *window
    );
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    virtual void accept_callback() {};
    virtual void reject_callback() {};
    SettingsManager& settings_manager();

private:
    std::unique_ptr<SettingInterface> mainGroup_;
    SettingsManager& settingsManager_;
};

}
