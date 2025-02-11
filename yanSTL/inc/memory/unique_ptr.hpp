#pragma once
#include "common.hpp"

namespace my {

template <typename T, typename Deleter>
class unique_ptr_base {
public:
    unique_ptr_base(T* ptr = nullptr, Deleter deleter = Deleter())
        : ptr_(ptr), deleter_(deleter) {
    }

    // 禁止拷贝构造与拷贝赋值
    unique_ptr_base(const unique_ptr_base&) = delete;
    unique_ptr_base& operator=(const unique_ptr_base&) = delete;

    // 移动构造和移动赋值
    unique_ptr_base(unique_ptr_base&& other) noexcept
        : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }

    unique_ptr_base& operator=(unique_ptr_base&& other) noexcept {
        if (this != &other) {
            deleter_(ptr_);  // 先释放当前指针
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // 析构函数，释放资源
    ~unique_ptr_base() {
        if (ptr_) {
            deleter_(ptr_);
        }
    }

    T* get() const { return ptr_; }
    Deleter& get_deleter() { return deleter_; }

protected:
    T* ptr_;
    Deleter deleter_;
};

template <typename T, typename Deleter>
class unique_ptr : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;

public:
    using Base::Base; // 使用基类构造函数

    // 访问指针内容
    T& operator*() const {
        return *this->ptr_;
    }

    T* operator->() const {
        return this->ptr_;
    }

    // 显式转换为裸指针
    operator T* () const {
        return this->ptr_;
    }

    // 定义重置函数，重置 unique_ptr 管理的对象
    void reset(T* ptr = nullptr) {
        if (this->ptr_ != ptr) {
            this->deleter_(this->ptr_);
            this->ptr_ = ptr;
        }
    }

    // 获取指针的原始值
    T* release() {
        T* tmp = this->ptr_;
        this->ptr_ = nullptr;
        return tmp;
    }
};

template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;

public:
    using Base::Base; // 使用基类构造函数

    // 获取数组的元素类型
    T& operator[](std::size_t index) const {
        return this->ptr_[index];
    }

    // 重置函数，适用于数组
    void reset(T* ptr = nullptr) {
        if (this->ptr_ != ptr) {
            this->deleter_(this->ptr_);
            this->ptr_ = ptr;
        }
    }

    // 获取数组的原始指针
    T* release() {
        T* tmp = this->ptr_;
        this->ptr_ = nullptr;
        return tmp;
    }
};

// make_unique 工厂函数实现
template <typename T, typename... Args>
unique_ptr<T, std::default_delete<T>> make_unique(Args&&... args) {
    return unique_ptr<T, std::default_delete<T>>(new T(std::forward<Args>(args)...));
}

template <typename T>
unique_ptr<T[], std::default_delete<T[]>> make_unique<T[]>(std::size_t size) {
    return unique_ptr<T[], std::default_delete<T[]>>(new T[size]);
}

} // namespace my
