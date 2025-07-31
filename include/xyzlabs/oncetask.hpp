
#ifndef ONCETASK_HPP
#define ONCETASK_HPP

#include <memory>
#include <atomic>
#include <string>

class OnceTaskInterface {
    std::string error_;
    uint64_t progress_ = 0;
    uint64_t progressMaximum_ = 0;

public:
    virtual void execute() {};
    const std::string& error() { return error_; };
    void set_error(const std::string &error) { error_ = error; };
    bool has_error() {
        return error_.empty();
    };
    inline uint64_t progress() {
        return progress_;
    }
    inline uint64_t progress_max() {
        return progressMaximum_;
    }
    inline void set_progress(uint64_t progress) {
        progress_ = progress;
    }
};

template <typename T>
class OnceTask: public OnceTaskInterface {
protected:
    T result_;
    std::atomic<bool> finished_ = false;

public:
    inline bool finished() {
        return finished_;
    }
    const T& get_value() {
        return result_;
    };
};

#endif
