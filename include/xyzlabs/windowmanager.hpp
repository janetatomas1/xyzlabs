
#pragma once

#include <vector>
#include <memory>

#include "xyzlabs/window.hpp"
#include "xyzlabs/event/event.hpp"
#include "xyzlabs/event/eventmanager.hpp"

namespace xyzlabs {

class XYZLabs;

/**
 * @brief Manages all application windows, their lifecycle, and per-frame updates.
 *
 * WindowManager provides:
 * - Creation, initialization, and destruction of windows.
 * - Tracking of open windows and current window index for updates.
 * - Deferred submission of new windows via the EventManager to ensure safe
 *   execution mid-frame with the correct OpenGL/ImGui context.
 *
 * Key features:
 * - Stores windows in a vector of unique_ptr<Window>.
 * - Supports querying windows by ID, title, or the currently updated window.
 * - Automatically removes closed windows during the update phase.
 * - Integrates with the main XYZLabs application and its EventManager for deferred actions.
 *
 * Threading / Execution Model:
 * - Windows are updated sequentially in the order stored in the vector.
 * - Any action that mutates a window or adds a new window is deferred via EventManager.
 * - All GLFW/ImGui/OpenGL operations occur on the main thread.
 */
class WindowManager {
    /// All managed windows
    std::vector<std::unique_ptr<Window>> windows_;

    /// Index of the window currently being updated
    size_t currentWindowIDx_ = 0;

    /**
     * @brief Removes windows that have been closed.
     *
     * Windows where is_open() returns false are erased from the vector.
     * Called automatically at the start of each update.
     */
    inline void flush_closed_windows() {
        std::erase_if(windows_, [](const auto& window) {
            return !window->is_open();
        });
    };

    /**
     * @brief Updates all windows sequentially.
     *
     * Calls Window::update() on each managed window.
     * Updates currentWindowIDx_ during iteration to track the currently processed window.
     */
    inline void update_windows() {
        for(currentWindowIDx_ = 0; currentWindowIDx_ < windows_.size(); currentWindowIDx_++) {
            windows_[currentWindowIDx_]->update();
        }
    };

    /**
     * @brief Submits a new window via a deferred action.
     *
     * The actual initialization and insertion into windows_ occurs mid-frame
     * via EventManager to ensure the correct context is current.
     *
     * @param window Unique pointer to the window to submit
     * @return Raw pointer to the window being submitted
     */
    Window* submit_window(std::unique_ptr<Window> window);

    /// Pointer to the owning application
    XYZLabs* app_;
public:
    /**
    * @brief Constructs a WindowManager bound to the given application.
    *
    * @param app Pointer to the XYZLabs application instance
    */
    WindowManager(XYZLabs* app);

    /**
    * @brief Initializes the main window.
    *
    * The window is immediately initialized, set to reference this WindowManager,
    * and added to the managed windows vector.
    *
    * @param window Unique pointer to the main window
    */
    void init_main_window(std::unique_ptr<Window> window);

    /**
    * @brief Initializes GLFW and sets core OpenGL hints.
    *
    * Logs success or failure using spdlog.
    */
    void init();

    /**
    * @brief Adds a new window.
    *
    * Supports two modes:
    * - Passing a constructor for a Window-derived type along with arguments
    * - Passing a pre-constructed std::unique_ptr<Window>
    *
    * The addition is deferred to mid-frame using EventManager.
    *
    * @tparam W Window type (defaults to Window)
    * @param args Constructor arguments or a unique_ptr<Window>
    * @return Raw pointer to the added window
    */
    template<WindowType W = Window, typename... Args>
    Window* add_window(Args... args);

    /**
    * @brief Updates all managed windows.
    *
    * Performs:
    * - Flush of closed windows
    * - Sequential update of all windows
    * - GLFW event polling
    */
    void update();

    /**
    * @brief Destroys all windows and terminates GLFW.
    */
    void destroy();

    /// Returns the number of managed windows
    size_t nwindows() const;

    /// Returns the first window (main window), or nullptr if none exist
    Window* get_main_window() const;

    /**
    * @brief Returns the window with the given unique ID.
    *
    * @param id Window ID
    * @return Pointer to the window if found, nullptr otherwise
    */
    Window* get_window_by_id(uint64_t id) const;

    /**
    * @brief Returns the window with the given title.
    *
    * @param title Window title
    * @return Pointer to the window if found, nullptr otherwise
    */
    Window* get_window_by_title(const std::string &title) const;

    /// Returns the currently updated window during update()
    Window* get_current_window();

    /// Returns the index of the currently updated window during update()
    size_t current_window_index() const;

    /// Returns a pointer to the owning application
    XYZLabs* app();
};

//
// Helper templates for type checking
//

template<typename T>
struct is_unique_ptr_to_window : std::false_type {};

template<typename T>
struct is_unique_ptr_to_window<std::unique_ptr<T>>
    : std::bool_constant<std::is_base_of_v<Window, T>> {};

template<typename T>
inline constexpr bool is_unique_ptr_to_window_v = is_unique_ptr_to_window<T>::value;

template<WindowType W, typename... Args>
Window* WindowManager::add_window(Args... args) {
    if constexpr (sizeof...(Args) == 1) {
        using First = std::tuple_element_t<0, std::tuple<Args...>>;
        if constexpr (is_unique_ptr_to_window_v<std::decay_t<First>> && std::is_same_v<W, Window>) {
            auto window = std::move(std::get<0>(std::forward_as_tuple(args...)));
            return submit_window(std::move(window));
        } else {
            return submit_window(
                std::make_unique<W>(
                    std::forward<Args>(args)...
                )
            );
        }
    } else {
        return submit_window(
            std::make_unique<W>(
                std::forward<Args>(args)...
            )
        );
    }
}

}
