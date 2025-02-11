#include "co_yantest.hpp"
#include "int_wrapper.hpp"
#include "yan_allocator.hpp"
#include <memory>

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

auto& ap = _alloc_proxy::get_instance();

template <typename T>
class shit_allocator
{
public:
    union u { int a; };
    enum class e {};
    using c = int;
    using value_type = std::add_cv_t<T>;
    using pointer = u*;
    using const_pointer = const e*;
    using void_pointer = T*;
    using const_void_pointer = volatile T*;
    using size_type = float;
    using difference_type = c;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    template <typename U> struct rebind { using other = shit_allocator<int>; };

    shit_allocator() = default;
    template <typename U>
    shit_allocator(const shit_allocator<U>&) {}
    pointer allocate(size_type n) { throw 1; }
    pointer allocate(size_type n, const void* hint) { throw 2; }
    void deallocate(pointer p, size_type n) { throw 3; }
    template <typename U, typename ...Args>
    void construct(U* p, Args&& ...args) { throw 4; }
    template <typename U>
    void destroy(U* p) { throw 5; }
    size_type max_size() const { return .0f; }
    shit_allocator<T> select_on_container_copy_construction() const { throw 7; }

    bool operator==(const shit_allocator& other) const noexcept { return false; }
};

template <typename T>
class shit_allocator_2
{
public:
    using value_type = T;
    using size_type = size_t;

    shit_allocator_2() = default;
    template <typename U> shit_allocator_2(const shit_allocator<U>&) {}
    T* allocate(size_type n) { return reinterpret_cast<T*>(n); }
    template <typename U> bool operator==(const shit_allocator_2<U>& other) const noexcept { return false; }
};

