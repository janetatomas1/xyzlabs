
#include <imgui.h>
#include <portable-file-dialogs.h>

#include "introwidget.hpp"
#include "constants.hpp"
#include "xyzlabs.hpp"
#include "analysiswidget.hpp"
#include "dataflowwidget.hpp"
#include "sinusflowwidget.hpp"


IntroWidget::IntroWidget(): Widget("Welcome !!!") {}

void IntroWidget::show(ImVec2 &size, ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    // first button - ANALYZE FILE
    ImGui::SetCursorPosX(size.x * 0.45);
    ImGui::SetCursorPosY(size.y * 0.45);

    if(ImGui::Button(constants::LOAD_FILE_BUTTON_TITLE.c_str(), basicBtnSize_)) {
        auto selection = pfd::open_file("Select a file").result();
        if(!selection.empty()) {
            std::unique_ptr<Widget> widget = std::make_unique<AnalysisWidget>(selection[0]);
            XYZLabs::instance().widget_manager().add_widget(std::move(widget));
        }
    }

    // second button - DATA FLOW
    ImGui::SetCursorPosX(size.x * 0.45);    // todo coords
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    if (ImGui::Button(constants::DATA_FLOW_BUTTON_TITLE.c_str(), basicBtnSize_)) { // todo custom btn size
        std::unique_ptr<Widget> flowWidget = std::make_unique<DataFlowWidget>();
        XYZLabs::instance().widget_manager().add_widget(std::move(flowWidget));
    }

    // third button - SINUS FLOW
    ImGui::SetCursorPosX(size.x * 0.45);    // todo coords
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

    if (ImGui::Button(constants::SINUS_FLOW_BUTTON_TITLE.c_str(), basicBtnSize_)) { // todo custom btn size
        std::unique_ptr<Widget> flowWidget = std::make_unique<SinusFlowWidget>();
        XYZLabs::instance().widget_manager().add_widget(std::move(flowWidget));
    }
}
