
#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include <string>


class PlotWidget {
    std::string title;
    std::string plotID;
    
public:
    void show(int width, int height);
    PlotWidget(const std::string &title);
};

#endif
