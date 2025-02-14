#include "co_yantest.hpp"
#include "yan_type_traits.hpp"
#include <type_traits>
#include <array>

//#define USE_STD

#ifdef USE_STD
#define NAMESPACE_MY ::std::
#else
#define NAMESPACE_MY ::my::
#endif

namespace my
{
    namespace test
    {

class empty {};
class intw : public empty { int i; };
class functor { void operator()() {} };
union u { int i; float f; };
enum class e : size_t { TEST };

case_t is_void_v()
{
    co_yield{  NAMESPACE_MY is_void_v<void>, std::format("`is_void_v<void>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_void_v<const void>, std::format("`is_void_v<const void>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_void_v<volatile void>, std::format("`is_void_v<volatile void>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_void_v<const volatile void>, std::format("`is_void_v<const volatile void>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_void_v<int>, std::format("`is_void_v<int>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<float>, std::format("`is_void_v<float>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<empty>, std::format("input an empty class into `is_void_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<intw>, std::format("input a class into `is_void_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<u>, std::format("input a union into `is_void_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<e>, std::format("input an enum class into `is_void_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<int*>, std::format("input a pointer into `is_void_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<int&>, std::format("input a reference into `is_void_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_void_v<void(int)>, std::format("input a function type into `is_void_v` should return `false`, but it actually returns `true`.")};
    co_return;
};

case_t is_integral_v()
{
    co_yield{ !NAMESPACE_MY is_integral_v<void>, std::format("`is_integral_v<void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<const void>, std::format("`is_integral_v<const void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<volatile long double&>, std::format("`is_integral_v<volatile long double>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<const volatile double>, std::format("`is_integral_v<const volatile double>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<float>, std::format("`is_integral_v<float>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<empty>, std::format("input an empty class into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<intw>, std::format("input a class into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<u>, std::format("input a union into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<e>, std::format("input an enum class into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<int*>, std::format("input a pointer into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<int&>, std::format("input a reference into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_integral_v<int(int)>, std::format("input a function type into `is_integral_v` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_integral_v<int>, std::format("`is_integral_v<int>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<bool>, std::format("`is_integral_v<bool>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const char>, std::format("`is_integral_v<const char>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<volatile signed char>, std::format("`is_integral_v<volatile signed char>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const volatile unsigned char>, std::format("`is_integral_v<const volatile unsigned char>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<wchar_t>, std::format("`is_integral_v<wchar_t>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const char8_t>, std::format("`is_integral_v<const char8_t>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<volatile char16_t>, std::format("`is_integral_v<volatile char16_t>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const volatile char32_t>, std::format("`is_integral_v<const volatile char32_t>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<short>, std::format("`is_integral_v<short>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const unsigned short>, std::format("`is_integral_v<const unsigned short>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<volatile unsigned>, std::format("`is_integral_v<volatile unsigned>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const volatile long>, std::format("`is_integral_v<const volatile long>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<long long>, std::format("`is_integral_v<long long>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_integral_v<const unsigned long long>, std::format("`is_integral_v<const unsigned long long>` should return `true`, but it actually returns `false`.") };
}

case_t is_volatile_v()
{
    co_yield{ !NAMESPACE_MY is_volatile_v<void>, std::format("`is_volatile_v<void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_volatile_v<const void>, std::format("`is_volatile_v<const void>` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_volatile_v<volatile void>, std::format("`is_volatile_v<volatile void>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_volatile_v<const volatile void>, std::format("`is_volatile_v<const volatile void>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_volatile_v<int>, std::format("`is_volatile_v<int>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_volatile_v<const float>, std::format("`is_volatile_v<const float>` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_volatile_v<volatile empty>, std::format("input an volatile class into `is_volatile_v` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_volatile_v<intw>, std::format("input a class into `is_volatile_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_volatile_v<const u>, std::format("input a const union into `is_volatile_v` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_volatile_v<volatile e>, std::format("input a volatile enum class into `is_volatile_v` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_volatile_v<int* volatile>, std::format("input a volatile pointer type to a value into `is_volatile_v` should return `true`, but it actually returns `false`.") };
    //co_yield{ !NAMESPACE_MY is_volatile_v<int& volatile>, std::format("input a volatile reference type into `is_volatile_v` should return `false` because a reference cannot be modified as volatile.") };
    co_yield{ !NAMESPACE_MY is_volatile_v<void(int) volatile>, std::format("input a function type into `is_volatile_v` should return `false`, because a function cannot be modified as volatile.") };
    co_yield{  NAMESPACE_MY is_volatile_v<void(* volatile)(int)>, std::format("input a volatile pointer type to a function into `is_volatile_v` should return `true`, but it actually returns `false`.") };
}

case_t is_pointer_v()
{
    co_yield{ !NAMESPACE_MY is_pointer_v<void>, std::format("`is_pointer_v<void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<const void>, std::format("`is_pointer_v<const void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<volatile void>, std::format("`is_pointer_v<volatile void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<const volatile void>, std::format("`is_pointer_v<const volatile void>` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_pointer_v<void*>, std::format("`is_pointer_v<void*>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_pointer_v<const void*>, std::format("`is_pointer_v<const void*>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_pointer_v<volatile void*>, std::format("`is_pointer_v<volatile void*>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_pointer_v<const volatile void*>, std::format("`is_pointer_v<const volatile void*>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<int>, std::format("`is_pointer_v<int>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<float>, std::format("`is_pointer_v<float>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<empty>, std::format("input an empty class into `is_pointer_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<intw>, std::format("input a class into `is_pointer_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<u>, std::format("input a union into `is_pointer_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<e>, std::format("input an enum class into `is_pointer_v` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_pointer_v<empty*>, std::format("input a pointer into `is_pointer_v` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_pointer_v<int* const>, std::format("input a const pointer into `is_pointer_v` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<int&>, std::format("input a reference into `is_pointer_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_pointer_v<void(int)>, std::format("input a function type into `is_pointer_v` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_volatile_v<void(* volatile)(int)>, std::format("input a volatile pointer type to a function into `is_volatile_v` should return `true`, but it actually returns `false`.") };
}

my::test::case_t is_reference_v()
{
    co_yield{ !NAMESPACE_MY is_reference_v<void>, std::format("`is_reference_v<void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<const void>, std::format("`is_reference_v<const void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<volatile void>, std::format("`is_reference_v<volatile void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<const volatile void>, std::format("`is_reference_v<const volatile void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<int>, std::format("`is_reference_v<int>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<float>, std::format("`is_reference_v<float>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<empty>, std::format("input an empty class into `is_reference_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<intw>, std::format("input a class into `is_reference_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<u>, std::format("input a union into `is_reference_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<e>, std::format("input an enum class into `is_reference_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<int*>, std::format("input a pointer into `is_reference_v` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_reference_v<u&>, std::format("input a lvalue reference into `is_reference_v` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_reference_v<const e&>, std::format("input a const lvalue reference into `is_reference_v` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_reference_v<int&&>, std::format("input a rvalue reference into `is_reference_v` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_reference_v<void(int)>, std::format("input a function type into `is_reference_v` should return `false`, but it actually returns `true`.") };
    co_return;
};

my::test::case_t is_function_v()
{
    co_yield{  NAMESPACE_MY is_function_v<void(int)>, std::format("`is_function_v<void(int)>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_function_v<int(float, char)>, std::format("`is_function_v<int(float, char)>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_function_v<int>, std::format("`is_function_v<int>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_function_v<void*>, std::format("`is_function_v<void*>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_function_v<int&>, std::format("`is_function_v<int&>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_function_v<functor>, std::format("input a functor into `is_function_v<int&>` should return `false`, but it actually returns `true`.") };
    co_return;
}

my::test::case_t is_object_v()
{
    co_yield{ !NAMESPACE_MY is_object_v<void>, std::format("`is_object_v<void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_object_v<void(int)>, std::format("`is_object_v<void(int)>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_object_v<int&>, std::format("input an reference into `is_object_v` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_object_v<int>, std::format("`is_object_v<int>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_object_v<float*>, std::format("`is_object_v<float*>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_object_v<const u>, std::format("input a const union should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_object_v<e>, std::format("input an enum class into `is_object_v` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_object_v<empty>, std::format("input an empty class into `is_object_v` should return `true`, but it actually returns `false`.") };
    co_return;
}

my::test::case_t is_array_v()
{
    co_yield{  NAMESPACE_MY is_array_v<int[10]>, std::format("`is_array_v<int[10]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<const int[10]>, std::format("`is_array_v<const int[10]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<volatile int[10]>, std::format("`is_array_v<volatile int[10]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<const volatile int[10]>, std::format("`is_array_v<const volatile int[10]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<int[]>, std::format("`is_array_v<int[]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<const int[]>, std::format("`is_array_v<const int[]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<volatile int[]>, std::format("`is_array_v<volatile int[]>` should return `true`, but it actually returns `false`.") };
    co_yield{  NAMESPACE_MY is_array_v<const volatile int[]>, std::format("`is_array_v<const volatile int[]>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_array_v<int>, std::format("`is_array_v<int>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<int*>, std::format("`is_array_v<int*>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<int&>, std::format("`is_array_v<int&>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<int&&>, std::format("`is_array_v<int&&>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<float>, std::format("`is_array_v<float>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<empty>, std::format("input an empty class into `is_array_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<intw>, std::format("input a class into `is_array_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<u>, std::format("input a union into `is_array_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<e>, std::format("input an enum class into `is_array_v` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<void>, std::format("`is_array_v<void>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_array_v<std::array<int,3>>, std::format("`is_array_v<std::array<int,3>>` should return `false`, because it is not a built-in array.") };
    co_return;
}

class nodefault
{
public:
    nodefault() = delete;
    nodefault(const nodefault&) = default;
    nodefault& operator=(const nodefault&) = default;
    nodefault(nodefault&&) = default;
    nodefault& operator=(nodefault&&) = default;
};
class noncopyable
{
public:
    noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
    noncopyable(noncopyable&&) = default;
    noncopyable& operator=(noncopyable&&) = default;
};
class nonmoveable 
{
public:
    nonmoveable() = default;
    nonmoveable(const nonmoveable&) = default;
    nonmoveable& operator=(const nonmoveable&) = default;
    nonmoveable(nonmoveable&&) = delete;
    nonmoveable& operator=(nonmoveable&&) = delete;
};
class non : public nodefault, public noncopyable, public nonmoveable {};

my::test::case_t is_default_constructible_v()
{
    co_yield{ !NAMESPACE_MY is_default_constructible_v<nodefault>, std::format("`is_default_constructible_v<nodefault>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_copy_constructible_v<noncopyable>, std::format("`is_default_constructible_v<noncopyable>` should return `false`, but it actually returns `true`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<nonmoveable>, std::format("`is_default_constructible_v<nonmoveable>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_default_constructible_v<non>, std::format("`is_default_constructible_v<non>` should return `false`, but it actually returns `true`.") };
    co_yield{ NAMESPACE_MY is_default_constructible_v<int>, std::format("`is_default_constructible_v<int>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_default_constructible_v<float>, std::format("`is_default_constructible_v<float>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_default_constructible_v<empty>, std::format("`is_default_constructible_v<empty>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_default_constructible_v<intw>, std::format("`is_default_constructible_v<intw>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_default_constructible_v<u>, std::format("`is_default_constructible_v<u>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_default_constructible_v<e>, std::format("`is_default_constructible_v<e>` should return `true`, but it actually returns `false`.") };
    co_return;
}

my::test::case_t is_copy_constructible_v()
{
    co_yield{ NAMESPACE_MY is_copy_constructible_v<nodefault>, std::format("`is_copy_constructible_v<nodefault>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_copy_constructible_v<noncopyable>, std::format("`is_copy_constructible_v<noncopyable>` should return `false`, but it actually returns `true`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<nonmoveable>, std::format("`is_copy_constructible_v<nonmoveable>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_copy_constructible_v<non>, std::format("`is_copy_constructible_v<non>` should return `false`, but it actually returns `true`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<int>, std::format("`is_copy_constructible_v<int>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<float>, std::format("`is_copy_constructible_v<float>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<empty>, std::format("`is_copy_constructible_v<empty>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<intw>, std::format("`is_copy_constructible_v<intw>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<u>, std::format("`is_copy_constructible_v<u>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_copy_constructible_v<e>, std::format("`is_copy_constructible_v<e>` should return `true`, but it actually returns `false`.") };
    co_return;
}

my::test::case_t is_move_constructible_v()
{
    co_yield{ NAMESPACE_MY is_copy_constructible_v<nodefault>, std::format("`is_move_constructible_v<nodefault>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_copy_constructible_v<noncopyable>, std::format("`is_move_constructible_v<noncopyable>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_move_constructible_v<nonmoveable>, std::format("`is_move_constructible_v<nonmoveable>` should return `false`, but it actually returns `true`.") };
    co_yield{ !NAMESPACE_MY is_move_constructible_v<non>, std::format("`is_move_constructible_v<non>` should return `false`, but it actually returns `true`.") };
    co_yield{ NAMESPACE_MY is_move_constructible_v<int>, std::format("`is_move_constructible_v<int>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_move_constructible_v<float>, std::format("`is_move_constructible_v<float>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_move_constructible_v<empty>, std::format("`is_move_constructible_v<empty>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_move_constructible_v<intw>, std::format("`is_move_constructible_v<intw>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_move_constructible_v<u>, std::format("`is_move_constructible_v<u>` should return `true`, but it actually returns `false`.") };
    co_yield{ NAMESPACE_MY is_move_constructible_v<e>, std::format("`is_move_constructible_v<e>` should return `true`, but it actually returns `false`.") };
    co_return;
}

my::test::case_t is_same_v()
{
    co_yield{ NAMESPACE_MY is_same_v<int, int>, std::format("`is_same_v<int, int>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_same_v<int, float>, std::format("`is_same_v<int, float>` should return `false`, but it actually returns `true`.") };
    co_yield{ NAMESPACE_MY is_same_v<const int, const int>, std::format("`is_same_v<const int, const int>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_same_v<int, const int>, std::format("`is_same_v<int, const int>` should return `false`, but it actually returns `true`.") };
    co_return;
}

my::test::case_t is_any_of_v()
{
#ifndef USE_STD
    co_yield{  NAMESPACE_MY is_any_of_v<int, int, float, double>, std::format("`is_any_of_v<int, int, float, double>` should return `true`, but it actually returns `false`.") };
    co_yield{ !NAMESPACE_MY is_any_of_v<char, int, float, double>, std::format("`is_any_of_v<char, int, float, double>` should return `false`, but it actually returns `true`.") };
    co_yield{  NAMESPACE_MY is_any_of_v<const int, const int, int, float>, std::format("`is_any_of_v<const int, const int, int, float>` should return `true`, but it actually returns `false`.") };
#endif
    co_return;
}

my::test::case_t add_const_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY add_const_t<int>, const int>, std::format("`add_const_t<int>` should be `const int`, but it is actually `{}`.", typename_<NAMESPACE_MY add_const_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_const_t<const int>, const int>, std::format("`add_const_t<const int>` should be `const int`, but it is actually `{}`.", typename_<NAMESPACE_MY add_const_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_const_t<volatile int>, const volatile int>, std::format("`add_const_t<volatile int>` should be `const volatile int`, but it is actually `{}`.", typename_<NAMESPACE_MY add_const_t<volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_const_t<int*>, int* const>, std::format("`add_const_t<int*>` should be `int* const`, but it is actually `{}`.", typename_<NAMESPACE_MY add_const_t<int*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_const_t<int&>, int&>, std::format("`add_const_t<int&>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_const_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_const_t<int&&>, int&&>, std::format("`add_const_t<int&&>` should be `int&&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_const_t<int&&>>()) };
    co_return;
}

my::test::case_t remove_const_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY remove_const_t<const int>, int>, std::format("`remove_const_t<const int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_const_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_const_t<const volatile int>, volatile int>, std::format("`remove_const_t<const volatile int>` should be `volatile int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_const_t<const volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_const_t<int>, int>, std::format("`remove_const_t<int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_const_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_const_t<int* const>, int*>, std::format("`remove_const_t<int* const>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_const_t<int* const>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_const_t<int&>, int&>, std::format("`remove_const_t<int&>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_const_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_const_t<const int&>, const int&>, std::format("`remove_const_t<const int&>` should be `const int&`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_const_t<const int&>>()) };
    co_return;
}

my::test::case_t remove_cv_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<const int>, int>, std::format("`remove_cv_t<const int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<volatile int>, int>, std::format("`remove_cv_t<volatile int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<const volatile int>, int>, std::format("`remove_cv_t<const volatile int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<const volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<int>, int>, std::format("`remove_cv_t<int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<int* const volatile>, int*>, std::format("`remove_cv_t<int* const volatile>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<int* const volatile>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<int&>, int&>, std::format("`remove_cv_t<int&>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_cv_t<const int&>, const int&>, std::format("`remove_cv_t<const int&>` should be `const int&`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_cv_t<const int&>>()) };
    co_return;
}

my::test::case_t remove_reference_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<int&>, int>, std::format("`remove_reference_t<int&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<const int&>, const int>, std::format("`remove_reference_t<const int&>` should be `const int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<const int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<int&&>, int>, std::format("`remove_reference_t<int&&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<int&&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<const int&&>, const int>, std::format("`remove_reference_t<const int&&>` should be `const int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<const int&&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<int>, int>, std::format("`remove_reference_t<int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<int*>, int*>, std::format("`remove_reference_t<int*>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<int*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_reference_t<int* const&>, int* const>, std::format("`remove_reference_t<int* const&>` should be `int* const`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_reference_t<int* const&>>()) };
    co_return;
}

my::test::case_t add_lvalue_reference_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<int>, int&>, std::format("`add_lvalue_reference_t<int>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<const int>, const int&>, std::format("`add_lvalue_reference_t<const int>` should be `const int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<volatile int>, volatile int&>, std::format("`add_lvalue_reference_t<volatile int>` should be `volatile int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<const volatile int>, const volatile int&>, std::format("`add_lvalue_reference_t<const volatile int>` should be `const volatile int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<const volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<int&>, int&>, std::format("`add_lvalue_reference_t<int&>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<const int&>, const int&>, std::format("`add_lvalue_reference_t<const int&>` should be `const int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<const int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<volatile int&>, volatile int&>, std::format("`add_lvalue_reference_t<volatile int&>` should be `volatile int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<volatile int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_lvalue_reference_t<const volatile int&>, const volatile int&>, std::format("`add_lvalue_reference_t<const volatile int&>` should be `const volatile int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_lvalue_reference_t<const volatile int&>>()) };
    co_return;
}

my::test::case_t add_rvalue_reference_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<int>, int&&>, std::format("`add_rvalue_reference_t<int>` should be `int&&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<const int>, const int&&>, std::format("`add_rvalue_reference_t<const int>` should be `const int&&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<volatile int>, volatile int&&>, std::format("`add_rvalue_reference_t<volatile int>` should be `volatile int&&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<const volatile int>, const volatile int&&>, std::format("`add_rvalue_reference_t<const volatile int>` should be `const volatile int&&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<const volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<int&>, int&>, std::format("`add_rvalue_reference_t<int&>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<const int&>, const int&>, std::format("`add_rvalue_reference_t<const int&>` should be `const int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<const int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<volatile int&>, volatile int&>, std::format("`add_rvalue_reference_t<volatile int&>` should be `volatile int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<volatile int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_rvalue_reference_t<const volatile int&>, const volatile int&>, std::format("`add_rvalue_reference_t<const volatile int&>` should be `const volatile int&`, but it is actually `{}`.", typename_<NAMESPACE_MY add_rvalue_reference_t<const volatile int&>>()) };
    co_return;
}

my::test::case_t add_pointer_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<int>, int*>, std::format("`add_pointer_t<int>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<const int>, const int*>, std::format("`add_pointer_t<const int>` should be `const int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<volatile int>, volatile int*>, std::format("`add_pointer_t<volatile int>` should be `volatile int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<const volatile int>, const volatile int*>, std::format("`add_pointer_t<const volatile int>` should be `const volatile int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<const volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<int&>, int*>, std::format("`add_pointer_t<int&>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<const int&>, const int*>, std::format("`add_pointer_t<const int&>` should be `const int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<const int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<volatile int&>, volatile int*>, std::format("`add_pointer_t<volatile int&>` should be `volatile int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<volatile int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY add_pointer_t<const volatile int&>, const volatile int*>, std::format("`add_pointer_t<const volatile int&>` should be `const volatile int*`, but it is actually `{}`.", typename_<NAMESPACE_MY add_pointer_t<const volatile int&>>()) };
    co_return;
}

my::test::case_t rank_v()
{
    co_yield{ NAMESPACE_MY rank_v<int> == 0, std::format("`rank_v<int>` should be 0, but it is actually `{}`.", NAMESPACE_MY rank_v<int>) };
    co_yield{ NAMESPACE_MY rank_v<int[]> == 1, std::format("`rank_v<int[]>` should be 1, but it is actually `{}`.", NAMESPACE_MY rank_v<int[]>) };
    co_yield{ NAMESPACE_MY rank_v<int[10]> == 1, std::format("`rank_v<int[10]>` should be 1, but it is actually `{}`.", NAMESPACE_MY rank_v<int[10]>) };
    co_yield{ NAMESPACE_MY rank_v<int[][5]> == 2, std::format("`rank_v<int[][5]>` should be 2, but it is actually `{}`.", NAMESPACE_MY rank_v<int[][5]>) };
    co_yield{ NAMESPACE_MY rank_v<int[10][5]> == 2, std::format("`rank_v<int[10][5]>` should be 2, but it is actually `{}`.", NAMESPACE_MY rank_v<int[10][5]>) };
    co_yield{ NAMESPACE_MY rank_v<int[2][3][4][5][6]> == 5, std::format("`rank_v<int[10][5]>` should be 2, but it is actually `{}`.", NAMESPACE_MY rank_v<int[2][3][4][5][6]>) };
    co_yield{ NAMESPACE_MY rank_v<const int[]> == 1, std::format("`rank_v<const int[]>` should be 1, but it is actually `{}`.", NAMESPACE_MY rank_v<const int[]>) };
    co_return;
}

my::test::case_t extent_v()
{
    co_yield{ NAMESPACE_MY extent_v<int, 0> == 0, std::format("`extent_v<int, 0>` should be 0, but it is actually `{}`.", NAMESPACE_MY extent_v<int, 0>) };
    co_yield{ NAMESPACE_MY extent_v<int[10], 0> == 10, std::format("`extent_v<int[10], 0>` should be 10, but it is actually `{}`.", NAMESPACE_MY extent_v<int[10], 0>) };
    co_yield{ NAMESPACE_MY extent_v<int[][5], 1> == 5, std::format("`extent_v<int[][5], 1>` should be 5, but it is actually `{}`.", NAMESPACE_MY extent_v<int[][5], 1>) };
    co_yield{ NAMESPACE_MY extent_v<int[10][5], 1> == 5, std::format("`extent_v<int[10][5], 1>` should be 5, but it is actually `{}`.", NAMESPACE_MY extent_v<int[10][5], 1>) };
    co_yield{ NAMESPACE_MY extent_v<int[10][5], 0> == 10, std::format("`extent_v<int[10][5], 0>` should be 10, but it is actually `{}`.", NAMESPACE_MY extent_v<int[10][5], 0>) };
    co_yield{ NAMESPACE_MY extent_v<const int[10], 0> == 10, std::format("`extent_v<const int[10], 0>` should be 10, but it is actually `{}`.", NAMESPACE_MY extent_v<const int[10], 0>) };
    co_yield{ NAMESPACE_MY extent_v<const int[2][3][4][5][6], 3> == 5, std::format("`extent_v<const int[10], 0>` should be 10, but it is actually `{}`.", NAMESPACE_MY extent_v<const int[2][3][4][5][6], 3>) };
    co_yield{ NAMESPACE_MY extent_v<int[][5], 0> == 0, std::format("`extent_v<int[][5], 0>` should be 0, but it is actually `{}`.", NAMESPACE_MY extent_v<int[][5], 0>) };
    co_return;
}

my::test::case_t remove_extent_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<int>, int>, std::format("`remove_extent_t<int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<int[10]>, int>, std::format("`remove_extent_t<int[10]>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<int[10]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<int[]> , int>, std::format("`remove_extent_t<int[]>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<int[]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<int[10][5]>, int[5]>, std::format("`remove_extent_t<int[10][5]>` should be `int[5]`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<int[10][5]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<const int[10]>, const int>, std::format("`remove_extent_t<const int[10]>` should be `const int`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<const int[10]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<int[2][3][4]>, int[3][4]>, std::format("`remove_extent_t<int[2][3][4]>` should be `int[3][4]`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<int[2][3][4]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY remove_extent_t<int[][5]>, int[5]>, std::format("`remove_extent_t<int[][5]>` should be `int[5]`, but it is actually `{}`.", typename_<NAMESPACE_MY remove_extent_t<int[][5]>>()) };
    co_return;
}

my::test::case_t conditional_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<true, int, double>, int>, std::format("`conditional_t<true, int, double>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<true, int, double>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<false, int, double>, double>, std::format("`conditional_t<false, int, double>` should be `double`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<false, int, double>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<true, const int, double>, const int>, std::format("`conditional_t<true, const int, double>` should be `const int`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<true, const int, double>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<false, int, const double>, const double>, std::format("`conditional_t<false, int, const double>` should be `const double`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<false, int, const double>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<true, int&, double&>, int&>, std::format("`conditional_t<true, int&, double&>` should be `int&`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<true, int&, double&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<false, int&, double&>, double&>, std::format("`conditional_t<false, int&, double&>` should be `double&`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<false, int&, double&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<true, const int&, volatile double&>, const int&>, std::format("`conditional_t<true, const int&, volatile double&>` should be `const int&`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<true, const int&, volatile double&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<false, int&&, double&&>, double&&>, std::format("`conditional_t<false, int&&, double&&>` should be `double&&`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<false, int&&, double&&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<true, const int*, volatile double*>, const int*>, std::format("`conditional_t<true, const int*, volatile double*>` should be `const int*`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<true, const int*, volatile double*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY conditional_t<false, int*, double*>, double*>, std::format("`conditional_t<false, int*, double*>` should be `double*`, but it is actually `{}`.", typename_<NAMESPACE_MY conditional_t<false, int*, double*>>()) };
    co_return;
}

my::test::case_t decay_t()
{
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int>, int>, std::format("`decay_t<int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const int>, int>, std::format("`decay_t<const int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<volatile int>, int>, std::format("`decay_t<volatile int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const volatile int>, int>, std::format("`decay_t<const volatile int>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const volatile int>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int[]>, int*>, std::format("`decay_t<int[]>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int[]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int[5]>, int*>, std::format("`decay_t<int[5]>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int[5]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int()>, int(*)()>, std::format("`decay_t<int()()>` should be `int(*)()`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int(*)()>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int(&)[5]>, int*>, std::format("`decay_t<int(&)[5]>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int(&)[5]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int(&&)[5]>, int*>, std::format("`decay_t<int(&&)[5]>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int(&&)[5]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const int(&)[5]>, const int*>, std::format("`decay_t<const int(&)[5]>` should be `const int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const int(&)[5]>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int*>, int*>, std::format("`decay_t<int*>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const int*>, const int*>, std::format("`decay_t<const int*>` should be `const int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const int*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<volatile int*>, volatile int*>, std::format("`decay_t<volatile int*>` should be `volatile int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<volatile int*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const volatile int*>, const volatile int*>, std::format("`decay_t<const volatile int*>` should be `const volatile int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const volatile int*>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int* const>, int*>, std::format("`decay_t<int* const>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int* const>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int* volatile>, int*>, std::format("`decay_t<int* volatile>` should be `int*`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int* volatile>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int* const volatile>, int*>, std::format("`decay_t<int*>` should be `int* const volatile`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int* const volatile>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int&>, int>, std::format("`decay_t<int&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const int&>, int>, std::format("`decay_t<const int&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<volatile int&>, int>, std::format("`decay_t<volatile int&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<volatile int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const volatile int&>, int>, std::format("`decay_t<const volatile int&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const volatile int&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<int&&>, int>, std::format("`decay_t<int&&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<int&&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const int&&>, int>, std::format("`decay_t<const int&&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const int&&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<volatile int&&>, int>, std::format("`decay_t<volatile int&&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<volatile int&&>>()) };
    co_yield{ std::is_same_v<NAMESPACE_MY decay_t<const volatile int&&>, int>, std::format("`decay_t<const volatile int&&>` should be `int`, but it is actually `{}`.", typename_<NAMESPACE_MY decay_t<const volatile int&&>>()) };

    co_return;
}

    }
}


int main()
{
    using namespace my;
    auto t = test::test();
    t.new_case(test::is_void_v(), "is_void_v");
    t.new_case(test::is_integral_v(), "is_integral_v");
    t.new_case(test::is_volatile_v(), "is_volatile_v");
    t.new_case(test::is_pointer_v(), "is_pointer_v");
    t.new_case(test::is_reference_v(), "is_reference_v");
    t.new_case(test::is_function_v(), "is_function_v");
    t.new_case(test::is_object_v(), "is_object_v");
    t.new_case(test::is_array_v(), "is_array_v");
    t.new_case(test::is_default_constructible_v(), "is_default_constructible_v");
    t.new_case(test::is_copy_constructible_v(), "is_copy_constructible_v");
    t.new_case(test::is_move_constructible_v(), "is_move_constructible_v");
    t.new_case(test::is_same_v(), "is_same_v");
    t.new_case(test::is_any_of_v(), "is_any_of_v");
    t.new_case(test::add_const_t(), "add_const_t");
    t.new_case(test::remove_const_t(), "remove_const_t");
    t.new_case(test::remove_cv_t(), "remove_cv_t");
    t.new_case(test::remove_reference_t(), "remove_reference_t");
    t.new_case(test::add_lvalue_reference_t(), "add_lvalue_reference_t");
    t.new_case(test::add_rvalue_reference_t(), "add_rvalue_reference_t");
    t.new_case(test::add_pointer_t(), "add_pointer_t");
    t.new_case(test::rank_v(), "rank_v");
    t.new_case(test::extent_v(), "extent_v");
    t.new_case(test::remove_extent_t(), "remove_extent_t");
    t.new_case(test::conditional_t(), "conditional_t");
    t.new_case(test::decay_t(), "decay_t");
}