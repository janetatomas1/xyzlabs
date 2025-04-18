
#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include <string>

#include "widget.hpp"

class PlotWidget : public Widget {
    std::string title;
    std::string plotID;
    
public:
    void show(const ImVec2 &size, const ImVec2 &position) override;
    PlotWidget(std::string &title);
};

#endif
