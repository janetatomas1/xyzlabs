
#include "resultmanager.hpp"

void ResultManager::receive_result(std::unique_ptr<Result> res) {
    resultQueue_.enqueue(std::move(res));
}

void ResultManager::publish() {
    
}
