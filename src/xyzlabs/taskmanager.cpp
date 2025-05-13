
#include <spdlog/spdlog.h>

#include "taskmanager.hpp"


TaskManager::TaskManager(): pool_(asio::thread_pool(threadCount_)) {}

void TaskManager::execute_task(std::unique_ptr<Task> task) {
    asio::post(pool_, [task = std::move(task)]() mutable {
        task->execute();
    });
}

void TaskManager::run() {
    asio::post(pool_, [this]() {
        io_.run();
    });
}

void TaskManager::stop() {
    io_.stop();
    pool_.stop();
    pool_.join();
}
