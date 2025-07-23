
#ifndef PERIODICTASK_HPP
#define PERIODICTASK_HPP

#include <array>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

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
        if(!writeReady_) {
            update();
            writeReady_ = true;
            swap(backIdx_, middleIdx_);
        }

        timer_.expires_after(std::chrono::milliseconds(milisecondsTimeout_));
        timer_.async_wait(handler);    
    };


protected:
    boost::asio::steady_timer timer_;
    uint64_t milisecondsTimeout_ = 1000;
    std::atomic<bool> running_ = false;

    std::atomic<bool> writeReady_ = false;
    std::atomic<uint8_t> frontIdx_ = 0;
    std::atomic<uint8_t> middleIdx_ = 1;
    std::atomic<uint8_t> backIdx_ = 2;
    boost::asio::io_context io;

public:
    PeriodicTaskInterface();
    inline bool write_ready() {
        return writeReady_;
    }
    void stop() {
        running_ = false;
    }
    void start() {
        running_ = true;
        spdlog::info("start");
        update_();
    }
    virtual void update() {};
};

template <typename T>
class PeriodicTask: public PeriodicTaskInterface {
protected:
    std::array<T, 3> states_;

public:
    PeriodicTask(): PeriodicTaskInterface() {};
    const T& get_value() {
        if(writeReady_) {
            swap(frontIdx_, middleIdx_);
            writeReady_ = false;
        }

        return states_[frontIdx_];
    };

    virtual void update_state(const T *state1, T *state2) {}
    void update() override {
        T *state1 = nullptr;
        T *state2 = nullptr;

        if(writeReady_) {
            state1 = &states_[middleIdx_];
        } else {
            state1 = &states_[frontIdx_];
        }
        state2 = &states_[backIdx_];
        update_state(state1, state2);
    }
};

#endif
