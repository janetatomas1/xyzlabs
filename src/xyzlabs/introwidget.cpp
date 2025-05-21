
#include <imgui.h>
#include <portable-file-dialogs.h>

#include "introwidget.hpp"
#include "constants.hpp"
#include "xyzlabs.hpp"


IntroWidget::IntroWidget(): Widget("Welcome !!!") {}

void IntroWidget::show(ImVec2 &size, ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    ImGui::SetCursorPosX(size.x * 0.45);
    ImGui::SetCursorPosY(size.y * 0.45);

    if(ImGui::Button(constants::LOAD_FILE_BUTTON_TITLE.c_str(), loadFileBtnSize_)) {
    }
}
