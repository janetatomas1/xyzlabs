
#pragma once

#include <imgui.h>
#include <string>
#include <memory>

namespace xyzlabs {

class Widget {
    std::string title_;
    uint64_t id_ = 0;
    std::string titleID_;

    bool open_ = true;
    ImGuiTabItemFlags tabItemFlags_ = ImGuiTabItemFlags_SetSelected;

public:
    Widget() = default;
    Widget(const std::string &title);
    virtual void show(const ImVec2 &size, const ImVec2& position);
    inline const std::string& title() const;
    inline uint64_t id() const;
    inline bool is_open() const;
    inline std::string& title_id();
    virtual ~Widget() = default;
    void close();
    virtual void destroy() {};
    virtual void init() {};
};

const std::string &Widget::title() const {
    return title_;
}

uint64_t Widget::id() const {
    return id_;
}

bool Widget::is_open() const {
    return open_;
}

std::string &Widget::title_id() {
    return titleID_;
}

template <typename T>
concept WidgetType = std::derived_from<T, Widget> || std::same_as<T, Widget>;

ImVec2 operator*(const ImVec2 &first, const ImVec2 &second);
ImVec2 operator*(const ImVec2 &first, float x );
ImVec2 operator*(float x, const ImVec2 &first);
ImVec2 operator+(const ImVec2 &first, const ImVec2 &second);
ImVec2 operator-(const ImVec2 &first, const ImVec2 &second);

}
