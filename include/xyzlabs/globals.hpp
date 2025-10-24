
#ifndef GLOBALS_HPP
#define GLOBALS_HPP

class XYZLabs;
class RandomGenerator;
class WindowManager;
class EventManager;
class TaskManager;
class SettingsManager;

XYZLabs &app();
RandomGenerator &random_generator();
WindowManager &window_manager();
EventManager &event_manager();
TaskManager &task_manager();
SettingsManager &settings_manager();

#endif
