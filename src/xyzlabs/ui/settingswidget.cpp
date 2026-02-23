
#include "xyzlabs/utils/operators.hpp"

#include "xyzlabs/ui/settingswidget.hpp"

namespace xyzlabs {

SettingsWidget::SettingsWidget(
    SettingsManager& settingsManager,
    const std::string& title,
    Widget *parent,
    Window *window
): Widget(title, parent, window), settingsManager_(settingsManager) {
    mainGroup_ = settingsManager_.clone_settings();
}

void SettingsWidget::show(const ImVec2 &size, const ImVec2 &pos) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, rowSize));
    const ImVec2 scrollRegionSize = pos + size * ImVec2{0.98f, 0.8f};
    const ImVec2 saveBtnSize = pos + size * ImVec2{0.2f, 0.07f};
    const ImVec2 discardBtnPos = pos + size * ImVec2{0.52f, 0.9f};
    const ImVec2 saveBtnPos = pos + size * ImVec2{0.75f, 0.9f};

    ImGui::BeginChild("ScrollRegion", scrollRegionSize, true, ImGuiWindowFlags_HorizontalScrollbar);

    mainGroup_->show("");

    ImGui::EndChild();

    ImGui::SetCursorPos(discardBtnPos);
    if(ImGui::Button("Discard changes", saveBtnSize)) {
        reject_callback();
    }

    ImGui::SetCursorPos(saveBtnPos);
    if(ImGui::Button("Save changes", saveBtnSize)) {
        auto group = dynamic_unique_cast<SettingsGroup, SettingInterface>(std::move(mainGroup_));
        accept_callback();
    }
    ImGui::PopStyleVar(ImGuiStyleVar_FramePadding);
}

SettingsManager& SettingsWidget::settings_manager() {
    return settingsManager_;
}

}
