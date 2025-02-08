#pragma once

namespace my
{

// ����T���пɽ���Args...����ʵ�εĹ��캯��ʱ�����档
template <typename T, typename... Args>
inline constexpr bool is_constructible_v = __is_constructible(T, Args...);


/// ����TΪvoidʱ�����档
template <typename T>
inline constexpr bool is_void_v = false;

template <> // �ṩ�ػ�T=void
inline constexpr bool is_void_v<void> = true;
template <> // �ṩ�ػ�T=c void
inline constexpr bool is_void_v<const void> = true;
template <> // �ṩ�ػ�T=v void
inline constexpr bool is_void_v<volatile void> = true;
template <> // �ṩ�ػ�T=cv void
inline constexpr bool is_void_v<const volatile void> = true;


/// ����T1, T2����ͬ����ʱ�����档����CV���Ρ�
template <typename T1, typename T2>
inline constexpr bool is_same_v = false;

template <typename T> // �ṩ�ػ���������һ������T
inline constexpr bool is_same_v<T, T> = true;

/// ����TΪTypes...��������֮һʱ�����档�ⲻ�Ǳ�׼�涨�����ӡ�
template <typename T, typename... Types> // �۵����ʽ�����ж�
inline constexpr bool is_any_of_v = (is_same_v<T, Types> || ...);


/// ���ض�Ӧ�Ĵ���const���ε����͡�
template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
using add_const_t = add_const<T>::type;

/// ���ض�Ӧ�Ĳ�����const���ε����͡�
template <typename T>
struct remove_const {
    using type = T;
};

// �Դ�c���������ػ�
template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = remove_const<T>::type;

/// ���ض�Ӧ�Ĳ�����CV���ε����͡�
template <typename T>
struct remove_cv {
    using type = remove_const<T>::type;
};

// �Դ�v���������ػ�������removeconst
template <typename T>
struct remove_cv<volatile T> {
    using type = remove_const<T>::type;
};

template <typename T>
using remove_cv_t = remove_cv<T>::type;

/// ����TΪ�������͡��ַ����͡���������ʱ�����档
template <typename T>
inline constexpr bool is_integral_v = false;

template <typename T> requires is_any_of_v<remove_cv_t<T>, bool, int, short, long, long long, char, unsigned short, unsigned, unsigned long, unsigned long long, unsigned char, signed char, wchar_t, char8_t, char16_t, char32_t>
inline constexpr bool is_integral_v<T> = true;

/// ����T���ж���volatile����ʱ�����档
template <typename T>
inline constexpr bool is_volatile_v = false;

template <typename T> // �ṩ���ж���v���ε��ػ�
inline constexpr bool is_volatile_v<volatile T> = true;

/// ����TΪָ��ʱ�����档
template <typename T>
inline constexpr bool is_pointer_v = false;

template <typename T> // �ṩ�ɽ���Ϊָ�����͵��ػ�
inline constexpr bool is_pointer_v<T*> = true;

template <typename T> // �ṩ�ɽ���Ϊָ�����͵��ػ�
inline constexpr bool is_pointer_v<T* const> = true;

template <typename T> // �ṩ�ɽ���Ϊָ�����͵��ػ�
inline constexpr bool is_pointer_v<T* volatile> = true;

/// ����TΪ����ʱ�����档
template <typename T>
inline constexpr bool is_reference_v = false;

template <typename T>
inline constexpr bool is_reference_v<T&> = true;

template <typename T>
inline constexpr bool is_reference_v<T&&> = true;

/// ����TΪ����ʱ�����档��ʾ��ֻ��UΪ����������ʱ��is_const_v<const U>���ؼ١�
template <typename T>
inline constexpr bool is_function_v = false;

template <typename Ret, typename... Args>
inline constexpr bool is_function_v<Ret(Args...)> = true;

/// ����TΪ����void������������������κ�����ʱ�����档
template <typename T>
inline constexpr bool is_object_v = !(is_any_of_v<T, void> || is_function_v<T> || is_reference_v<T>);

/// ����TΪԭ������ʱ�����档
template <typename T>
inline constexpr bool is_array_v = false;

template <typename T, size_t N>
inline constexpr bool is_array_v<T[N]> = true;

template <typename T>
inline constexpr bool is_array_v<T[]> = true;

/// ����T�����Ĭ�Ϲ���ʱ�����档
template <typename T>
inline constexpr bool is_default_constructible_v = is_constructible_v<T>;

/// ����T����ɿ�������ʱ�����档
template <typename T>
inline constexpr bool is_copy_constructible_v = is_constructible_v<T, const T&>;

/// ����T������ƶ�����ʱ�����档
template <typename T>
inline constexpr bool is_move_constructible_v = is_constructible_v<T, T&&>;

/// ���ض�Ӧ�ķ��������͡�
template <typename T>
struct remove_reference {
    using type = T;
};

// ����ֵ�����ػ�
template <typename T>
struct remove_reference<T&> {
    using type = T;
};

// ����ֵ���ã��˴������������ã��ػ�
template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = remove_reference<T>::type;

/// ���ض�Ӧ����ֵ�������͡����������۵�����
template <typename T>
using add_lvalue_reference_t = T&;

/// ���ض�Ӧ����ֵ�������͡����������۵�����
template <typename T>
using add_rvalue_reference_t = T&&;

/// ���ض�Ӧ�ķ��������͵�ָ�롣
template <typename T>
using add_pointer_t = remove_reference_t<T>*;

/// ��������T��ά�ȣ��������鷵��0��
template <typename T>
inline constexpr size_t rank_v = 0;

template <typename T>
inline constexpr size_t rank_v<T[]> = rank_v<T> + 1;

template <typename T, size_t N>
inline constexpr size_t rank_v<T[N]> = rank_v<T> + 1;

/// ��������T�ڵ�I��ά���ϵĴ�С��
template <typename T, size_t I>
inline constexpr size_t extent_v = 0;

template <typename T, size_t I>
inline constexpr size_t extent_v<T[], I> = extent_v<T, I - 1>;

template <typename T, size_t I, size_t N>
inline constexpr size_t extent_v<T[N], I> = extent_v<T, I - 1>;

template <typename T, size_t N>
inline constexpr size_t extent_v<T[N], 0> = N;

/// ��ȡ����T��Ԫ�ص����͡��������鷵���䱾��
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

/// �� B == true ʱ���� T�����򷵻� F��
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

/// ����T���˻����͡�
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