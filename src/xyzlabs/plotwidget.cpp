
#include <implot.h>
#include <fmt/format.h>

#include "plotwidget.hpp"

void PlotWidget::show(const ImVec2 &size, const ImVec2 &position) {
    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(position);
    ImGui::Begin(title.c_str(), NULL, flags);
    if(ImPlot::BeginPlot(plotID.c_str(), size, flags)) {
        ImPlot::EndPlot();
    }
    ImGui::End();
}

PlotWidget::PlotWidget(std::string &title): title(title), plotID(fmt::format("##{}", title)) {
}
