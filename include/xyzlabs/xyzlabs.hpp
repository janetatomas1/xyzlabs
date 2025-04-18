
#ifndef XYZLABS_HPP
#define XYZLABS_HPP

#include <imgui.h>
#include <memory>

#include "widgetmanager.hpp"

class GLFWwindow;

class XYZLabs {
    GLFWwindow *window;

    const char* glsl_version = "#version 300 es";
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    int width, height;
    WidgetManager wManager;

    void init();
    int mainloop();
    void exit();

    XYZLabs() = default;
    XYZLabs(const XYZLabs&) = delete;
    XYZLabs& operator=(const XYZLabs&) = delete;
public:
    static XYZLabs& instance();
    int exec();
};

#endif
