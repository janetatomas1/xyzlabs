
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <thread>
#include <memory>
#include <queue>
#include <condition_variable>
#include <mutex>

#include "task.hpp"

class TaskManager {
    std::thread worker;
    std::queue<std::unique_ptr<Task>> tasks;
    std::condition_variable cv;
    std::mutex mux;

    bool running;
    bool busy;
public:
    TaskManager() = default;
    void start();
};

#endif
