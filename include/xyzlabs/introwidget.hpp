
#ifndef INTROWIDGET_HPP
#define INTROWIDGET_HPP

#include "xyzlabs/widget.hpp"

class IntroWidget: public Widget {
public:
    void show(const ImVec2 &size, const ImVec2& position) override;
    IntroWidget();
};

#endif
