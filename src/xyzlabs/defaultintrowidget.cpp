
#include <imgui.h>
#include <portable-file-dialogs.h>

#include "xyzlabs/defaultintrowidget.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/burningforest.hpp"


DefaultIntroWidget::DefaultIntroWidget(): Widget("Intro") {}

void DefaultIntroWidget::show(const ImVec2 &size, const ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    ImGui::SetCursorPos({size.x * 0.37f, size.y * 0.45f});
    ImVec2 btnSize = {size.x * 0.15f, size.y * 0.035f};

    if(ImGui::Button(constants::BURNING_FOREST_BTN.c_str(), btnSize)) {
        XYZLabs::instance()
        .widget_manager()
        .add_widget<BurningForest>();
    }
}
