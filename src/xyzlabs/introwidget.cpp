
#include <imgui.h>

#include "introwidget.hpp"
#include "constants.hpp"
#include "portable-file-dialogs.h"


IntroWidget::IntroWidget(): Widget("") {}

void IntroWidget::show(ImVec2 &size, ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    ImGui::SetCursorPosX(size.x * 0.45);
    ImGui::SetCursorPosY(size.y * 0.45);

    if(ImGui::Button(constants::LOAD_FILE_BUTTON_TITLE.c_str(), loadFileBtnSize_)) {
        auto selection = pfd::open_file("Select a file").result();
        if(!selection.empty())
            std::cout << "User selected file " << selection[0] << "\n";
    }
}