case_t alloc_proxy()
{
#ifndef USE_STD
    ap.reset_uncheck();
    co_yield "allocate(10)";
    void* p1 = ap.allocate(10);
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 10, std::format("alloc_proxy's current_allocated_bytes should be `10`, but it actually is `{}`", ap.current_allocated_bytes) };
    co_yield{ ap.total_allocated_bytes == 10, std::format("alloc_proxy's total_allocated_bytes should be `10`, but it actually is `{}`", ap.total_allocated_bytes) };
    co_yield{ ap.total_allocations == 1, std::format("alloc_proxy's total_allocations should be `1`, but it actually is `{}`", ap.total_allocations) };

    co_yield "allocate(20)";
    void* p2 = ap.allocate(20);
    co_yield{ ap.current_allocations == 2, std::format("alloc_proxy's current_allocations should be `2`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 30, std::format("alloc_proxy's current_allocated_bytes should be `30`, but it actually is `{}`", ap.current_allocated_bytes) };
    co_yield{ ap.total_allocated_bytes == 30, std::format("alloc_proxy's total_allocated_bytes should be `30`, but it actually is `{}`", ap.total_allocated_bytes) };
    co_yield{ ap.total_allocations == 2, std::format("alloc_proxy's total_allocations should be `2`, but it actually is `{}`", ap.total_allocations) };

    co_yield "deallocate(20)";
    ap.deallocate(p2, 20);
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 10, std::format("alloc_proxy's current_allocated_bytes should be `10`, but it actually is `{}`", ap.current_allocated_bytes) };

    co_yield "deallocate(10)";
    ap.deallocate(p1, 10);
    co_yield{ ap.current_allocations == 0, std::format("alloc_proxy's current_allocations should be `0`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 0, std::format("alloc_proxy's current_allocated_bytes should be `0`, but it actually is `{}`", ap.current_allocated_bytes) };

    co_yield "allocate(15)";
    void* p3 = ap.allocate(15);
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 15, std::format("alloc_proxy's current_allocated_bytes should be `15`, but it actually is `{}`", ap.current_allocated_bytes) };
    co_yield{ ap.total_allocated_bytes == 45, std::format("alloc_proxy's total_allocated_bytes should be `45`, but it actually is `{}`", ap.total_allocated_bytes) };
    co_yield{ ap.total_allocations == 3, std::format("alloc_proxy's total_allocations should be `3`, but it actually is `{}`", ap.total_allocations) };

    co_yield "deallocate(15) and reset";
    ap.deallocate(p3, 15);
    ap.reset();
    co_yield{ ap.current_allocations == 0, std::format("alloc_proxy's current_allocations should be `0`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 0, std::format("alloc_proxy's current_allocated_bytes should be `0`, but it actually is `{}`", ap.current_allocated_bytes) };
    co_yield{ ap.total_allocated_bytes == 0, std::format("alloc_proxy's total_allocated_bytes should be `0`, but it actually is `{}`", ap.total_allocated_bytes) };
    co_yield{ ap.total_allocations == 0, std::format("alloc_proxy's total_allocations should be `0`, but it actually is `{}`", ap.total_allocations) };

    co_yield nullptr;
    // -------------------------

    co_yield "allocate(10)";
    void* p4 = ap.allocate(10);
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 10, std::format("alloc_proxy's current_allocated_bytes should be `10`, but it actually is `{}`", ap.current_allocated_bytes) };

    co_yield "reset";
    try {
        ap.reset();
        co_yield{ false, "reset should have thrown memory_leak exception, but it did not" };
    }
    catch (const _alloc_proxy::memory_leak&) {}
    co_yield "deallocate(10) and reset";
    ap.deallocate(p4, 10);
    ap.reset();

#endif
    co_return;
}

case_t allocator_test()
{
#ifndef USE_STD
    allocator<int> alloc;
    ap.reset_uncheck();

    co_yield "allocate(10)";
    auto* p1 = alloc.allocate(10);
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 10 * sizeof(int), std::format("alloc_proxy's current_allocated_bytes should be `{}`, but it actually is `{}`", 10 * sizeof(int), ap.current_allocated_bytes) };

    co_yield "allocate_at_least(8)";
    auto [p2, size2] = alloc.allocate_at_least(8);
    co_yield{ size2 == 8, std::format("allocated size should be `8`, but it actually is `{}`", size2) };
    co_yield{ ap.current_allocations == 2, std::format("alloc_proxy's current_allocations should be `2`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 10 * sizeof(int) + 8 * sizeof(int), std::format("alloc_proxy's current_allocated_bytes should be `{}`, but it actually is `{}`", 10 * sizeof(int) + 8 * sizeof(int), ap.current_allocated_bytes) };

    co_yield "deallocate(10)";
    alloc.deallocate(p1, 10);
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 8 * sizeof(int), std::format("alloc_proxy's current_allocated_bytes should be `{}`, but it actually is `{}`", 8 * sizeof(int), ap.current_allocated_bytes) };

    co_yield "deallocate(allocate_at_least(5).size)";
    alloc.deallocate(p2, size2);
    co_yield{ ap.current_allocations == 0, std::format("alloc_proxy's current_allocations should be `0`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 0, std::format("alloc_proxy's current_allocated_bytes should be `0`, but it actually is `{}`", ap.current_allocated_bytes) };

    co_yield "reset alloc_proxy";
    ap.reset();

    co_yield nullptr;

    co_yield "operator== test";
    allocator<int> alloc1;
    allocator<int> alloc2;
    co_yield{ alloc1 == alloc2, "our allocators should be equal" };

    co_yield nullptr;
#endif
    co_return;
}

case_t allocator_traits_types()
{
    using T = NAMESPACE_MY allocator_traits<std::allocator<int>>;
    co_yield std::format("test with T = `{}`", typename_<T>());
    co_yield{ std::is_same_v<typename T::value_type, int>,
               std::format("T::value_type should be int, but is `{}`", typename_<typename T::value_type>()) };
    co_yield{ std::is_same_v<typename T::pointer, int*>,
               std::format("T::pointer should be int*, but is `{}`", typename_<typename T::pointer>()) };
    co_yield{ std::is_same_v<typename T::const_pointer, const int*>,
               std::format("T::const_pointer should be const int*, but is `{}`", typename_<typename T::const_pointer>()) };
    co_yield{ std::is_same_v<typename T::void_pointer, void*>,
               std::format("T::void_pointer should be void*, but is `{}`", typename_<typename T::void_pointer>()) };
    co_yield{ std::is_same_v<typename T::const_void_pointer, const void*>,
               std::format("T::const_void_pointer should be const void*, but is `{}`", typename_<typename T::const_void_pointer>()) };
    co_yield{ std::is_same_v<typename T::difference_type, std::ptrdiff_t>,
               std::format("T::difference_type should be std::ptrdiff_t, but is `{}`", typename_<typename T::difference_type>()) };
    co_yield{ std::is_same_v<typename T::size_type, std::size_t>,
               std::format("T::size_type should be std::size_t, but is `{}`", typename_<typename T::size_type>()) };
    co_yield{ std::is_same_v<typename T::propagate_on_container_copy_assignment, std::false_type>,
               std::format("T::propagate_on_container_copy_assignment should be std::false_type, but is `{}`", typename_<typename T::propagate_on_container_copy_assignment>()) };
    co_yield{ std::is_same_v<typename T::propagate_on_container_move_assignment, std::true_type>,
               std::format("T::propagate_on_container_move_assignment should be std::true_type, but is `{}`", typename_<typename T::propagate_on_container_move_assignment>()) };
    co_yield{ std::is_same_v<typename T::propagate_on_container_swap, std::false_type>,
               std::format("T::propagate_on_container_swap should be std::false_type, but is `{}`", typename_<typename T::propagate_on_container_swap>()) };
    co_yield{ std::is_same_v<typename T::is_always_equal, std::true_type>,
               std::format("T::is_always_equal should be std::true_type, but is `{}`", typename_<typename T::is_always_equal>()) };
    co_yield{ std::is_same_v<typename T::template rebind_alloc<double>, std::allocator<double>>,
               std::format("T::rebind_alloc<double> should be std::allocator<double>, but is `{}`", typename_<typename T::template rebind_alloc<double>>()) };
    co_yield{ std::is_same_v<typename T::template rebind_traits<double>, NAMESPACE_MY allocator_traits<std::allocator<double>>>,
               std::format("T::rebind_traits<double> should be my::allocator_traits<std::allocator<double>>, but is `{}`", typename_<NAMESPACE_MY allocator_traits<std::allocator<double>>>()) };
    co_yield nullptr;
    using U = std::allocator_traits<NAMESPACE_MY allocator<int>>;
    co_yield std::format("test with U = `{}`", typename_<U>());
    co_yield{ std::is_same_v<typename U::value_type, int>,
               std::format("U::value_type should be int, but is `{}`", typename_<typename U::value_type>()) };
    co_yield{ std::is_same_v<typename U::pointer, int*>,
               std::format("U::pointer should be int*, but is `{}`", typename_<typename U::pointer>()) };
    co_yield{ std::is_same_v<typename U::const_pointer, const int*>,
               std::format("U::const_pointer should be const int*, but is `{}`", typename_<typename U::const_pointer>()) };
    co_yield{ std::is_same_v<typename U::void_pointer, void*>,
               std::format("U::void_pointer should be void*, but is `{}`", typename_<typename U::void_pointer>()) };
    co_yield{ std::is_same_v<typename U::const_void_pointer, const void*>,
               std::format("U::const_void_pointer should be const void*, but is `{}`", typename_<typename U::const_void_pointer>()) };
    co_yield{ std::is_same_v<typename U::difference_type, std::ptrdiff_t>,
               std::format("U::difference_type should be std::ptrdiff_t, but is `{}`", typename_<typename U::difference_type>()) };
    co_yield{ std::is_same_v<typename U::size_type, std::size_t>,
               std::format("U::size_type should be std::size_t, but is `{}`", typename_<typename U::size_type>()) };
    co_yield{ std::is_same_v<typename U::propagate_on_container_copy_assignment, std::false_type>,
               std::format("U::propagate_on_container_copy_assignment should be std::false_type, but is `{}`", typename_<typename U::propagate_on_container_copy_assignment>()) };
    co_yield{ std::is_same_v<typename U::propagate_on_container_move_assignment, std::true_type>,
               std::format("U::propagate_on_container_move_assignment should be std::true_type, but is `{}`", typename_<typename U::propagate_on_container_move_assignment>()) };
    co_yield{ std::is_same_v<typename U::propagate_on_container_swap, std::false_type>,
               std::format("U::propagate_on_container_swap should be std::false_type, but is `{}`", typename_<typename U::propagate_on_container_swap>()) };
    co_yield{ std::is_same_v<typename U::is_always_equal, std::true_type>,
              std::format("U::is_always_equal should be std::true_type, but is `{}`", typename_<typename U::is_always_equal>()) };
    co_return;
}

case_t our_traits_for_user_defined_allocator()
{
    using T = NAMESPACE_MY allocator_traits<shit_allocator<int>>;
    std::string error_message = ": allocator_traits should use alloctor's defination or implementation if possible.";
    co_yield{ std::is_same_v<typename T::value_type, const volatile int>, "value_type" + error_message};
    co_yield{ std::is_same_v<typename T::pointer, shit_allocator<int>::u* > ,"pointer" + error_message };
    co_yield{ std::is_same_v<typename T::const_pointer, const shit_allocator<int>::e* > ,"const_pointer" + error_message };
    co_yield{ std::is_same_v<typename T::void_pointer, int*> ,"void_pointer" + error_message };
    co_yield{ std::is_same_v<typename T::const_void_pointer, volatile int*> ,"const_void_pointer" + error_message };
    co_yield{ std::is_same_v<typename T::difference_type, shit_allocator<int>::c>,"difference_type" + error_message };
    co_yield{ std::is_same_v<typename T::size_type, float> , "size_type" + error_message };
    co_yield{ std::is_same_v<typename T::propagate_on_container_copy_assignment, std::true_type> ,"propagate_on_container_copy_assignment" + error_message };
    co_yield{ std::is_same_v<typename T::propagate_on_container_move_assignment, std::false_type> ,"propagate_on_container_move_assignment" + error_message };
    co_yield{ std::is_same_v<typename T::propagate_on_container_swap, std::true_type> ,"propagate_on_container_swap" + error_message };
    co_yield{ std::is_same_v<typename T::is_always_equal, std::false_type>,"is_always_equal" + error_message };
    co_yield{ std::is_same_v<typename T::template rebind_alloc<double>, shit_allocator<int>> ,"rebind_alloc" + error_message };
    co_yield{ std::is_same_v<typename T::template rebind_traits<double>, NAMESPACE_MY allocator_traits<shit_allocator<int>>>,"rebind_traits" + error_message };

    co_return;
}

case_t allocator_with_std_traits()
{
    using Alloc = NAMESPACE_MY allocator<int_wrapper>;
    using T = std::allocator_traits<Alloc>;
    Alloc alloc;
    ap.reset_uncheck();
    int_wrapper::reset();
    co_yield "allocate raw memory for 5 objects";
    auto* raw_memory = T::allocate(alloc, 5);
#ifndef USE_STD
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 5 * sizeof(int_wrapper), std::format("alloc_proxy's current_allocations should be `{}`, but it actually is `{}`",  5 * sizeof(int_wrapper), ap.current_allocated_bytes) };
#endif
    co_yield{ int_wrapper::construct_count == 0, std::format("there should be nothing constructed after allocation without construction,\nbut `{}` objects constructed after allocation actually.", int_wrapper::construct_count) };
    co_yield{ int_wrapper::current_object_count == 0, std::format("there should be nothing constructed after allocation without construction,\nbut `{}` objects constructed after allocation actually.", int_wrapper::current_object_count) };

    co_yield "construct 5 objects in allocated memory";
    for (size_t i = 0; i < 5; ++i) { T::construct(alloc, &raw_memory[i], i); }
#ifndef USE_STD
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 5 * sizeof(int_wrapper), std::format("alloc_proxy's current_allocations should be `{}`, but it actually is `{}`",  5 * sizeof(int_wrapper), ap.current_allocated_bytes) };
#endif
    co_yield{ int_wrapper::construct_count == 5, std::format("There should be `5` objects constructed but `{}` actually.", int_wrapper::construct_count) };
    co_yield{ int_wrapper::current_object_count == 5, std::format("There should be `5` objects in memory but `{}` actually.", int_wrapper::current_object_count) };

    co_yield "destroy these objects";
    for (size_t i = 0; i < 5; ++i) { T::destroy(alloc, &raw_memory[i]); }
#ifndef USE_STD
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 5 * sizeof(int_wrapper), std::format("alloc_proxy's current_allocations should be `{}`, but it actually is `{}`",  5 * sizeof(int_wrapper), ap.current_allocated_bytes) };
#endif
    co_yield{ int_wrapper::construct_count == 5, std::format("There should be `5` objects constructed but `{}` actually.", int_wrapper::construct_count) };
    co_yield{ int_wrapper::current_object_count == 0, std::format("There should be no objects in memory but `{}` objects actually.", int_wrapper::current_object_count) };

    co_yield "deallocate raw memory and reset alloc_proxy";
    T::deallocate(alloc, raw_memory, 5);
    ap.reset();
    co_return;
}

case_t allocator_with_our_traits()
{
    using Alloc = NAMESPACE_MY allocator<int_wrapper>;
    using T = NAMESPACE_MY allocator_traits<Alloc>;
    Alloc alloc;
    ap.reset_uncheck();
    int_wrapper::reset();
    co_yield "allocate raw memory for 5 objects";
    auto raw_memory = T::allocate(alloc, 5);
#ifndef USE_STD
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 5 * sizeof(int_wrapper), std::format("alloc_proxy's current_allocations should be `{}`, but it actually is `{}`",  5 * sizeof(int_wrapper), ap.current_allocated_bytes) };
#endif
    co_yield{ int_wrapper::construct_count == 0, std::format("there should be nothing constructed after allocation without construction,\nbut `{}` objects constructed after allocation actually.", int_wrapper::construct_count) };
    co_yield{ int_wrapper::current_object_count == 0, std::format("there should be nothing constructed after allocation without construction,\nbut `{}` objects constructed after allocation actually.", int_wrapper::current_object_count) };

    co_yield "construct 5 objects in allocated memory";
    for (size_t i = 0; i < 5; ++i) { T::construct(alloc, &raw_memory[i], i); }
#ifndef USE_STD
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 5 * sizeof(int_wrapper), std::format("alloc_proxy's current_allocations should be `{}`, but it actually is `{}`",  5 * sizeof(int_wrapper), ap.current_allocated_bytes) };
#endif
    co_yield{ int_wrapper::construct_count == 5, std::format("There should be `5` objects constructed but `{}` actually.", int_wrapper::construct_count) };
    co_yield{ int_wrapper::current_object_count == 5, std::format("There should be `5` objects in memory but `{}` actually.", int_wrapper::current_object_count) };

    co_yield "destroy these objects";
    for (size_t i = 0; i < 5; ++i) { T::destroy(alloc, &raw_memory[i]); }
