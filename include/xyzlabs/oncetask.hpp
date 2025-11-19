
#pragma once

#include <memory>
#include <atomic>
#include <string>
#include <type_traits>

template <typename T>
constexpr bool is_movable_v = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

namespace xyzlabs {

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
        return !error_.empty();
    };
};

template <typename T>
class OnceTask: public OnceTaskInterface {
protected:
    T result_;
public:
    virtual const T& get_value_ref() {
        return result_;
    };
    virtual T get_value() {
        if constexpr (is_movable_v<T>) {
            return std::move(result_);
        } else {
            return result_;
        }
    };
};

}
