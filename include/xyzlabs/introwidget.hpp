
#ifndef INTROWIDGET_HPP
#define INTROWIDGET_HPP

#include "widget.hpp"

class IntroWidget: public Widget {
    ImVec2 basicBtnSize_ = {150.0, 30.0};
public:
    void show(ImVec2 &size, ImVec2& position) override;
    IntroWidget();
};

#endif
