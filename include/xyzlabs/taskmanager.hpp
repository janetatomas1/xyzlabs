
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <thread>
#include <memory>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <readerwriterqueue/readerwriterqueue.h>

#include "task.hpp"

class TaskManager {
    std::thread worker;
    moodycamel::ReaderWriterQueue<std::unique_ptr<Task>> tasks;
    std::condition_variable cv;
    std::mutex mux;

    bool running;
    bool busy;
public:
    TaskManager() = default;
    void start();
};

#endif
