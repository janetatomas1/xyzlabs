
#include "taskmanager.hpp"
#include "xyzlabs.hpp"


void TaskManager::start() {
    worker = std::thread([this] {
        while(running) {
            std::unique_lock lock(mux);
            cv.wait(lock);
            
            while(!tasks.empty() && running) {
                auto &t = tasks.front();
                tasks.pop();
            }
        }
    });
}
