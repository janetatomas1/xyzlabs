
#ifndef CONTINIUMWAVE_HPP
#define CONTINIUMWAVE_HPP

#include <imgui.h>
#include <memory>

#include "plotwidget.hpp"

class GLFWwindow;

class ContiniumWave {
    GLFWwindow *window;

    const char* glsl_version = "#version 300 es";
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    PlotWidget plot = PlotWidget("abcd");

    int width, height;
    
    void init();
    int mainloop();
    void exit();
public:
    int exec();
};

#endif
