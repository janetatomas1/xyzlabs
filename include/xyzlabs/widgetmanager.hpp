
#ifndef WIDGETMANAGER_HPP
#define WIDGETMANAGER_HPP

#include <vector>
#include <memory>

#include <imgui.h>

#include "widget.hpp"

constexpr size_t DEFAULT_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

class WidgetManager {
    std::vector<std::unique_ptr<Widget>> newWidgets_;
    std::vector<std::unique_ptr<Widget>> tabs_;

    ImVec2 innerSize_ = {0.0, 0.0};
    ImVec2 innerPos_ = {0.0, 0.0};
    ImVec2 toolBarSize_ = {0.0, 0.0};
    ImVec2 toolBarPos_ = {0.0, 0.0};

    const float TOOLBAR_RELATIVE_HEIGHT = 0.08;

public:
    WidgetManager() = default;
    void show(ImVec2 &size, ImVec2 &pos);
    void add_widget(std::unique_ptr<Widget> widget);
};

#endif
