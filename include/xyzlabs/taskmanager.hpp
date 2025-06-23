
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <boost/asio.hpp>

#include "xyzlabs/task.hpp"

namespace asio = boost::asio;

class TaskManager {
    const size_t threadCount_ = 4;

    asio::thread_pool pool_;
    asio::io_context io_;

public:
    TaskManager();
    void execute_task(std::unique_ptr<Task> task);
    void execute_periodic_task(std::unique_ptr<Task> task);
    void run();
    void stop();
};

#endif
