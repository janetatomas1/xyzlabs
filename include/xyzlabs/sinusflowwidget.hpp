
#ifndef SINUSFLOWWIDGET_HPP
#define SINUSFLOWWIDGET_HPP

#include "widget.hpp"


class SinusFlowWidget : public Widget {
public:
    void show(ImVec2 &size, ImVec2& position) override;

    SinusFlowWidget();

    ~SinusFlowWidget() = default;
};

#endif
