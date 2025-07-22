
// #ifndef TASK_HPP
// #define TASK_HPP

// #include <memory>


// class OnceTaskInterface {
// public:
//     virtual void execute() {};
// };

// template <typename T>
// class OnceTask: public OnceTaskInterface {
// protected:
//     T result_;
//     std::atomic<bool> finished_ = false;

// public:
//     inline bool finished() {
//         return finished_;
//     }
//     const T& get_value() {
//         return result_;
//     };
// };


// #endif
