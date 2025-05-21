
#ifndef TASK_HPP
#define TASK_HPP

#include <memory>

#include "xyzlabs/result.hpp"

class Task {
public:
    virtual std::unique_ptr<Result> execute() {
        return nullptr;
    };
};

#endif
