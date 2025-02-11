#pragma once
#include "common.hpp"

namespace my {

template <typename T, typename Deleter>
class unique_ptr_base {
public:
    unique_ptr_base(T* ptr = nullptr, Deleter deleter = Deleter())
        : ptr_(ptr), deleter_(deleter) {
    }

    // ��ֹ���������뿽����ֵ
    unique_ptr_base(const unique_ptr_base&) = delete;
    unique_ptr_base& operator=(const unique_ptr_base&) = delete;

    // �ƶ�������ƶ���ֵ
    unique_ptr_base(unique_ptr_base&& other) noexcept
        : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr;
    }

    unique_ptr_base& operator=(unique_ptr_base&& other) noexcept {
        if (this != &other) {
            deleter_(ptr_);  // ���ͷŵ�ǰָ��
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // �����������ͷ���Դ
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
    using Base::Base; // ʹ�û��๹�캯��

    // ����ָ������
    T& operator*() const {
        return *this->ptr_;
    }

    T* operator->() const {
        return this->ptr_;
    }

    // ��ʽת��Ϊ��ָ��
    operator T* () const {
        return this->ptr_;
    }

    // �������ú��������� unique_ptr ����Ķ���
    void reset(T* ptr = nullptr) {
        if (this->ptr_ != ptr) {
            this->deleter_(this->ptr_);
            this->ptr_ = ptr;
        }
    }

    // ��ȡָ���ԭʼֵ
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
    using Base::Base; // ʹ�û��๹�캯��

    // ��ȡ�����Ԫ������
    T& operator[](std::size_t index) const {
        return this->ptr_[index];
    }

    // ���ú���������������
    void reset(T* ptr = nullptr) {
        if (this->ptr_ != ptr) {
            this->deleter_(this->ptr_);
            this->ptr_ = ptr;
        }
    }

    // ��ȡ�����ԭʼָ��
    T* release() {
        T* tmp = this->ptr_;
        this->ptr_ = nullptr;
        return tmp;
    }
};

// make_unique ��������ʵ��
template <typename T, typename... Args>
unique_ptr<T, std::default_delete<T>> make_unique(Args&&... args) {
    return unique_ptr<T, std::default_delete<T>>(new T(std::forward<Args>(args)...));
}

template <typename T>
unique_ptr<T[], std::default_delete<T[]>> make_unique<T[]>(std::size_t size) {
    return unique_ptr<T[], std::default_delete<T[]>>(new T[size]);
}

} // namespace my
