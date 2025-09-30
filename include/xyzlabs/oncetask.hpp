
#ifndef ONCETASK_HPP
#define ONCETASK_HPP

#include <memory>
#include <atomic>
#include <string>

class OnceTaskInterface {
protected:
    std::string error_;
    std::atomic<bool> finished_ = false;

public:
    inline bool finished() {
        return finished_;
    }
    void finish() {
        finished_ = true;
    }
    virtual void execute() {};
    const std::string& error() { return error_; };
    void set_error(const std::string &error) { error_ = error; };
    bool has_error() {
        return error_.empty();
    };
};

template <typename T>
class OnceTask: public OnceTaskInterface {
protected:
    T result_;
public:
    const T& get_value() {
        return result_;
    };
};

#endif
