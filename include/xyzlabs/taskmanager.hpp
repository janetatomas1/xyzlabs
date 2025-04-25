
#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <boost/asio.hpp>

namespace asio = boost::asio;

class TaskManager {
    const size_t threadCount_ = 4;

    asio::thread_pool pool_;
    asio::io_context io_;

public:
    TaskManager();
    void run();
    void stop();
};

#endif
