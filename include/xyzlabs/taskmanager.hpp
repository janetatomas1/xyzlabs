
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <boost/asio.hpp>

#include "xyzlabs/oncetask.hpp"
#include "xyzlabs/periodictask.hpp"

namespace asio = boost::asio;

class TaskManager {
    const size_t threadCount_ = 4;

    asio::thread_pool pool_;
    asio::io_context io_;

public:
    TaskManager();
    void execute_task(std::shared_ptr<OnceTaskInterface> task);
    void execute_periodic_task(std::shared_ptr<PeriodicTaskInterface> task);
    void stop_periodic_task(std::shared_ptr<PeriodicTaskInterface> task);
    void run();
    void stop();
    asio::io_context& io_ctx();
};

#endif
