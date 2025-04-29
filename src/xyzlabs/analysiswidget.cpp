
#include <implot.h>
#include <fmt/format.h>

#include <format>
#include <filesystem>

#include "analysiswidget.hpp"

void AnalysisWidget::show(ImVec2 &size, ImVec2& position) {
    // TODO: create separate flags for the plot
    if(ImPlot::BeginPlot(id().c_str(), size, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImPlot::EndPlot();
    }
}

AnalysisWidget::AnalysisWidget(const std::string &filepath):
    Widget(std::format("Analyze {}", std::filesystem::path(filepath).filename().string())) {}
