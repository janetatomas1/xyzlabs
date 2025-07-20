
#ifndef TASK_HPP
#define TASK_HPP

#include <memory>
#include <array>
#include <boost/asio.hpp>

inline void swap(std::atomic<uint8_t> &a, std::atomic<uint8_t> &b) {
    auto tmp = a.load();
    a.store(b);
    b.store(tmp);
}

class OnceTaskInterface {};
class PeriodicTaskInterface {};

template <typename T>
class OnceTask: public OnceTaskInterface {
protected:
    T result_;
    std::atomic<bool> finished_ = false;

public:
    inline bool finished() {
        return finished_;
    }
    virtual void execute();
    const T& get_value() {
        return result_;
    };
};

template <typename T>
class PeriodicTask: public OnceTaskInterface {
protected:
    std::array<T, 3> states_;
    std::atomic<bool> writeReady_ = false;
    std::atomic<uint8_t> frontIdx_ = 0;
    std::atomic<uint8_t> middleIdx_ = 1;
    std::atomic<uint8_t> backIdx_ = 2;

    boost::asio::steady_timer timer_;
    uint64_t milisecondsTimeout_ = 1000;
    std::atomic<bool> running_ = false;

    virtual void update_() {
        if(!writeReady_) {
            update();
            writeReady_ = true;
            swap(backIdx_, middleIdx_);
        }

        timer_.expires_after(std::chrono::milliseconds(milisecondsTimeout_));
        timer_.async_wait([this]() {
            update_();
        });
    };
    virtual void update() {};
public:

    inline bool write_ready() {
        return writeReady_;
    }
    void start() {
        running_ = true;
        update_();
    }

    void stop() {
        running_ = false;
    }

    const T& get_value() {
        if(writeReady_) {
            swap(frontIdx_, middleIdx_);
            writeReady_ = false;
        }

        return states_[frontIdx_];
    };
};


#endif
