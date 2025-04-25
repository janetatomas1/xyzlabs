
#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include <string>

#include "widget.hpp"


class PlotWidget : Widget {
    std::string plotID_;
public:
    void show(ImVec2 &size, ImVec2& position) override;
    PlotWidget(const std::string &title);
};

#endif