#ifndef USE_STD
    co_yield{ ap.current_allocations == 1, std::format("alloc_proxy's current_allocations should be `1`, but it actually is `{}`", ap.current_allocations) };
    co_yield{ ap.current_allocated_bytes == 5 * sizeof(int_wrapper), std::format("alloc_proxy's current_allocations should be `{}`, but it actually is `{}`",  5 * sizeof(int_wrapper), ap.current_allocated_bytes) };
#endif
    co_yield{ int_wrapper::construct_count == 5, std::format("There should be `5` objects constructed but `{}` actually.", int_wrapper::construct_count) };
    co_yield{ int_wrapper::current_object_count == 0, std::format("There should be no objects in memory but `{}` objects actually.", int_wrapper::current_object_count) };

    co_yield "deallocate raw memory and reset alloc_proxy";
    T::deallocate(alloc, raw_memory, 5);
    ap.reset();
    co_return;
}

case_t shit_allocator_with_our_traits()
{
    using T = NAMESPACE_MY allocator_traits<shit_allocator<int>>;
    shit_allocator<int> alloc;
    volatile int i;
    int ii;
    auto u = shit_allocator<int>::u{};
    std::string error_message = ": allocator_traits should use alloctor's defination or implementation if possible.";
    co_yield "test allocate(without hint)";
    try { T::allocate(alloc, 0); co_yield{ false, "allocate(without hint)" + error_message }; } catch (const int&) {}
    co_yield nullptr;
    co_yield "test allocate(with hint)";
    try { T::allocate(alloc, 0, &i); co_yield{ false, "allocate(with hint)" + error_message }; } catch (const int&) {}
    co_yield nullptr;
    co_yield "test deallocate";
    try { T::deallocate(alloc, &u, .5f); co_yield{ false, "deallocate" + error_message }; } catch (const int&) {}
    co_yield nullptr;
    co_yield "test construct";
    try { T::construct(alloc, &ii); co_yield{ false, "construct" + error_message }; } catch (const int&) {}
    co_yield nullptr;
    co_yield "test destroy";
    try { T::destroy(alloc, &ii); co_yield{ false, "destroy" + error_message }; } catch (const int&) {}
    co_yield nullptr;
    co_yield "test max_size";
    co_yield{ T::max_size(alloc) == .0f, "max_size" + error_message };
    co_yield nullptr;
    co_yield "test select_on_container_copy_construction";
    try { T::select_on_container_copy_construction(alloc); co_yield{ false, "select_on_container_copy_construction" + error_message }; } catch (const int&) {}
    co_return;
}

