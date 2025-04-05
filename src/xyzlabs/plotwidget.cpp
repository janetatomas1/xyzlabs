
#include <implot.h>
#include <fmt/format.h>

#include "plotwidget.hpp"

void PlotWidget::show(int width, int height) {
    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    auto size = ImVec2(width, height);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos({0, 0});
    ImGui::Begin(title.c_str(), NULL, flags);
    if(ImPlot::BeginPlot(plotID.c_str(), size, flags)) {
        ImPlot::EndPlot();
    }
    ImGui::End();
}

PlotWidget::PlotWidget(const std::string &title): title(title), plotID(fmt::format("##{}", title)) {
}
