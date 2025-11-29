
#pragma once

#include <imgui.h>
#include <string>
#include <cinttypes>

#include "xyzlabs/operators.hpp"

namespace xyzlabs {

class Widget {
    std::string title_;
    uint64_t id_ = 0;
    std::string windowID;
    int windowFlags_ = ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse;;

public:
    Widget(const std::string &title = "");
    virtual void show(const ImVec2 &size, const ImVec2& position);
    virtual void display(const ImVec2 &size, const ImVec2& position);
    const std::string& title() const;
    uint64_t id() const;
    virtual ~Widget() = default;
    virtual void destroy() {};
    virtual void init() {};
    int get_flags();
    void set_flags(int flags);
};

template <typename T>
concept WidgetType = std::derived_from<T, Widget> || std::same_as<T, Widget>;

}
