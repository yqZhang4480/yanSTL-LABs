#include <atomic>
#include <stdexcept>

namespace my {

class control_block_base {
public:
    virtual ~control_block_base() = default;
    virtual void dispose() noexcept = 0;
    virtual void destroy() noexcept = 0;
    std::atomic<size_t> shared_count{ 1 };
    std::atomic<size_t> weak_count{ 0 };
};

template <typename T, typename Deleter = std::default_delete<T>>
class regular_control_block : public control_block_base {
public:
    regular_control_block(T* ptr, Deleter deleter) : ptr_(ptr), deleter_(std::move(deleter)) {}

    void dispose() noexcept override {
        deleter_(ptr_);
    }

    void destroy() noexcept override {
        delete this;
    }

private:
    T* ptr_;
    Deleter deleter_;
};

template <typename T>
class make_shared_control_block : public control_block_base {
public:
    template <typename... Args>
    make_shared_control_block(Args&&... args) {
        new (&storage_) T(std::forward<Args>(args)...);
    }

    void dispose() noexcept override {
        reinterpret_cast<T*>(&storage_)->~T();
    }

    void destroy() noexcept override {
        delete this;
    }

    T* get() noexcept {
        return reinterpret_cast<T*>(&storage_);
    }

private:
    alignas(T) std::aligned_storage_t<sizeof(T), alignof(T)> storage_;
};

template <typename T>
class enable_shared_from_this;

template <typename T>
class shared_ptr {
public:
    shared_ptr() noexcept : ptr_(nullptr), cb_(nullptr) {}

    explicit shared_ptr(T* ptr) : ptr_(ptr), cb_(new regular_control_block<T>(ptr, std::default_delete<T>())) {
        enable_shared_from_this_helper(ptr);
    }

    template <typename Deleter>
    shared_ptr(T* ptr, Deleter deleter) : ptr_(ptr), cb_(new regular_control_block<T, Deleter>(ptr, std::move(deleter))) {
        enable_shared_from_this_helper(ptr);
    }

    template <typename... Args>
    explicit shared_ptr(make_shared_control_block<T>* cb) : cb_(cb) {
        ptr_ = cb->get();
        enable_shared_from_this_helper(ptr_);
    }

    ~shared_ptr() {
        if (cb_ != nullptr) {
            if (cb_->shared_count.fetch_sub(1) == 1) {
                cb_->dispose();
                if (cb_->weak_count == 0) {
                    cb_->destroy();
                }
            }
        }
    }

    shared_ptr(const shared_ptr& other) noexcept : ptr_(other.ptr_), cb_(other.cb_) {
        if (cb_ != nullptr) {
            cb_->shared_count.fetch_add(1);
        }
    }

    shared_ptr(shared_ptr&& other) noexcept : ptr_(other.ptr_), cb_(other.cb_) {
        other.ptr_ = nullptr;
        other.cb_ = nullptr;
    }

    explicit shared_ptr(const weak_ptr<T>& wp) noexcept : ptr_(wp.ptr_), cb_(wp.cb_) {
        if (cb_ != nullptr && cb_->shared_count.load() > 0) {
            cb_->shared_count.fetch_add(1);
        }
        else {
            ptr_ = nullptr;
            cb_ = nullptr;
        }
    }


    shared_ptr& operator=(const shared_ptr& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            cb_ = other.cb_;
            if (cb_ != nullptr) {
                cb_->shared_count.fetch_add(1);
            }
        }
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            cb_ = other.cb_;
            other.ptr_ = nullptr;
            other.cb_ = nullptr;
        }
        return *this;
    }

    T* get() const noexcept { return ptr_; }
    T& operator*() const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    bool operator==(std::nullptr_t) const noexcept { return ptr_ == nullptr; }

    size_t use_count() const noexcept {
        return cb_ != nullptr ? cb_->shared_count.load() : 0;
    }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    void reset() noexcept {
        shared_ptr().swap(*this);
    }

    void swap(shared_ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(cb_, other.cb_);
    }

private:
    template <typename U>
    friend class shared_ptr;
    template <typename U>
    friend class weak_ptr;
    template <typename U>
    friend class enable_shared_from_this;

    T* ptr_;
    control_block_base* cb_;

    void enable_shared_from_this_helper(T* ptr) {
        if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
            enable_shared_from_this<T>* esft = static_cast<enable_shared_from_this<T>*>(ptr);
            esft->weak_this_ = *this;
        }
    }
};

template <typename T>
class weak_ptr {
public:
    weak_ptr() noexcept : ptr_(nullptr), cb_(nullptr) {}

    weak_ptr(const shared_ptr<T>& sp) noexcept : ptr_(sp.ptr_), cb_(sp.cb_) {
        if (cb_ != nullptr) {
            cb_->weak_count.fetch_add(1);
        }
    }

    ~weak_ptr() {
        if (cb_ != nullptr) {
            if (cb_->weak_count.fetch_sub(1) == 1) {
                if (cb_->shared_count == 0) {
                    cb_->destroy();
                }
            }
        }
    }

    weak_ptr(const weak_ptr& other) noexcept : ptr_(other.ptr_), cb_(other.cb_) {
        if (cb_ != nullptr) {
            cb_->weak_count.fetch_add(1);
        }
    }

    weak_ptr(weak_ptr&& other) noexcept : ptr_(other.ptr_), cb_(other.cb_) {
        other.ptr_ = nullptr;
        other.cb_ = nullptr;
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            cb_ = other.cb_;
            if (cb_ != nullptr) {
                cb_->weak_count.fetch_add(1);
            }
        }
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr_ = other.ptr_;
            cb_ = other.cb_;
            other.ptr_ = nullptr;
            other.cb_ = nullptr;
        }
        return *this;
    }

    shared_ptr<T> lock() const noexcept {
        if (cb_ != nullptr && cb_->shared_count > 0) {
            return shared_ptr<T>(*this);
        }
        else {
            return shared_ptr<T>();
        }
    }

    bool expired() const noexcept {
        return cb_ == nullptr || cb_->shared_count == 0;
    }

    void reset() noexcept {
        weak_ptr().swap(*this);
    }

    void swap(weak_ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(cb_, other.cb_);
    }

private:
    template <typename U>
    friend class shared_ptr;

    T* ptr_;
    control_block_base* cb_;
};

template <typename T>
class enable_shared_from_this {
public:
    shared_ptr<T> shared_from_this() {
        return weak_this_.lock();
    }

    shared_ptr<const T> shared_from_this() const {
        return weak_this_.lock();
    }

private:
    template <typename U>
    friend class shared_ptr;

    weak_ptr<T> weak_this_;
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto* cb = new make_shared_control_block<T>(std::forward<Args>(args)...);
    return shared_ptr<T>(cb);
}

} // namespace my