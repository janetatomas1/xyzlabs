
#ifndef INTROWIDGET_HPP
#define INTROWIDGET_HPP

#include "xyzlabs/widget.hpp"

class IntroWidget: public Widget {
    ImVec2 loadFileBtnSize_ = {150.0, 30.0};
public:
    void show(const ImVec2 &size, const ImVec2& position) override;
    IntroWidget();
};

#endif
