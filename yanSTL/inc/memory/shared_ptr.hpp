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

/// 在此处继承control_block_base并实现控制块逻辑。

template <typename T>
class shared_ptr {

    /// 在此处实现 shared_ptr 相关成员。

private:
    template <typename U>
    friend class shared_ptr;
    template <typename U>
    friend class weak_ptr;
    template <typename U>
    friend class enable_shared_from_this;

    T* ptr_;
    control_block_base* cb_;
};

template <typename T>
class weak_ptr {
 
    /// 在此处实现 weak_ptr 相关成员。

private:
    template <typename U>
    friend class shared_ptr;

    T* ptr_;
    control_block_base* cb_;
};

#ifndef DISMISS_ENABLE_SHARED_FROM_THIS 

/// 在此处实现 enable_shared_from_this 相关逻辑。

#endif


/// 还需在此实现 make_shared 和 allcoate_shared。

} // namespace my