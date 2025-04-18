
#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <imgui.h>

#include "widgetmanager.hpp"


class Widget {
public:
    virtual void show(const ImVec2 &size, const ImVec2 &position) = 0;
    Widget() = default;
};

#endif
