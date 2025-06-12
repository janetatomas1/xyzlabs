
#include <imgui.h>
#include <portable-file-dialogs.h>

#include "xyzlabs/introwidget.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"


IntroWidget::IntroWidget(): Widget("Welcome !!!") {}

void IntroWidget::show(const ImVec2 &size, const ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    ImGui::SetCursorPosX(size.x * 0.45);
    ImGui::SetCursorPosY(size.y * 0.45);

    if(ImGui::Button(constants::LOAD_FILE_BUTTON_TITLE.c_str(), loadFileBtnSize_)) {
    }
}
