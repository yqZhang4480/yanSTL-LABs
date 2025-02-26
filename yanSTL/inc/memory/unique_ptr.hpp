#pragma once
#include "common.hpp"
#include <compare>
#include <utility>
#include <type_traits>

namespace my {

template <typename T>
class default_deleter {
    void operator()(T* pointer) { delete pointer; }
};

template <typename T, typename Deleter = default_deleter<T>>
class unique_ptr_base {
public:
    template< class U, class E > 
    friend class unique_ptr_base;

    using pointer = T*;

    unique_ptr_base() noexcept : ptr_(nullptr), deleter_(Deleter()) {}
    unique_ptr_base( std::nullptr_t ) noexcept : ptr_(nullptr), deleter_([](T*){}) {}

    explicit unique_ptr_base( pointer p ) noexcept : ptr_(p), deleter_(Deleter()) {}

    template<typename D = Deleter>
    explicit unique_ptr_base(T* p, D&& d) noexcept : ptr_(p), deleter_(std::forward<D>(d)) {}

    template<typename A> requires (std::is_lvalue_reference_v<Deleter> && std::is_same_v<std::remove_cvref_t<Deleter>, A>)
    unique_ptr_base(T* p, A& d) noexcept
        : ptr_(p), deleter_(d) {}

    template<typename A> requires std::is_lvalue_reference_v<Deleter>
    unique_ptr_base(T* p, A&&) = delete;

    template< class U, class E >
    unique_ptr_base( unique_ptr_base<U, E>&& u ) noexcept : ptr_(std::exchange(u.ptr_, nullptr)), deleter_(std::move(u.deleter_)) {}
    template< class U, class E >
    unique_ptr_base& operator=( unique_ptr_base<U, E>&& u ) noexcept
    {
        _delete(ptr_);
        ptr_ = std::exchange(u.ptr_, nullptr);
        deleter_ = std::move(deleter_);
        return *this;
    }
    unique_ptr_base& operator=( std::nullptr_t ) noexcept
    {
        reset();
    }

    void reset( pointer p = pointer() ) noexcept
    {
        _delete(ptr_);
        ptr_ = p;
    }

    pointer release() noexcept
    {
        return std::exchange(ptr_, nullptr);
    }

    void swap( unique_ptr_base& other ) noexcept
    {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

    pointer get() const noexcept { return ptr_; }
    Deleter& get_deleter() noexcept { return deleter_; }
    const Deleter& get_deleter() const noexcept { return deleter_; }

    operator bool() const noexcept { return !!ptr_; }

    ~unique_ptr_base() { _delete(ptr_); }

private:
    void _delete( pointer p ) noexcept { if (p) { deleter_(p); } }

protected:
    T* ptr_;
    Deleter deleter_;
};

template <typename T, typename Deleter>
class unique_ptr : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;
public:
    using Base::Base; // 使用基类构造函数

    const T& operator*() const { return *Base::get(); }
    T& operator*() { return *Base::get(); }
    T* operator->() const { return Base::get(); } 
};

template <typename T, typename Deleter>
class unique_ptr<T[], Deleter> : public unique_ptr_base<T, Deleter> {
    using Base = unique_ptr_base<T, Deleter>;
public:
    using Base::Base; // 使用基类构造函数

    T& operator[]( std::size_t i ) const { return *(Base::get() + i); }
};


template <typename T, typename D>
bool operator==( std::nullptr_t, const unique_ptr_base<T, D>& u ) { return !u; }
template <typename T, typename D>
bool operator==( const unique_ptr_base<T, D>& u, std::nullptr_t ) { return !u; }
template <typename T, typename D>
auto operator<=>( const unique_ptr_base<T, D>& u, std::nullptr_t n ) { return u.get() <=> n; }
template< class T1, class D1, class T2, class D2 >
bool operator==( const unique_ptr_base<T1, D1>& u, const unique_ptr_base<T2, D2>& v ) { return u.get() == v.get(); }
template< class T1, class D1, class T2, class D2 >
auto operator<=>( const unique_ptr_base<T1, D1>& u, const unique_ptr_base<T2, D2>& v ) { return u.get() <=> v.get(); }

namespace detail
{
template<typename Tp>
struct make_unique_return_type
{ typedef unique_ptr<Tp> single_object; };

template<typename Tp>
struct make_unique_return_type<Tp[]>
{ typedef unique_ptr<Tp[]> array; };

template<typename Tp, size_t N>
struct make_unique_return_type<Tp[N]>
{ struct invalid_type { }; };

template<typename Tp>
using unique_ptr_t = typename make_unique_return_type<Tp>::single_object;
template<typename Tp>
using unique_ptr_array_t = typename make_unique_return_type<Tp>::array;
template<typename Tp>
using invalid_make_unique_t = typename make_unique_return_type<Tp>::invalid_type;
}

template<typename Tp, typename... Args>
inline detail::unique_ptr_t<Tp> make_unique(Args&&... args)
{ return unique_ptr<Tp>(new Tp(std::forward<Args>(args)...)); }

template<typename Tp>
inline detail::unique_ptr_array_t<Tp> make_unique(size_t num)
{ return unique_ptr<Tp>(new std::remove_extent_t<Tp>[num]()); }

template<typename Tp, typename... Args>
detail::invalid_make_unique_t<Tp>
make_unique(Args&&...) = delete;

} // namespace my