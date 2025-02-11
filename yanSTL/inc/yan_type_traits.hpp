#pragma once

namespace my
{

// 仅当T具有可接受Args...类型实参的构造函数时返回真。
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// 仅当T为void时返回真。
template <typename T>
inline constexpr bool is_void_v = false;

template <> // 提供特化T=void
inline constexpr bool is_void_v<void> = true;
template <> // 提供特化T=c void
inline constexpr bool is_void_v<const void> = true;
template <> // 提供特化T=v void
inline constexpr bool is_void_v<volatile void> = true;
template <> // 提供特化T=cv void
inline constexpr bool is_void_v<const volatile void> = true;


/// 仅当T1, T2是相同类型时返回真。考虑CV修饰。
template <typename T1, typename T2>
inline constexpr bool is_same_v = false;

template <typename T> // 提供特化：仅传入一个类型T
inline constexpr bool is_same_v<T, T> = true;

/// 仅当T为Types...中众类型之一时返回真。这不是标准规定的算子。
template <typename T, typename... Types> // 折叠表达式依次判断
inline constexpr bool is_any_of_v = (is_same_v<T, Types> || ...);


/// 返回对应的带有const修饰的类型。
template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
using add_const_t = add_const<T>::type;

/// 返回对应的不带有const修饰的类型。
template <typename T>
struct remove_const {
    using type = T;
};

// 对带c修饰类型特化
template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = remove_const<T>::type;

/// 返回对应的不带有CV修饰的类型。
template <typename T>
struct remove_cv {
    using type = remove_const<T>::type;
};

// 对带v修饰类型特化，复用removeconst
template <typename T>
struct remove_cv<volatile T> {
    using type = remove_const<T>::type;
};

template <typename T>
using remove_cv_t = remove_cv<T>::type;

/// 仅当T为布尔类型、字符类型、整数类型时返回真。
template <typename T>
inline constexpr bool is_integral_v = false;

template <typename T> requires is_any_of_v<remove_cv_t<T>, bool, int, short, long, long long, char, unsigned short, unsigned, unsigned long, unsigned long long, unsigned char, signed char, wchar_t, char8_t, char16_t, char32_t>
inline constexpr bool is_integral_v<T> = true;

/// 仅当T具有顶层volatile修饰时返回真。
template <typename T>
inline constexpr bool is_volatile_v = false;

template <typename T> // 提供具有顶层v修饰的特化
inline constexpr bool is_volatile_v<volatile T> = true;

/// 仅当T为指针时返回真。
template <typename T>
inline constexpr bool is_pointer_v = false;

template <typename T> // 提供可解析为指针类型的特化
inline constexpr bool is_pointer_v<T*> = true;

template <typename T> // 提供可解析为指针类型的特化
inline constexpr bool is_pointer_v<T* const> = true;

template <typename T> // 提供可解析为指针类型的特化
inline constexpr bool is_pointer_v<T* volatile> = true;

/// 仅当T为引用时返回真。
template <typename T>
inline constexpr bool is_reference_v = false;

template <typename T>
inline constexpr bool is_reference_v<T&> = true;

template <typename T>
inline constexpr bool is_reference_v<T&&> = true;

/// 仅当T为函数时返回真。提示：只有U为函数或引用时，is_const_v<const U>返回假。
template <typename T>
inline constexpr bool is_function_v = false;

template <typename Ret, typename... Args>
inline constexpr bool is_function_v<Ret(Args...)> = true;

/// 仅当T为除了void、函数、引用以外的任何类型时返回真。
template <typename T>
inline constexpr bool is_object_v = !(is_any_of_v<T, void> || is_function_v<T> || is_reference_v<T>);

/// 仅当T为原生数组时返回真。
template <typename T>
inline constexpr bool is_array_v = false;

template <typename T, size_t N>
inline constexpr bool is_array_v<T[N]> = true;

template <typename T>
inline constexpr bool is_array_v<T[]> = true;

/// 仅当T能完成默认构造时返回真。
template <typename T>
inline constexpr bool is_default_constructible_v = is_constructible_v<T>;

/// 仅当T能完成拷贝构造时返回真。
template <typename T>
inline constexpr bool is_copy_constructible_v = is_constructible_v<T, const T&>;

/// 仅当T能完成移动构造时返回真。
template <typename T>
inline constexpr bool is_move_constructible_v = is_constructible_v<T, T&&>;

/// 返回对应的非引用类型。
template <typename T>
struct remove_reference {
    using type = T;
};

// 对左值引用特化
template <typename T>
struct remove_reference<T&> {
    using type = T;
};

// 对右值引用（此处不是万能引用）特化
template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = remove_reference<T>::type;

/// 返回对应的左值引用类型。适用引用折叠规则。
template <typename T>
using add_lvalue_reference_t = T&;

/// 返回对应的右值引用类型。适用引用折叠规则。
template <typename T>
using add_rvalue_reference_t = T&&;

/// 返回对应的非引用类型的指针。
template <typename T>
using add_pointer_t = remove_reference_t<T>*;

/// 返回数组T的维度，若非数组返回0。
template <typename T>
inline constexpr size_t rank_v = 0;

template <typename T>
inline constexpr size_t rank_v<T[]> = rank_v<T> + 1;

template <typename T, size_t N>
inline constexpr size_t rank_v<T[N]> = rank_v<T> + 1;

/// 返回数组T在第I个维度上的大小。
template <typename T, size_t I>
inline constexpr size_t extent_v = 0;

template <typename T, size_t I>
inline constexpr size_t extent_v<T[], I> = extent_v<T, I - 1>;

template <typename T, size_t I, size_t N>
inline constexpr size_t extent_v<T[N], I> = extent_v<T, I - 1>;

template <typename T, size_t N>
inline constexpr size_t extent_v<T[N], 0> = N;

/// 获取数组T的元素的类型。若非数组返回其本身。
template <typename T>
struct remove_extent {
    using type = T;
};

template <typename T>
struct remove_extent<T[]> {
    using type = T;
};

template <typename T, size_t N>
struct remove_extent<T[N]> {
    using type = T;
};

template <typename T>
using remove_extent_t = remove_extent<T>::type;

/// 当 B == true 时返回 T，否则返回 F。
template <bool B, typename T, typename F>
struct conditional {
    using type = void;
};

template <typename T, typename F>
struct conditional<true, T, F> {
    using type = T;
};


template <typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = conditional<B, T, F>::type;

/// 返回T的退化类型。
template <typename T>
struct decay { using type = remove_cv_t<T>; };

template <typename T>
struct decay<const T> { using type = remove_cv_t<T>; };

template <typename T>
struct decay<volatile T> { using type = remove_cv_t<T>; };

template <typename T>
struct decay<const volatile T> { using type = remove_cv_t<T>; };


template <typename T> requires is_function_v<T>
struct decay<T> { using type = remove_cv_t<add_pointer_t<T>>; };

template <typename T> requires is_array_v<T>
struct decay<T> { using type = remove_extent_t<T>*; };

template <typename T> requires is_reference_v<T> && !is_array_v<remove_reference_t<T>>
struct decay<T> { using type = remove_cv_t<remove_reference_t<T>>; };

template <typename T> requires is_reference_v<T> && is_array_v<remove_reference_t<T>>
struct decay<T> { using type = remove_extent_t<remove_reference_t<T>>*; };

template <typename T>
using decay_t = typename decay<T>::type;
}