
#ifndef WIDGETMANAGER_HPP
#define WIDGETMANAGER_HPP

#include <vector>
#include <memory>
#include <ranges>

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "xyzlabs/widget.hpp"


template <typename T>
concept WidgetConcept = std::derived_from<Widget, Widget>;

class WidgetManager {
    std::vector<std::unique_ptr<Widget>> newWidgets_;
    std::vector<std::unique_ptr<Widget>> widgets_;
    bool toolbarOpen_ = true;
    int currentWidget_ = 0;
    float toolbarClosedRatio_ = 0.05f;
    float toolbarOpenRatio_ = 0.15;

public:
    WidgetManager() = default;
    void show(const ImVec2 &size);
    inline void flush_new_widgets();
    inline void remove_closed_widgets();

    template<WidgetConcept W, typename... Args>
    uint64_t add_widget(Args... args);
    template<WidgetConcept W>
    uint64_t add_intro_widget();
    inline void display_radio_buttons();
    void show_toolbar(const ImVec2 &size);
    inline float toolbar_window_ratio() {
        return toolbarOpen_ ? toolbarOpenRatio_ : toolbarClosedRatio_;
    }
    void close_current_widget();
};

void WidgetManager::flush_new_widgets() {
    if(!newWidgets_.empty()) {
        for(auto &w: newWidgets_) {
            widgets_.push_back(std::move(w));
        }
        toolbarOpen_ = true;
        newWidgets_.clear();
        currentWidget_ = widgets_.size() - 1;
    }
}

template<WidgetConcept W, typename... Args>
uint64_t WidgetManager::add_widget(Args... args) {
    auto widget = std::make_unique<W>(std::forward<Args>(args)...);
    const uint64_t id = widget->id();
    spdlog::info("Added new widget. Title: {}, id: {}", widget->title(), id);
    newWidgets_.push_back(std::move(widget));
    return id;
}

template<WidgetConcept W>
uint64_t WidgetManager::add_intro_widget() {
    auto widget = std::make_unique<W>();
    const uint64_t id = widget->id();
    spdlog::info("Added new intro widget. Title: {}, id: {}", widget->title(), id);
    
    if(widgets_.empty()) {
        widgets_.push_back(std::move(widget));
    } else {
        widgets_[0] = std::move(widget);
    }

    return id;
}

void WidgetManager::remove_closed_widgets() {
    std::erase_if(widgets_, [](const auto &w){
        return !w || !(w->is_open());
    });

    if(currentWidget_ >= widgets_.size()) {
        currentWidget_ = widgets_.size() - 1;
    }
}

void WidgetManager::display_radio_buttons() {
    ImGui::Text("Open widgets");
    ImGui::Dummy(ImVec2(0, 10));

    for(int i=0;i < widgets_.size();i++) {
        ImGui::RadioButton(widgets_[i]->title_id().c_str(), &currentWidget_, i);
    }
}

#endif
