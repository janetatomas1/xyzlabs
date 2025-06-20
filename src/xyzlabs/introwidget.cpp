
#include <imgui.h>
#include <portable-file-dialogs.h>

#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/burningforest.hpp"


IntroWidget::IntroWidget(): Widget("Welcome !!!") {}

void IntroWidget::show(const ImVec2 &size, const ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    ImGui::SetCursorPosX(size.x * 0.40);
    ImGui::SetCursorPosY(size.y * 0.45);
    ImVec2 btnSize = {250.0, 30.0};

    if(ImGui::Button(constants::BURNING_FOREST_BTN.c_str(), btnSize)) {
        XYZLabs::instance()
        .widget_manager()
        .add_widget<BurningForest>();
    }
}
