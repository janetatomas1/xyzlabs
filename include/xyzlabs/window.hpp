
#pragma once

#include <concepts>
#include <vector>

#include "xyzlabs/ui/widget.hpp"

class ImGuiContext;
struct GLFWwindow;

using WindowHandle = GLFWwindow*;

namespace xyzlabs {

class WindowManager;

/**
 * @brief Represents a single application window with an ImGui context and a central widget.
 *
 * Window encapsulates:
 * - A native GLFW window and OpenGL context
 * - An ImGui context bound to that window
 * - A single central Widget responsible for rendering UI content
 *
 * The Window lifecycle is managed by WindowManager and the application event system.
 * Most mutating operations (closing, resizing, widget replacement, style changes)
 * should be deferred via EventManager actions and executed mid-frame on the owning thread.
 *
 * Key properties:
 * - Each Window owns its own ImGui context.
 * - Only one central widget may exist at a time.
 * - All OpenGL and ImGui calls are executed with the correct context made current.
 * - Window operations are safe to call from non-render code paths via deferred actions.
 *
 * Threading model:
 * - Window methods may enqueue actions through EventManager.
 * - Actual OpenGL / ImGui state mutation occurs during the application's dispatch phase.
 *
 * Lifetime:
 * - A Window is considered open while is_open() returns true.
 * - Calling close() schedules the window for closure.
 * - Destruction cleans up ImGui, OpenGL, and GLFW resources.
 */
class Window {
    /// Indicates whether the window is currently open
    bool open_ = true;

    /// Native GLFW window handle
    WindowHandle handle_ = nullptr;

    /// ImGui context associated with this window
    ImGuiContext *ctx_ = nullptr;

    /// Window title displayed by the system
    std::string title_;

    /// Unique window identifier
    uint64_t id_ = 0;

    /// Current window width in pixels
    int32_t width_ = 0;

    /// Current window height in pixels
    int32_t height_ = 0;

    /// Central UI widget rendered inside the window
    std::unique_ptr<Widget> centralWidget_;

    /// Owning WindowManager
    WindowManager *windowManager_ = nullptr;

    /**
    * @brief Submits a new central widget via a deferred action.
    *
    * The current widget (if any) is destroyed and replaced during dispatch.
    * The returned pointer refers to the widget instance that will become active.
    */
    Widget* submit_widget(std::unique_ptr<Widget> widget);
public:
    /**
     * @brief Makes this window's OpenGL and ImGui contexts current.
     *
     * Must be called before any OpenGL or ImGui operations targeting this window.
     */
    void make_context_current();

    /**
     * @brief Initializes the window, OpenGL context, and ImGui backend.
     *
     * Creates the GLFW window, initializes GLEW, sets up ImGui,
     * and registers necessary GLFW callbacks.
     *
     * Called once by the WindowManager during window creation.
     * When overriding, make sure to call the base class implementation.
     */
    virtual void init();

    /// Returns the current window width in pixels
    int32_t width();

    /// Returns the current window height in pixels
    int32_t height();

    /**
     * @brief Schedules the window to be closed.
     *
     * The actual close occurs via a deferred action processed mid-frame.
     */
    void close();

    /**
     * @brief Constructs a window with the given title and size.
     *
     * If width or height is zero, the window will be maximized.
     *
     * @param title Window title
     * @param width Initial width (pixels)
     * @param height Initial height (pixels)
     */
    Window(
        const std::string &title = "",
        int32_t width = 0,
        int32_t height = 0
    );

    /**
     * @brief Destroys the window and all associated resources.
     *
     * Cleans up:
     * - Central widget
     * - ImGui context
     * - OpenGL backend
     * - GLFW window
     */
    virtual ~Window();

    /**
     * @brief Updates the window for a single frame.
     *
     * Performs:
     * - Context activation
     * - ImGui frame creation
     * - Central widget rendering
     * - Buffer swap and input processing
     *
     * Called once per frame by WindowManager.
     */
    void update();

    /// Returns whether the window is currently open
    bool is_open() const;

    /// Returns the native GLFW window handle
    WindowHandle handle();

    /**
     * @brief Handles per-frame keyboard input.
     *
     * Default behavior closes the window when Escape is pressed.
     * May be overridden by derived classes.
     */
    virtual void key_callback();

    /// Returns the window title
    const std::string& title() const {
        return title_;
    };

    /// Returns the unique window identifier
    uint64_t id() const {
        return id_;
    }

    /**
     * @brief Sets the central widget of the window.
     *
     * The widget replacement is deferred and executed mid-frame.
     * The previous widget (if any) is destroyed.
     *
     * Supports:
     * - Passing constructor arguments
     * - Passing an existing std::unique_ptr<Widget>
     *
     * @tparam W Widget type (must derive from Widget)
     * @return Pointer to the widget that will become active
     */
    template<
        WidgetType W = Widget,
        typename... Args
    >
    Widget* set_central_widget(Args... args);

    /**
     * @brief Sets the background color of the window.
     *
     * Color change is deferred and applied mid-frame.
     *
     * @param color RGBA color in normalized float range [0,1]
     */
    void set_color(const std::array<float, 4> &color);

    /**
     * @brief Exports a region of the window framebuffer as RGBA pixels.
     *
     * Reads pixels from the OpenGL front buffer and vertically flips the image
     * to match conventional image coordinate systems.
     *
     * @param x X offset in pixels
     * @param y Y offset in pixels
     * @param width Width of the region
     * @param height Height of the region
     *
     * @return RGBA pixel data (row-major)
     */
    std::vector<unsigned char> export_img(
        int x, int y,
        int width, int height
    );

    /// Returns a reference to the ImGui style for this window
    ImGuiStyle& style();

    /// Returns the owning WindowManager
    WindowManager *window_manager();

    /// Returns the central widget of the window
    template <WidgetType W = Widget>
    W* central_widget() {
        return dynamic_cast<W*>(centralWidget_.get());
    }

    /// Sets the owning WindowManager
    void set_window_manager(WindowManager *windowManager);

    /**
     * @brief Schedules a window resize.
     *
     * Resize is executed via a deferred action and applied safely
     * during the dispatch phase.
     *
     * @param width New width in pixels
     * @param height New height in pixels
     */
    void set_size(int width, int height);
};


template<
    WidgetType W,
    typename... Args
>
Widget* Window::set_central_widget(Args... args) {
    if constexpr (sizeof...(Args) == 1) {
        using First = std::tuple_element_t<0, std::tuple<Args...>>;
        if constexpr (is_unique_ptr_to_widget_v<std::decay_t<First>> && std::is_same_v<W, Widget>) {
            auto widget = std::move(std::get<0>(std::forward_as_tuple(args...)));
            return submit_widget(std::move(widget));
        } else {
            return submit_widget(
                std::make_unique<W>(
                    std::forward<Args>(args)...
                )
            );
        }
    } else {
        return submit_widget(
            std::make_unique<W>(
                std::forward<Args>(args)...
            )
        );
    }
}

template <typename T>
concept WindowType = std::derived_from<T, Window> || std::same_as<T, Window>;

}
