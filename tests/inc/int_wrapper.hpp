#pragma once
#include <utility>

namespace my
{
    namespace test
    {

class int_wrapper {
public:
    static inline size_t construct_count = 0;
    static inline size_t copy_count = 0;
    static inline size_t move_count = 0;
    static inline size_t current_object_count = 0;

    static void reset()
    {
        construct_count = 0;
        copy_count = 0;
        move_count = 0;
        current_object_count = 0;
    }

    int_wrapper(int value = 0) : value_(value) {
        ++construct_count;
        ++current_object_count;
    }

    int_wrapper(const int_wrapper& other) : value_(other.value_) {
        ++copy_count;
        ++construct_count;
        ++current_object_count;
    }

    int_wrapper(int_wrapper&& other) noexcept : value_(std::move(other.value_)) {
        ++move_count;
        ++construct_count;
        ++current_object_count;
    }

    int_wrapper& operator=(const int_wrapper& other) {
        if (this != &other) {
            value_ = other.value_;
            ++copy_count;
        }
        return *this;
    }

    int_wrapper& operator=(int_wrapper&& other) noexcept {
        if (this != &other) {
            value_ = std::move(other.value_);
            ++move_count;
        }
        return *this;
    }

    friend bool operator==(const int_wrapper& a, const int_wrapper& b) { return a.value_ == b.value_; }
    friend bool operator==(const int& a, const int_wrapper& b) { return a == b.value_; }
    friend bool operator==(const int_wrapper& a, const int& b) { return a.value_ == b; }

    ~int_wrapper() { --current_object_count; }

    operator int () { return value_; }
private:
    int value_;
};

    }
}

