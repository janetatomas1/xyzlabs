
#pragma once

#include <array>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>

namespace xyzlabs {

inline void swap(std::atomic<uint8_t> &a, std::atomic<uint8_t> &b) {
    auto tmp = a.load();
    a.store(b);
    b.store(tmp);
}

class PeriodicTaskInterface {
    void update_() {
        timer_.expires_after(std::chrono::milliseconds(milisecondsTimeout_));
        timer_.async_wait(handler);
    };
    std::function<void(const boost::system::error_code& code)> handler = [this](const boost::system::error_code& code) {
        if(running_) {
            if(!writeReady_) {
                update();
                writeReady_ = true;
                swap(backIdx_, middleIdx_);
            }

            timer_.expires_after(std::chrono::milliseconds(milisecondsTimeout_));
            timer_.async_wait(handler);    
        }
    };


protected:
    boost::asio::steady_timer timer_;
    uint64_t milisecondsTimeout_ = 100;
    std::atomic<bool> running_ = false;

    std::atomic<bool> writeReady_ = false;
    std::atomic<uint8_t> frontIdx_ = 0;
    std::atomic<uint8_t> middleIdx_ = 1;
    std::atomic<uint8_t> backIdx_ = 2;

public:
    PeriodicTaskInterface(uint64_t timeout);
    inline bool write_ready() {
        return writeReady_;
    }
    void stop() {
        running_ = false;
        timer_.cancel();
        timer_.wait();
    }
    void start() {
        running_ = true;
        update_();
    }
    virtual void update() {};
};

template <typename T>
class PeriodicTask: public PeriodicTaskInterface {
protected:
    std::array<T, 3> states_;

public:
    PeriodicTask(const T &initialValue, uint64_t timeout = 1000): PeriodicTaskInterface(timeout) {
        states_ = {initialValue, initialValue, initialValue};
    };
    PeriodicTask(uint64_t timeout = 1000): PeriodicTaskInterface(timeout) {};
    const T& get_value() {
        if(writeReady_) {
            swap(frontIdx_, middleIdx_);
            writeReady_ = false;
        }

        return states_[frontIdx_];
    };

    virtual void update_state(const T &state1, T &state2) {}
    void update() override {
        if(writeReady_) {
            update_state(states_[middleIdx_], states_[backIdx_]);
        } else {
            update_state(states_[frontIdx_], states_[backIdx_]);
        }
    }
};

}