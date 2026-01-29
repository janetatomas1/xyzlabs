
#pragma once

#include <imgui.h>
#include <string>
#include <cinttypes>
#include <memory>

#include "xyzlabs/ui/relativelayout.hpp"
#include "xyzlabs/utils/operators.hpp"


namespace xyzlabs {

class Window;
class XYZLabs;
class TaskManager;
class EventManager;
class ObjectRegistry;
class SettingsManager;

/**
 * @brief Base class for all UI widgets.
 *
 * Widget represents a drawable UI element with:
 *  - A unique ID and ImGui window identifier
 *  - Optional parent/child hierarchy
 *  - Optional attachment to a Window
 *  - Relative layout support
 *  - Access to core application services
 *
 * Widgets are typically subclassed and override show() to render content.
 * The display() method handles ImGui window creation and layout.
 */
class Widget {
protected:
    /**
     * @brief Access the global task manager.
     *
     * Convenience accessor routed through the owning application.
     */
    TaskManager& task_manager();

    /**
     * @brief Access the global event manager.
     *
     * Allows widgets to publish or subscribe to events.
     */
    EventManager& event_manager();

    /**
     * @brief Access the object registry.
     *
     * Used for global object lookup and lifetime management.
     */
    ObjectRegistry& object_registry();

    /**
     * @brief Access the settings manager.
     *
     * Provides access to persistent application settings.
     */
    SettingsManager& settings_manager();

public:
    /**
     * @brief Construct a widget.
     *
     * A unique ID is automatically generated and used to build
     * a stable ImGui window identifier.
     *
     * @param title  Display title of the widget
     * @param parent Optional parent widget
     * @param window Optional owning window
     */
    Widget(
        const std::string& title = "",
        Widget* parent = nullptr,
        Window* window = nullptr
    );

    /**
     * @brief Render the widget contents.
     *
     * This method should be overridden by derived classes to
     * draw ImGui elements. It is called from display() after
     * layout and window setup are complete.
     *
     * @param size     Computed widget size
     * @param position Computed widget position
     */
    virtual void show(const ImVec2& size, const ImVec2& position);

    /**
     * @brief Display the widget window.
     *
     * Handles ImGui::Begin/End, applies layout computation,
     * sets window position and size, and then calls show().
     * Should be only called for top-level widgets.
     * Should be overridden if custom behavior is required.
     *
     * @param size     Available parent size
     * @param position Available parent position
     */
    virtual void display(const ImVec2& size, const ImVec2& position);

    /**
     * @brief Get the widget title.
     */
    const std::string& title() const;

    /**
     * @brief Get the ImGui window identifier.
     *
     * The identifier includes a unique suffix to avoid collisions.
     */
    const std::string& window_id() const;

    /**
     * @brief Get the unique widget ID.
     */
    uint64_t id() const;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Widget() = default;

    /**
     * @brief Called when the widget is destroyed.
     *
     * Can be overridden to release resources.
     */
    virtual void destroy() {}

    /**
     * @brief Initialize the widget.
     *
     * Called after construction but before display.
     */
    virtual void init() {}

    /**
     * @brief Get ImGui window flags.
     */
    int get_flags();

    /**
     * @brief Set ImGui window flags.
     *
     * @param flags ImGuiWindowFlags mask
     */
    void set_flags(int flags);

    /**
     * @brief Access the widget's relative layout.
     */
    RelativeLayout& layout();

    /**
     * @brief Get the parent widget.
     */
    Widget* parent();

    /**
     * @brief Get the parent widget cast to a specific type.
     *
     * @tparam W Target widget type
     * @return Pointer to parent casted to W, or nullptr if incompatible
     */
    template<class W>
    W* parent_as();

    /**
     * @brief Set the parent widget.
     */
    void set_parent(Widget* parent);

    /**
     * @brief Get the owning window.
     *
     * If no window is directly assigned, the parent hierarchy
     * is searched recursively.
     */
    Window* window();

    /**
     * @brief Set the owning window.
     */
    void set_window(Window* window);

    /**
     * @brief Get the owning application instance.
     *
     * Resolved via the window or parent hierarchy.
     */
    XYZLabs* app();
};


/**
 * @brief Type trait to detect std::unique_ptr<T> where T derives from Widget.
 */
template<typename T>
struct is_unique_ptr_to_widget;

/**
 * @brief Convenience boolean for is_unique_ptr_to_widget.
 */
template<typename T>
inline constexpr bool is_unique_ptr_to_widget_v;

/**
 * @brief Concept matching Widget or any class derived from Widget.
 */
template <typename T>
concept WidgetType = std::derived_from<T, Widget> || std::same_as<T, Widget>;


}
