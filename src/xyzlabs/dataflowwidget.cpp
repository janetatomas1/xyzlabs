
#include <implot.h>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <vector>

#include "dataflowwidget.hpp"

using namespace boost::interprocess;

constexpr int MAX_POINTS = 500;

struct SharedData {
    float value;
};

void DataFlowWidget::show(ImVec2 &size, ImVec2& position) {
    // live data simulation - from other process
    static std::vector<float> x_data(MAX_POINTS);
    static std::vector<float> y_data(MAX_POINTS);
    static int index = 0;
    static float t = 0.0f;
    static float dt = 0.01f;

    static shared_memory_object shm;
    static mapped_region region;
    static SharedData* shared_data_ptr;

    //  expecting float value in correstponding shared memory
    try {
        shm = shared_memory_object(open_only, "FlowWidgetSharedData", read_only); // todo make shared memory scalable
        region = mapped_region(shm, read_only);
        shared_data_ptr = static_cast<SharedData*>(region.get_address());
    } catch (const interprocess_exception& ex) {
        shared_data_ptr = nullptr;
    }

    // time handling todo
    t += dt;
    x_data[index] = t;

    // shared memory load
    y_data[index] = shared_data_ptr ? shared_data_ptr->value : 0;

    index = (index + 1) % MAX_POINTS;

    std::vector<float> plot_x, plot_y;
    for (int i = 0; i < MAX_POINTS; ++i) {
        int real_index = (index + i) % MAX_POINTS;
        plot_x.push_back(x_data[real_index]);
        plot_y.push_back(y_data[real_index]);
    }

    if(ImPlot::BeginPlot(id().c_str(), size, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImPlot::SetupAxes("Time", "Shared data", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
    
        ImPlot::PlotLine("Shared data", plot_x.data(), plot_y.data(), MAX_POINTS);
        ImPlot::EndPlot();
    }
}

DataFlowWidget::DataFlowWidget() :
    Widget("Data flow plot") {}
