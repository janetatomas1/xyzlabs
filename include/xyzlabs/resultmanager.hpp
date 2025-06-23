
#ifndef RESULTMANAGER_HPP
#define RESULTMANAGER_HPP

#include <memory>

#include "concurrentqueue.h"
#include "result.hpp"

class ResultManager {
    moodycamel::ConcurrentQueue<std::unique_ptr<Result>> resultQueue_;
    
public:
    void receive_result(std::unique_ptr<Result> result);
    void publish();
};

#endif
