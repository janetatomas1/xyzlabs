
#include <implot.h>
#include <fmt/format.h>

#include "plotwidget.hpp"

void PlotWidget::show(ImVec2 &size, ImVec2& position) {
    // TODO: create separate flags for the plot
    if(ImPlot::BeginPlot(plotID_.c_str(), size, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImPlot::EndPlot();
    }
}

PlotWidget::PlotWidget(const std::string &title): 
    Widget(title),
    plotID_(fmt::format("##{}", title)) {
}
