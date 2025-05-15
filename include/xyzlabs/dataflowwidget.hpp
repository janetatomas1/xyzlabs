
#ifndef DATAFLOWWIDGET_HPP
#define DATAFLOWWIDGET_HPP

#include "widget.hpp"


class DataFlowWidget : public Widget {
public:
    void show(ImVec2 &size, ImVec2& position) override;

    DataFlowWidget();

    ~DataFlowWidget() = default;
};

#endif
