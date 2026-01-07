
#pragma once

namespace xyzlabs {

class XYZLabs;
class EventManager;
class TaskManager;
class ObjectRegistry;

XYZLabs &app();
EventManager &event_manager();
TaskManager &task_manager();
ObjectRegistry &object_registry();

}
