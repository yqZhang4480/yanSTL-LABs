#pragma once
#include "common.hpp"

namespace my {

template <typename T, typename Deleter>
class unique_ptr_base {
public:

    /// 请在此处定义单对象版本和数组版本 unique_ptr 的共有成员。

protected:
    T* ptr_;
    Deleter deleter_;
};

template <typename T, typename Deleter>
class unique_ptr : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;
public:
    using Base::Base; // 使用基类构造函数

    /// 请在此处定义单对象版本 unique_ptr 的特有成员。
};

template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;
public:
    using Base::Base; // 使用基类构造函数

    /// 请在此处定义数组版本 unique_ptr 的特有成员。
};

/// 还需在此处定义 make_unique 函数。

} // namespace my