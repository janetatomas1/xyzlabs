
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
    ImGuiTabItemFlags tabItemFlags_ = ImGuiTabItemFlags_SetSelected;

public:
    Widget(const std::string &title);
    virtual void show(const ImVec2 &size, const ImVec2& position);
    inline const std::string& title() const;
    inline IDType id() const;
    inline bool is_open() const;
    inline std::string& title_id();
    virtual ~Widget() = default;
    void close();
    virtual void destroy() {};
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

std::string &Widget::title_id() {
    return titleID_;
}

#endif
