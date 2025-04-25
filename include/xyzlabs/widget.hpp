
#ifndef WIDGET_H
#define WIDGET_H

#include <imgui.h>
#include <string>
#include <memory>

class Widget {
    std::string title_;
    std::string id_;

public:
    Widget(const std::string &title);
    virtual void show(ImVec2 &size, ImVec2& position);
    inline const std::string& title() const;
    inline const std::string &id() const;
};

const std::string& Widget::title() const {
    return title_;
}

const std::string &Widget::id() const {
    return id_;
}

#endif
