
#include <implot.h>

#include <cmath>
#include <vector>

#include "sinusflowwidget.hpp"

constexpr int MAX_POINTS = 500;

void SinusFlowWidget::show(ImVec2 &size, ImVec2& position) {
    // live data simulation - sinus function
    static std::vector<float> x_data(MAX_POINTS);
    static std::vector<float> y_data(MAX_POINTS);
    static int index = 0;
    static float t = 0.0f;
    static float dt = 0.03f;

    t += dt;

    x_data[index] = t;
    y_data[index] = std::sin(t);

    index = (index + 1) % MAX_POINTS;

    std::vector<float> plot_x, plot_y;
    for (int i = 0; i < MAX_POINTS; ++i) {
        int real_index = (index + i) % MAX_POINTS;
        plot_x.push_back(x_data[real_index]);
        plot_y.push_back(y_data[real_index]);
    }

    if(ImPlot::BeginPlot(id().c_str(), size, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImPlot::SetupAxes("Time", "sin(t)");
        ImPlot::SetupAxisLimits(ImAxis_Y1, -1.1, 1.1, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - 10.0, t, ImGuiCond_Always);
    
        ImPlot::PlotLine("sin(t)", plot_x.data(), plot_y.data(), MAX_POINTS);
        ImPlot::EndPlot();
    }
}

SinusFlowWidget::SinusFlowWidget() :
    Widget("Sinus function flow plot") {}
