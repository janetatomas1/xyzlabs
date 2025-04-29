
#ifndef ANALYSISWIDGET_HPP
#define ANALYSISWIDGET_HPP

#include <string>

#include "widget.hpp"


class AnalysisWidget : public Widget {
public:
    void show(ImVec2 &size, ImVec2& position) override;
    AnalysisWidget(const std::string &filepath);
    ~AnalysisWidget() = default;
};

#endif
