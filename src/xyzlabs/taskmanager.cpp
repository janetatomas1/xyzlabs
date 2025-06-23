
#include <spdlog/spdlog.h>

#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/xyzlabs.hpp"


TaskManager::TaskManager(): pool_(asio::thread_pool(threadCount_)) {}

void TaskManager::execute_task(std::unique_ptr<Task> task) {
    asio::post(pool_, [task = std::move(task)]() mutable {
        auto result = task->execute();
        XYZLabs::instance()
        .result_manager()
        .receive_result(std::move(result));
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

void TaskManager::execute_periodic_task(std::unique_ptr<Task> task) {

}
