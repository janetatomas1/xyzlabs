
#pragma once

#include "xyzlabs/ui/widget.hpp"
#include "xyzlabs/settings/setting.hpp"
#include "xyzlabs/settings/settingsmanager.hpp"
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
    void init() override;

private:
    float rowSize = 10.0f;
    std::unique_ptr<SettingsGroup> mainGroup_;
    SettingsManager& settingsManager_;
};

}