case_t alloc_at_least()
{
#ifndef USE_STD
    using T = my::allocator_traits<my::allocator<int>>;
    my::allocator<int> alloc;
    std::srand(std::time(0));
    int n = 1000;
    ap.reset_uncheck();

    while (n--)
    {
        size_t c = std::rand() + 1;
        co_yield std::format("alloc.allocate_at_least({})", c);
        auto [p3, size3] = alloc.allocate_at_least(c);
        size_t should = 1;
        while (should < c) { should *= 2; }
        co_yield{ size3 == should, std::format("allocated size should be `{}`, but it actually is `{}`", should, size3) };
        co_yield{ ap.current_allocated_bytes == should * sizeof(int), std::format("alloc_proxy's current_allocated_bytes should be `{}`, but it actually is `{}`", should * sizeof(int), ap.current_allocated_bytes) };

        co_yield "then deallocate it and reset alloc_proxy";
        alloc.deallocate(p3, size3);
        ap.reset();
        co_yield nullptr;
    }
    n = 1000;
    while (n--)
    {
        size_t c = std::rand() + 1;
        co_yield std::format("my::allocator_traits<my::allocator>::allocate_at_least(alloc, {})", c);
        auto [p2, size2] = T::allocate_at_least(alloc, c);
        size_t should = 1;
        while (should < c) { should *= 2; }
        co_yield{ size2 == should, std::format("allocated size should be `{}`, but it actually is `{}`", should, size2) };
        co_yield{ ap.current_allocated_bytes == should * sizeof(int), std::format("alloc_proxy's current_allocated_bytes should be `{}`, but it actually is `{}`", should * sizeof(int), ap.current_allocated_bytes) };

        co_yield "then deallocate it and reset alloc_proxy";
        alloc.deallocate(p2, size2);
        ap.reset();
        co_yield nullptr;
    }

    shit_allocator_2<int> sa2;
    co_yield std::format("my::allocator_traits' DEFAULT allocate_at_least(alloc, 10)");

    auto [p1, size1] = my::allocator_traits<decltype(sa2)>::allocate_at_least(sa2, 10);
    co_yield{ size1 == 10, std::format("allocated size should be `10`, but it actually is `{}`", size1) };
    co_yield{ reinterpret_cast<size_t>(p1) == 10, std::format("my::allocator_traits' DEFAULT allocate_at_least(alloc, 10) should call alloc.allocate(10) but it actually did not.") };

    co_yield nullptr;

#endif
    co_return;
}
    }
}

int main()
{
    my::test::test t;
    t.new_case(my::test::alloc_proxy(), "alloc_proxy");
    t.new_case(my::test::allocator_test(), "allocator");
    t.new_case(my::test::alloc_at_least(), "alloc_at_least");
    t.new_case(my::test::allocator_traits_types(), "member types of my::allocator_traits (by default)");
    t.new_case(my::test::our_traits_for_user_defined_allocator(), "member types of my::allocator_traits (user defined allocator)");
    t.new_case(my::test::allocator_with_std_traits(), "std::allocator_traits<my::allocator>");
    t.new_case(my::test::allocator_with_our_traits(), "my::allocator_traits<my::allocator>");
    t.new_case(my::test::shit_allocator_with_our_traits(), "my::allocator_traits<user_defined_allocator>");
}