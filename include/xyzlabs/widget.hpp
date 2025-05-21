
#ifndef WIDGET_H
#define WIDGET_H

#include <imgui.h>
#include <string>
#include <memory>

#include "xyzlabs/idgenerator.hpp"

class Widget {
    std::string title_;
    IDType id_;
    std::string titleID_;

    bool open_ = true;
    bool *openPtr_;
    ImGuiTabItemFlags tabItemFlags_ = ImGuiTabItemFlags_SetSelected;

public:
    Widget(const std::string &title);
    virtual void show(ImVec2 &size, ImVec2& position);
    inline const std::string& title() const;
    inline IDType id() const;
    void disable_closing();
    inline bool is_open() const;
    inline bool *is_open_ptr();
    inline std::string& title_id();
    virtual ~Widget() = default;
    inline void deselect();
    inline ImGuiTabItemFlags tab_item_flags() const;
};

const std::string &Widget::title() const {
    return title_;
}

IDType Widget::id() const {
    return id_;
}

bool Widget::is_open() const {
    return open_;
}

bool *Widget::is_open_ptr() {
    return openPtr_;
}

std::string &Widget::title_id() {
    return titleID_;
}

void Widget::deselect() {
    tabItemFlags_ = ImGuiTabItemFlags_None;
}

ImGuiTabItemFlags Widget::tab_item_flags() const {
    return tabItemFlags_;
}

#endif
