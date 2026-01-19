
#pragma once

#include <imgui.h>
#include <string>
#include <cinttypes>
#include <memory>

#include "xyzlabs/eventmanager.hpp"
#include "xyzlabs/ui/relativelayout.hpp"
#include "xyzlabs/operators.hpp"


namespace xyzlabs {

class Window;
class XYZLabs;
class TaskManager;
class EventManager;
class ObjectRegistry;
class SettingsManager;

class Widget {
    std::string title_;
    uint64_t id_ = 0;
    std::string windowID_;
    int windowFlags_ = ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse;
    RelativeLayout layout_;

    Widget *parent_;
    Window *window_;
protected:
    TaskManager& task_manager();
    EventManager& event_manager();
    ObjectRegistry& object_registry();
    SettingsManager& settings_manager();

public:
    Widget(
        const std::string &title = "",
        Widget *parent = nullptr,
        Window *window = nullptr
    );
    virtual void show(const ImVec2 &size, const ImVec2& position);
    virtual void display(const ImVec2 &size, const ImVec2& position);
    const std::string& title() const;
    const std::string& window_id() const;
    uint64_t id() const;
    virtual ~Widget() = default;
    virtual void destroy() {};
    virtual void init() {};
    int get_flags();
    void set_flags(int flags);
    RelativeLayout &layout();
    Widget* parent();
    template<class W>
    W* parent_as();
    void set_parent(Widget *parent);
    Window* window();
    void set_window(Window *window);
    XYZLabs* app();
};

template<typename T>
struct is_unique_ptr_to_widget : std::false_type {};

template<typename T>
struct is_unique_ptr_to_widget<std::unique_ptr<T>>
    : std::bool_constant<std::is_base_of_v<Widget, T>> {};

template<typename T>
inline constexpr bool is_unique_ptr_to_widget_v = is_unique_ptr_to_widget<T>::value;

template <typename T>
concept WidgetType = std::derived_from<T, Widget> || std::same_as<T, Widget>;

template<class W>
W* Widget::parent_as() {
    return dynamic_cast<W*>(parent());
}

}
