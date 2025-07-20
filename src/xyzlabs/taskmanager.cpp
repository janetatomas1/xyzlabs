
#include <spdlog/spdlog.h>

#include "xyzlabs/taskmanager.hpp"
#include "xyzlabs/xyzlabs.hpp"


TaskManager::TaskManager(): pool_(asio::thread_pool(threadCount_)) {}

void TaskManager::execute_task(std::shared_ptr<OnceTaskInterface> &task) {
    asio::post(pool_, []() mutable {

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

void TaskManager::execute_periodic_task(std::shared_ptr<PeriodicTaskInterface> &task) {

}
