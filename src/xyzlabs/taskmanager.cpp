
#include "taskmanager.hpp"
#include "xyzlabs.hpp"


void TaskManager::start() {
    worker = std::thread([this] {
        while(running) {
            std::unique_lock lock(mux);
            cv.wait(lock, [this]() {
                return busy;
            });
            
            while(running) {
                std::unique_ptr<Task> task = nullptr;
                tasks.try_dequeue(task);
                if(task == nullptr) {
                    break;
                } else {
                    task->execute();
                }
            }

            busy = false;
            lock.unlock();
        }
    });
}
