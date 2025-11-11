
#pragma once

#include <imgui.h>
#include <string>
#include <memory>

#include "xyzlabs/operators.hpp"

namespace xyzlabs {

class Widget {
    std::string title_;
    uint64_t id_ = 0;
    std::string titleID_;

    ImGuiTabItemFlags tabItemFlags_ = ImGuiTabItemFlags_SetSelected;
public:
    Widget() = default;
    Widget(const std::string &title);
    virtual void show(const ImVec2 &size, const ImVec2& position);
    inline const std::string& title() const;
    inline uint64_t id() const;
    inline std::string& title_id();
    virtual ~Widget() = default;
    virtual void destroy() {};
    virtual void init() {};
};

template <typename T>
concept WidgetType = std::derived_from<T, Widget> || std::same_as<T, Widget>;

}
