
#pragma once

namespace xyzlabs {

class XYZLabs;
class RandomGenerator;
class WindowManager;
class EventManager;
class TaskManager;
class SettingsManager;
class ObjectRegistry;

XYZLabs &app();
WindowManager &window_manager();
EventManager &event_manager();
TaskManager &task_manager();
SettingsManager &settings_manager();
ObjectRegistry &object_registry();

}
