#pragma once

#include <concepts>
#include <filesystem>

#include <imgui.h>

#include "xyzlabs/utils/randomgenerator.hpp"
#include "xyzlabs/task/taskmanager.hpp"
#include "xyzlabs/settings/settingsmanager.hpp"
#include "xyzlabs/event/eventmanager.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/objectregistry.hpp"

namespace xyzlabs {

/**
 * @brief Core application class and runtime coordinator.
 *
 * @details
 * XYZLabs represents the main application instance and owns all
 * high-level subsystems:
 *
 * - Window and rendering management
 * - Event dispatching and deferred execution
 * - Background task execution
 * - Settings persistence
 * - Object lifetime and identification
 *
 * The class provides a minimal but explicit application lifecycle:
 *
 * 1. init()   — initialize subsystems and create the main window
 * 2. exec()   — enter the main loop
 * 3. close()  — request shutdown
 *
 * XYZLabs enforces **single ownership** of global systems and acts as
 * the central access point for managers used throughout the engine.
 *
 * @par Architecture Notes
 * - All UI-related operations are executed on the main thread.
 * - Deferred work is scheduled through EventManager.
 *
 * @par Thread Safety
 * XYZLabs itself is not thread-safe. Thread-safe interaction must go
 * through TaskManager or EventManager.
 */
class XYZLabs {
    /// Target delay between frames in milliseconds
    size_t renderTimeout_ = 30;

    /// Application title
    std::string title_;

    /// Background task execution manager
    TaskManager taskManager_;

    /// Persistent settings manager
    SettingsManager settingsManager_;

    /// Event and deferred action dispatcher
    EventManager eventManager_;

    /// Window lifecycle manager
    WindowManager windowManager_;

    /// Registry for uniquely identifiable objects
    ObjectRegistry objectRegistry_;

    /// Internal initialization routine
    void init_();

    /// Internal shutdown routine
    void exit_();

    /// Internal main loop
    void mainloop_();

    /// Process exit code
    int exitCode_ = 0;

    /// Current frame counter
    uint64_t frame_ = 0;
public:
    /**
     * @brief Constructs the application instance.
     *
     * Initializes core managers but does not create windows or start
     * the main loop.
     */
    XYZLabs();

    /// Virtual destructor for safe inheritance
    virtual ~XYZLabs() = default;

    /**
     * @brief Sets the initial widget for the main window.
     *
     * @tparam W Widget type
     * @param args Arguments forwarded to widget constructor
     *
     * @note Must be called after a main window has been created.
     */
    template<WidgetType W = Widget, typename... Args>
    void set_initial_widget(Args... args);

    /**
     * @brief Initializes the application with a default Window.
     *
     * @tparam W Initial central widget type
     * @param title Application title
     * @param args Arguments forwarded to widget constructor
     */
    template<WidgetType W = Widget, typename... Args>
    void init(const std::string &title, Args... args);

    /**
     * @brief Initializes the application with a custom Window type.
     *
     * @tparam Win Window type
     * @tparam W Initial central widget type
     * @param title Application title
     * @param args Arguments forwarded to widget constructor
     */
    template<WindowType Win, WidgetType W = Widget, typename... Args>
    void init(const std::string &title, Args... args);

    /// Access the task manager
    TaskManager &task_manager();

    /// Access the settings manager
    SettingsManager &settings_manager();

    /// Access the event manager
    EventManager &event_manager();

    /// Access the window manager
    WindowManager &window_manager();

    /// Access the object registry
    ObjectRegistry &object_registry();

    /**
     * @brief Returns the global random generator.
     *
     * @details
     * Provides a shared RNG for ID generation and non-deterministic
     * behavior across the engine.
     */
    static RandomGenerator& random_generator();

    /**
     * @brief Requests application shutdown.
     *
     * @note This does not immediately terminate execution.
     * Shutdown occurs when all windows are closed.
     */
    void close();

    /**
     * @brief Starts the main loop and blocks until exit.
     *
     * @return Process exit code
     */
    int exec();

    /// Returns the application title
    const std::string& title();

    /// Sets the application title
    void set_title(const std::string& title);

    /// Returns the process exit code
    int return_code();

    /// Returns the current frame number
    uint64_t frame();

    /// Returns the render timeout in milliseconds
    size_t timeout();

    /// Sets the render timeout in milliseconds
    void set_timeout(size_t timeout);
};


template<WidgetType W, typename... Args>
void XYZLabs::set_initial_widget(Args... args) {
    windowManager_
    .get_main_window()
    ->set_central_widget<W>(
        std::forward<Args>(args)...
    );
};

template<WidgetType W, typename... Args>
void XYZLabs::init(const std::string &title, Args... args) {
    windowManager_.init_main_window(std::make_unique<Window>(title));
    set_initial_widget<W>(std::forward<Args>(args)...);
}

template<WindowType Win, WidgetType W, typename... Args>
void XYZLabs::init(const std::string &title, Args... args) {
    title_ = title;
    windowManager_.init_main_window(std::make_unique<Win>(title));
    set_initial_widget<W>(std::forward<Args>(args)...);
}

}
