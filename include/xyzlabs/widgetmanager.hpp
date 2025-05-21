
#ifndef WIDGETMANAGER_HPP
#define WIDGETMANAGER_HPP

#include <vector>
#include <memory>

#include <imgui.h>

#include "widget.hpp"

constexpr size_t DEFAULT_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

template <typename T>
concept WidgetConcept = std::derived_from<Widget, Widget>;

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
    inline void flush_new_widgets();
    inline void remove_closed_tabs();

    template<WidgetConcept W, typename... Args>
    IDType add_widget(Args... args) {
        auto widget = std::make_unique<W>(std::forward<Args>(args)...);
        const IDType id = widget->id();
        newWidgets_.push_back(std::move(widget));
        return id;
    };
    bool disable_widget_closing(IDType id);
};

void WidgetManager::flush_new_widgets() {
    if(!newWidgets_.empty()) {
        for(auto &w: newWidgets_) {
            tabs_.push_back(std::move(w));
        }
        newWidgets_.clear();
    }
}

void WidgetManager::remove_closed_tabs() {
    tabs_.erase(
        std::remove_if(tabs_.begin(), tabs_.end(),
            [](auto &t) {
                return !t->is_open();
            }
        ), tabs_.end()
    );
}

#endif
