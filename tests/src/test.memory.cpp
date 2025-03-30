#include "co_yantest.hpp"
#include "int_wrapper.hpp"
#include "yan_memory.hpp"
#include <memory>
#include <type_traits>

//#define USE_STD

#ifdef USE_STD
#define NAMESPACE_MY ::std::
#undef DISMISS_UNIQUE_PTR
#undef DISMISS_SHARED_AND_WEAK_PTR
#undef DISMISS_ENABLE_SHARED_FROM_THIS
#else
#define NAMESPACE_MY ::my::
#endif

namespace my
{
    namespace test
    {

template <typename T>
struct custom_allocator
{
    using value_type = T;

    custom_allocator(int* counter = nullptr) : counter(counter) {}

    template <typename U>
    custom_allocator(const custom_allocator<U>& other) : counter(other.counter) {}
    

    template <typename U>
    struct rebind {
        using other = custom_allocator<U>;
    };

    T* allocate(std::size_t n) {
        counter ? (*counter)++ : 0;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) {
        counter ? (*counter)-- : 0;
        ::operator delete(p);
    }

    int* counter;
};

case_t unique_ptr_ctor() {
#ifdef DISMISS_UNIQUE_PTR
    co_yield { case_t::state::DISMISSED, "test for `unique_ptr` has been dismissed." };
#else
    co_yield { noexcept(NAMESPACE_MY unique_ptr<int>()), "Default constructor must be noexcept" };
    co_yield { noexcept(NAMESPACE_MY unique_ptr<int>(nullptr)), "nullptr_t constructor must be noexcept" };
    co_yield nullptr;

    struct TrackedDeleter {
        int copies = 0, moves = 0;
        void operator()(int*) const {}
        TrackedDeleter() = default;
        TrackedDeleter(const TrackedDeleter& d) noexcept : copies(d.copies + 1), moves(d.moves) {}
        TrackedDeleter(TrackedDeleter&& d) noexcept : copies(d.copies), moves(d.moves + 1) {}
    };

    TrackedDeleter d1;
    co_yield{ "unique_ptr<int, TrackedDeleter> p1(new int, d1);" };
    NAMESPACE_MY unique_ptr<int, TrackedDeleter> p1(new int, d1);
    co_yield{ p1.get_deleter().copies == 1 && p1.get_deleter().moves == 0, "Deleter should be copied once." };
    co_yield nullptr;

    co_yield{ "unique_ptr<int, TrackedDeleter> p2(new int, TrackedDeleter{});" };
    NAMESPACE_MY unique_ptr<int, TrackedDeleter> p2(new int, TrackedDeleter{});
    co_yield{ p2.get_deleter().copies == 0 && p2.get_deleter().moves == 1, "Deleter should be moved once." };
    co_yield nullptr;
    
    TrackedDeleter d2;
    co_yield{ "unique_ptr<int, TrackedDeleter&> p3(new int, d2);" };
    NAMESPACE_MY unique_ptr<int, TrackedDeleter&> p3(new int, d2);
    co_yield{ &p3.get_deleter() == &d2, "Deleter reference should bind to original object." };
    co_yield nullptr;

    TrackedDeleter d3;
    co_yield{ "unique_ptr<int, const TrackedDeleter&> p4(new int, d2);" };
    NAMESPACE_MY unique_ptr<int, const TrackedDeleter&> p4(new int, d3);
    co_yield{ &p4.get_deleter() == &d3, "Deleter reference should bind to original object." };
    co_yield nullptr;


    co_yield { noexcept(NAMESPACE_MY unique_ptr<int, TrackedDeleter>(
        std::declval<NAMESPACE_MY unique_ptr<int, TrackedDeleter>&&>())),
        "Move constructor should be noexcept" };
    co_yield { noexcept(NAMESPACE_MY unique_ptr<int, TrackedDeleter&>(
        std::declval<NAMESPACE_MY unique_ptr<int, TrackedDeleter&>&&>())),
        "Move constructor should be noexcept" }; 
    co_yield { noexcept(NAMESPACE_MY unique_ptr<int, const TrackedDeleter&>(
        std::declval<NAMESPACE_MY unique_ptr<int, const TrackedDeleter&>&&>())),
        "Move constructor should be noexcept" };
    co_yield nullptr;


    bool base_called = false;
    bool derived_called = false;

    struct BaseDeleter {
        bool* b;
        BaseDeleter(bool* ptr) : b(ptr) {}
        virtual void operator()(int* p) const {
            delete p;
            *b = true;
        }
        virtual ~BaseDeleter() = default;
    };
    struct DerivedDeleter : BaseDeleter {
        bool* b;
        DerivedDeleter(bool* ptr1, bool* ptr2) : BaseDeleter(ptr1), b(ptr2) {}
        void operator()(int* p) const override {
            delete p;
            *b = true;
        }
    };

    co_yield{"unique_ptr<int, DerivedDeleter> p6;"};
    NAMESPACE_MY unique_ptr<int, DerivedDeleter> p6(new int, {&base_called, &derived_called});
    co_yield{"unique_ptr<int, BaseDeleter> p7(std::move(p6));"};
    NAMESPACE_MY unique_ptr<int, BaseDeleter> p7(std::move(p6));
    co_yield{"reset p7."};
    p7.reset();

    co_yield{ base_called == true && derived_called == false,
        "Base deleter's operator() should be called due to type casting." };
    co_yield nullptr;
#endif
    co_return;
}

case_t unique_ptr() {
#ifdef DISMISS_UNIQUE_PTR
    co_yield { case_t::state::DISMISSED, "test for `unique_ptr` has been dismissed." };
#else
    co_yield "Create a unique_ptr p1 with default construction.";
    {
        NAMESPACE_MY unique_ptr<int_wrapper> p1;
        co_yield{ p1.get() == nullptr,
            std::format("p1.get() should return a nullptr when get() called, but we get {:x}.", (long)p1.get()) };
        co_yield{ p1 == nullptr,
            std::format("p1 should be a nullptr, and should be equal to a nullptr_t value. But actually is {:d}.", p1 == nullptr) };
        co_yield{ !p1,
            std::format("p1 should be a nullptr, and should return false when being converted to bool. But we got {:d}.", static_cast<bool>(p1)) };
        co_yield{ int_wrapper::current_object_count == 0,
            std::format("Unexpected object creation detected during default construction.") };

        co_yield "auto p = new int(43);";
        co_yield "p1.reset(p);";
        auto p = new int_wrapper(43);
        p1.reset(p);
        co_yield{ int_wrapper::current_object_count == 1,
            std::format("Object ownership not properly transferred after reset().") };
        co_yield{ !!p1,
            std::format("p1 should hold a non-nil pointer, and should return true when being converted to bool. But we get {:d}.", static_cast<bool>(p1)) };
        co_yield{ p1 != nullptr,
            std::format("p1 should hold a non-nil pointer, and should not be equal to a nullptr_t value. But actually is {:d}.", p1 != nullptr) };
        co_yield{ p1.get() == p,
            std::format("p1.get() should return pointer p, but we get {:x}.", (long)p1.get()) };
    }
    co_yield{ int_wrapper::current_object_count == 0,
        std::format("Resource leak detected after p1's destruction.") }; 
    co_yield nullptr;

    co_yield "Create a unique_ptr p2 with pointer construction, Argument = new int(42).";
    {
        NAMESPACE_MY unique_ptr<int_wrapper> p2(new int_wrapper(42));
        co_yield{ int_wrapper::current_object_count == 1,
            std::format("Ownership of dynamically allocated object not properly managed.") };
        co_yield{ !!p2,
            std::format("p2 should hold a non-nil pointer, and should return true when being converted to bool. But we get {:d}.", static_cast<bool>(p2)) };
        co_yield{ p2 != nullptr,
            std::format("p2 should hold a non-nil pointer, and should not be equal to a nullptr_t value. But actually is {:d}.", p2 != nullptr) };
        co_yield{ *p2 == int_wrapper(42),
            std::format("p2 should hold a value 42, but we get {:d}.", int(*p2)) };

        co_yield "Create a unique_ptr p3 by moving p2.";
        NAMESPACE_MY unique_ptr<int_wrapper> p3(std::move(p2));
        co_yield{ int_wrapper::current_object_count == 1,
            std::format("Object ownership corrupted during move operation.") };
        co_yield{ !p2,
            std::format("p2 should be empty now, but actually p2 is true in bool.") };
        co_yield{ p3 && *p3 == 42,
            std::format("p3 should hold a non-nil pointer and hold the value 42, but actually is {}.", p3 ? std::to_string((int)(*p3)) : std::string("not vaild")) };

        co_yield "Divide *p3 by 2.";
        *p3 = 21;
        co_yield{ *p3 == 21,
            std::format("p3 should hold a value 21 now. But we get {:d}.", int(*p3)) };

        co_yield "Create a unique_ptr p4, and do assignment with p3.";
        NAMESPACE_MY unique_ptr<int_wrapper> p4;
        p4 = std::move(p3);
        co_yield{ int_wrapper::current_object_count == 1,
            std::format("Object duplication detected during move assignment.") };
        co_yield{ !p3,
            std::format("p3 should be empty now, but actually p3 is true in bool.") };
        co_yield{ p4 && *p4 == 21,
            std::format("p4 should hold the value 21. But actually is {}.", p4 ? std::to_string((int)(*p4)) : std::string("not vaild")) };

        co_yield "Release p4.";
        int_wrapper* raw_ptr = p4.release();
        co_yield{ int_wrapper::current_object_count == 1,
            std::format("Resource prematurely released before manual deletion.") };
        co_yield{ !p4,
            std::format("p4 should be empty now. But we get {:d}.", static_cast<bool>(p4)) };
        co_yield{ raw_ptr && *raw_ptr == int_wrapper(21),
            std::format("p4.release() should return a pointer that holds the value 21, but actually is {}.", raw_ptr ? std::to_string((int)(*raw_ptr)) : std::string("not vaild")) };

        delete raw_ptr;
        co_yield{ int_wrapper::current_object_count == 0,
            std::format("Resource not freed after manual deletion.") };
    }
    co_yield nullptr;

    co_yield "struct V2 { int value; };\np5(new V2(77));";
    struct V2
    {
        V2(int value) : value(value) {}
        int value;
    };
    {
        NAMESPACE_MY unique_ptr<V2> p5(new V2(77));
        co_yield{ p5 && p5->value == 77, std::format("`p5->value` should be `77`, but actually is `{}`.", p5 ? std::to_string(p5->value) : std::string("not vaild")) };

        co_yield "let `p5->value` be `42`.";
        p5->value = 42;
        co_yield{ p5 && (*p5).value == 42, std::format("`(*p5).value` should be `42` now, but actually is `{}`.", p5 ? std::to_string(p5->value) : std::string("not vaild")) };
    }
    co_yield nullptr;

    co_yield "Let `p6` handle an int array.";
    {
        NAMESPACE_MY unique_ptr<int[]> p6(new int[3] {1, 2, 3});
        co_yield{ p6[0] == 1 && p6[1] == 2 && p6[2] == 3, std::format("`operator[]` should return `[1,2,3]` in a series, but we get `[{},{},{}]`.", p6[0], p6[1], p6[2]) };

        co_yield "Reset `p6` with a new array `{4,5,6}`.";
        p6.reset(new int[3] {4, 5, 6});
        co_yield{ p6[0] == 4 && p6[1] == 5 && p6[2] == 6, std::format("`p6` should now hold `[4,5,6]`, but actually `[{},{},{}]`.", p6[0], p6[1], p6[2]) };
    }
    co_yield nullptr;

    co_yield "Create a unique_ptr `p7` holds `V2(88)` by make_unique.";
    {
        auto p7 = NAMESPACE_MY make_unique<V2>(88);
        co_yield{ p7 && p7->value == 88, std::format("`make_unique p7` should hold a value `88`, but actually is `{}`.",  p7 ? std::to_string(p7->value) : std::string("not vaild")) };
    }
    co_yield nullptr;

    bool deleter_called = false;
    struct Deleter {
        bool* d;
        Deleter(bool* d) : d(d) {}
        void operator()(int* ptr) { delete ptr; *d = true; }
        Deleter(const Deleter& dd) : d(dd.d) {} 
        Deleter(Deleter&& dd) : d(dd.d) {} 
        Deleter& operator=(const Deleter& dd) { d = dd.d; return *this; }
        Deleter& operator=(Deleter&& dd) { d = dd.d; return *this; }
    };
    Deleter custom_deleter = &deleter_called;
    co_yield{ std::is_move_assignable<decltype(custom_deleter)>::value };
    {
        co_yield "Create an empty unique_ptr with a custom deleter";
        NAMESPACE_MY unique_ptr<int, Deleter> p10(nullptr, custom_deleter);
        co_yield "Going out of scope";
    }
    co_yield{ !deleter_called, std::format("There should be no effects when destory an empty unique_ptr, but actually the deleter has been called.") };
    {
        co_yield "Create two empty unique_ptrs with a custom deleter";
        NAMESPACE_MY unique_ptr<int, decltype(custom_deleter)> p11(nullptr, custom_deleter);
        co_yield "Move one into another";
        NAMESPACE_MY unique_ptr<int, decltype(custom_deleter)> p12(nullptr, custom_deleter);
        co_yield{ !deleter_called, std::format("There should be no effects when construct unique_ptrs, but actually the deleter has been called.") };
        p11 = std::move(p12);
        co_yield{ !deleter_called, std::format("There should be no effects when move assigning into an empty unique_ptr, but actually the deleter has been called.") };
        co_yield "Reset one";
        p11.reset();
        co_yield{ !deleter_called, std::format("There should be no effects when reset an empty unique_ptr, but actually the deleter has been called.") };
        co_yield "Going out of scope";
    }
    co_yield{ !deleter_called, std::format("There should be no effects when destory an empty unique_ptr, but actually the deleter has been called.") };

    {
        co_yield "create an nonempty unique_ptr with a custom deleter";
        NAMESPACE_MY unique_ptr<int, decltype(custom_deleter)> p11(new int, custom_deleter);
        co_yield "move it for another unique_ptr's construct";
        auto p12 = std::move(p11);
        co_yield{ !deleter_called, std::format("There should be no effects when move a unique_ptr, but actually the deleter has been called.") };
        co_yield "Going of scope";
    }
    co_yield{ deleter_called, std::format("The custom deleter should be called, but it seems not.") };
    co_yield nullptr;

    {
        auto s = int_wrapper::counter_scope();
        auto p = new int_wrapper(77);
        struct FakeDeleter { void operator()(int_wrapper*) {} };
        co_yield "unique_ptr<int_wrapper, FakeDeleter> p1(p);";
        NAMESPACE_MY unique_ptr<int_wrapper, FakeDeleter> p1(p);
        co_yield "unique_ptr<int_wrapper, TrueDeleter> p2(p);";
        NAMESPACE_MY unique_ptr<int_wrapper> p2(p);

        co_yield{ p1 == p2, "Two unique_ptr should be equal if their pointers are same, even if their deleter are different." };
    }    

    co_yield nullptr;
#endif
    co_return;
}

case_t shared_ptr() {
#ifdef DISMISS_SHARED_AND_WEAK_PTR
    co_yield { case_t::state::DISMISSED, "test for `shared_ptr` and `weak_ptr` has been dismissed." };
#else
    co_yield "Create a shared_ptr `p1` with default construction.";
    {
        NAMESPACE_MY shared_ptr<int_wrapper> p1;
        co_yield{ p1.get() == nullptr,
            std::format("`p1.get()` should return a `nullptr` for default constructed `shared_ptr`, but got `{:x}`.", (long)p1.get()) };
        co_yield{ !p1,
            std::format("`p1` should be `false` in bool context when empty, but conversion returned `{:d}`.", static_cast<bool>(p1)) };
        co_yield{ int_wrapper::current_object_count == 0,
            std::format("No object should be created on default construction, but `int_wrapper::current_object_count` is `{}`.", int_wrapper::current_object_count) };
    }
    co_yield nullptr;

    co_yield "Create a shared_ptr `p2` with pointer construction: new int_wrapper(42).";
    {
        NAMESPACE_MY shared_ptr<int_wrapper> p2(new int_wrapper(42));
        co_yield{ int_wrapper::current_object_count == 1,
            std::format("After constructing `p2`, there should be `1` object, but `int_wrapper::current_object_count` is `{}`.", int_wrapper::current_object_count) };
        co_yield{ p2 && *p2 == int_wrapper(42),
            std::format("`p2` should hold `int_wrapper(42)`, but got `{}`.", p2 ? std::to_string((int)(*p2)) : "null") };
        co_yield{ p2.use_count() == 1,
            std::format("`p2.use_count()` should be `1`, but got `{}`.", p2.use_count()) };

        co_yield "Copy construct shared_ptr `p3` from `p2`.";
        NAMESPACE_MY shared_ptr<int_wrapper> p3(p2);
        co_yield{ p2.use_count() == 2 && p3.use_count() == 2,
            std::format("After copying, `use_count` should be `2`, but got `p2.use_count()`: `{}`, `p3.use_count()`: `{}`.", p2.use_count(), p3.use_count()) };

        co_yield "Reset `p3` to release its ownership.";
        p3.reset();
        co_yield{ !p3,
            std::format("`p3` should be empty after `reset()`, but conversion to bool returned `{:d}`.", static_cast<bool>(p3)) };
        co_yield{ p2.use_count() == 1,
            std::format("After resetting `p3`, `p2.use_count()` should be `1`, but got `{}`.", p2.use_count()) };

        co_yield "Assign shared_ptr `p4` from `p2` using copy assignment.";
        NAMESPACE_MY shared_ptr<int_wrapper> p4;
        p4 = p2;
        co_yield{ p2.use_count() == 2 && p4.use_count() == 2,
            std::format("After copy assignment, `use_count` should be `2`, but got `p2.use_count()`: `{}`, `p4.use_count()`: `{}`.", p2.use_count(), p4.use_count()) };

        co_yield "Move construct shared_ptr `p5` from `p2`.";
        NAMESPACE_MY shared_ptr<int_wrapper> p5(std::move(p2));
        co_yield{ !p2,
            std::format("After move construction, `p2` should be empty.") };
        co_yield{ p5.use_count() == 2,
            std::format("After move, `p5.use_count()` should be `2`, but got `{}`.", p5.use_count()) };

        co_yield "Move assign shared_ptr `p6` from `p5`.";
        NAMESPACE_MY shared_ptr<int_wrapper> p6;
        p6 = std::move(p5);
        co_yield{ !p5,
            std::format("After move assignment, `p5` should be empty.") };
        co_yield{ p6.use_count() == 2,
            std::format("After move assignment, `p6.use_count()` should be `2`, but got `{}`.", p6.use_count()) };

        co_yield "Reset `p4` to test proper reference count decrement.";
        p4.reset();
        co_yield{ p6.use_count() == 1,
            std::format("After resetting `p4`, `p6.use_count()` should be `1`, but got `{}`.", p6.use_count()) };

        co_yield "Modify the shared object via `p6`.";
        *p6 = int_wrapper(21);
        co_yield{ *p6 == int_wrapper(21),
            std::format("After modification, `p6` should hold `int_wrapper(21)`, but got `{}`.", static_cast<int>(*p6)) };

        co_yield "Create more shared_ptr instances to test `use_count` consistency.";
        {
            NAMESPACE_MY shared_ptr<int_wrapper> p7(p6);
            NAMESPACE_MY shared_ptr<int_wrapper> p8(p6);
            NAMESPACE_MY shared_ptr<int_wrapper> p9 = p6;
            co_yield{ p6.use_count() == 4 && p7.use_count() == 4 && p8.use_count() == 4 && p9.use_count() == 4,
                std::format("After copying, `p6`, `p7`, `p8`, and `p9` `use_count` should be `4`, but got `p6.use_count()`: `{}`, `p7.use_count()`: `{}`, `p8.use_count()`: `{}`, `p9.use_count()`: `{}`.",
                p6.use_count(), p7.use_count(), p8.use_count(), p9.use_count()) };

            co_yield "Reset one of the copies, `p7`, and check `use_count`.";
            p7.reset();
            co_yield{ p6.use_count() == 3 && p8.use_count() == 3 && p9.use_count() == 3,
                std::format("After resetting `p7`, the `use_count` of `p6`, `p8`, and `p9` should be `3`, but got `p6.use_count()`: `{}`, `p8.use_count()`: `{}`, `p9.use_count()`: `{}`.",
                p6.use_count(), p8.use_count(), p9.use_count()) };

            co_yield "Assign a new shared_ptr `p10` from `p6` and then reset `p8`.";
            NAMESPACE_MY shared_ptr<int_wrapper> p10 = p6;
            p8.reset();
            co_yield{ p6.use_count() == 3 && p9.use_count() == 3 && p10.use_count() == 3,
                std::format("After assigning `p10` and resetting `p8`, the `use_count` of `p6`, `p9`, and `p10` should be `3`, but got `p6.use_count()`: `{}`, `p9.use_count()`: `{}`, `p10.use_count()`: `{}`.",
                p6.use_count(), p9.use_count(), p10.use_count()) };
        }
    }
    co_yield{ int_wrapper::current_object_count == 0,
        std::format("Resource leak detected: `int_wrapper::current_object_count` is `{}` after `shared_ptr` scope, expected `0`.", int_wrapper::current_object_count) };
    co_yield nullptr;

    co_yield "Test operator-> with a custom struct `V2`.";
    struct V2 {
        V2(int value) : value(value) {}
        int value;
    };
    {
        NAMESPACE_MY shared_ptr<V2> p7(new V2(77));
        co_yield{ p7 && p7->value == 77,
            std::format("`p7->value` should be `77`, but got `{}`.", p7 ? std::to_string(p7->value) : "not valid")};
        co_yield "Set `p7->value` to `42`.";
        p7->value = 42;
        co_yield{ p7 && (*p7).value == 42,
            std::format("After modification, `(*p7).value` should be `42`, but got `{}`.", p7 ? std::to_string(p7->value) : "not valid") };
    }
    co_yield nullptr;

    co_yield "Create a shared_ptr `p8` using make_shared with int_wrapper(88).";
    {
        auto p8 = NAMESPACE_MY make_shared<int_wrapper>(88);
        co_yield{ p8 && *p8 == int_wrapper(88),
            std::format("`p8` should hold `int_wrapper(88)`, but got `{}`.", p8 ? std::to_string((int)(*p8)) : "null") };
        co_yield{ p8.use_count() == 1,
            std::format("`p8.use_count()` should be `1`, but got `{}`.", p8.use_count()) };
        co_yield "Copy `p8` to `p9` to verify reference counting.";
        NAMESPACE_MY shared_ptr<int_wrapper> p9(p8);
        co_yield{ p8.use_count() == 2 && p9.use_count() == 2,
            std::format("After copying, `use_count` should be `2`, but got `p8.use_count()`: `{}`, `p9.use_count()`: `{}`.", p8.use_count(), p9.use_count()) };
    }
    co_yield{ int_wrapper::current_object_count == 0,
        std::format("Resource leak detected: `int_wrapper::current_object_count` is `{}` after `p8`/`p9` scope, expected `0`.", int_wrapper::current_object_count) };
    co_yield nullptr;

    co_yield "Test `allocate_shared` with a custom allocator.";
    {
        int alloc_counter = 0; // Track allocations and deallocations
        custom_allocator<int_wrapper> alloc(&alloc_counter);

        co_yield "Create a `shared_ptr` using `allocate_shared` with a custom allocator.";
        {
            auto p11 = NAMESPACE_MY allocate_shared<int_wrapper>(alloc, 123);
            co_yield{ p11 && *p11 == int_wrapper(123),
                std::format("`p11` should hold `int_wrapper(123)`, but got `{}`.", p11 ? std::to_string((int)(*p11)) : "null") };
            co_yield{ p11.use_count() == 1,
                std::format("`p11.use_count()` should be `1`, but got `{}`.", p11.use_count()) };
            co_yield{ alloc_counter == 1,
                std::format("There should be 1 time allocator call, but got `{}` time(s).", alloc_counter) };

            co_yield "Copy `p11` to `p12` to verify reference counting.";
            NAMESPACE_MY shared_ptr<int_wrapper> p12(p11);
            co_yield{ p11.use_count() == 2 && p12.use_count() == 2,
                std::format("After copying, `use_count` should be `2`, but got `p11.use_count()`: `{}`, `p12.use_count()`: `{}`.", p11.use_count(), p12.use_count()) };
        }

        co_yield{ alloc_counter == 0,
            std::format("After `p11` and `p12` go out of scope, allocator should deallocate all memory, but got `{}` block(s) still alive.", alloc_counter) };
        co_yield{ int_wrapper::current_object_count == 0,
            std::format("Resource leak detected: there is {} objects remaining after `allocate_shared` scope, expected `0`.", int_wrapper::current_object_count) };
    }
    co_yield nullptr;

    bool deleter_called = false;
    {
        auto custom_deleter = [&](int* ptr) { delete ptr; deleter_called = true; };
        NAMESPACE_MY shared_ptr<int> p10(new int(99), custom_deleter);
        co_yield{ p10 && *p10 == 99,
            std::format("`p10` should hold `99`, but got `{}`.", p10 ? std::to_string(*p10) : "null") };
        co_yield{ p10.use_count() == 1,
            std::format("`p10.use_count()` should be `1`, but got `{}`.", p10.use_count()) };
    }
    co_yield{ deleter_called,
        std::format("`Custom deleter` should be called upon `shared_ptr` destruction.") };

    co_yield nullptr;
#endif
    co_return;
}

case_t weak_ptr() {
#ifdef DISMISS_SHARED_AND_WEAK_PTR
    co_yield { case_t::state::DISMISSED, "test for `shared_ptr` and `weak_ptr` has been dismissed." };
#else
    {
        co_yield "Create a `shared_ptr` using `make_shared` with `int_wrapper(100)`.";
        auto p = NAMESPACE_MY make_shared<int_wrapper>(100);

        co_yield "Construct a `weak_ptr` from the `shared_ptr` `p`.";
        NAMESPACE_MY weak_ptr<int_wrapper> wp(p);
        co_yield{ !wp.expired(),
            std::format("`wp.expired()` should be `false` after creation from a live `shared_ptr`, but got `{}`.", wp.expired() ? "true" : "false") };

        co_yield "Use `lock()` to obtain a `shared_ptr` from `wp`.";
        auto lp = wp.lock();
        co_yield{ lp && *lp == int_wrapper(100),
            std::format("`wp.lock()` should return a valid `shared_ptr` holding `int_wrapper(100)`, but got `{}`.", lp ? std::to_string((int)(*lp)) : std::string("not valid")) };
        co_yield{ lp.use_count() == p.use_count(),
            std::format("`lp.use_count()` should equal `p.use_count()`, but got `lp.use_count()`: `{}`, `p.use_count()`: `{}`.", lp.use_count(), p.use_count()) };

        co_yield "Copy construct a new `weak_ptr` `wp2` from `wp` and check its expired state.";
        NAMESPACE_MY weak_ptr<int_wrapper> wp2(wp);
        co_yield{ !wp2.expired(),
            std::format("Copied `wp2` should not be expired, but `wp2.expired()` returned `true`.") };

        co_yield "In a new scope, create a `shared_ptr` `p2` with `int_wrapper(200)` and a corresponding `weak_ptr` `wp3`.";
        {
            auto p2 = NAMESPACE_MY make_shared<int_wrapper>(200);
            NAMESPACE_MY weak_ptr<int_wrapper> wp3(p2);
            co_yield{ !wp3.expired(),
                std::format("`wp3.expired()` should be `false` for a live `p2`, but got `true`.") };

            co_yield "Reset `p2` to test if `wp3` becomes expired.";
            p2.reset();
            co_yield{ wp3.expired(),
                std::format("After resetting `p2`, `wp3.expired()` should be `true`, but got `false`.") };

            co_yield "Use `lock()` on `wp3` to ensure it returns a null `shared_ptr`.";
            auto lp3 = wp3.lock();
            co_yield{ !lp3,
                std::format("After resetting `p2`, `wp3.lock()` should return a null `shared_ptr`, but got non-null.") };
        }

        co_yield "Assign `wp` to a new `weak_ptr` `wp4`.";
        NAMESPACE_MY weak_ptr<int_wrapper> wp4;
        wp4 = wp;
        co_yield{ !wp4.expired(),
            std::format("After assignment, `wp4.expired()` should be `false`, but got `true`.") };

        co_yield "Move `wp4` into `wp5`.";
        NAMESPACE_MY weak_ptr<int_wrapper> wp5(std::move(wp4));
        co_yield{ !wp5.expired(),
            std::format("After move construction, `wp5.expired()` should be `false`, but got `true`.") };
        co_yield{ wp4.expired(),
            std::format("After move, the moved-from `wp4` should be expired, but `wp4.expired()` returned `false`.") };

        co_yield "Reset all strong references (`lp` and `p`) to check that all related `weak_ptr`s become expired.";
        lp.reset();
        p.reset();
        co_yield{ wp.expired(),
            std::format("After resetting all strong references, `wp.expired()` should be `true`, but got `false`.") };
        auto lp_after_reset = wp.lock();
        co_yield{ !lp_after_reset,
            std::format("After resetting all strong references, `wp.lock()` should return a null `shared_ptr`, but got non-null.") };
        co_yield{ wp2.expired(),
            std::format("After resetting all strong references, `wp2.expired()` should be `true`, but got `false`.") };
        co_yield{ wp5.expired(),
            std::format("After resetting all strong references, `wp5.expired()` should be `true`, but got `false`.") };

        co_yield "Call `reset()` on `wp2` to clear it.";
        wp2.reset();
        co_yield{ wp2.expired(),
            std::format("After calling `wp2.reset()`, `wp2.expired()` should be `true`.") };

        co_yield "Self-assignment of `weak_ptr` (`wp5 = wp5`).";
        wp5 = wp5;
        co_yield{ wp5.expired(),
            std::format("After self-assignment, `wp5.expired()` should still be `true`.") };
    }
    co_yield nullptr;

    {
        co_yield "Create two `shared_ptr`s, `p1` and `p2`, with values `int_wrapper(100)` and `int_wrapper(200)` respectively.";
        auto p1 = NAMESPACE_MY make_shared<int_wrapper>(100);
        auto p2 = NAMESPACE_MY make_shared<int_wrapper>(200);
    
        co_yield "Create two `weak_ptr`s, `wp1` from `p1` and `wp2` from `p2`.";
        NAMESPACE_MY weak_ptr<int_wrapper> wp1(p1);
        NAMESPACE_MY weak_ptr<int_wrapper> wp2(p2);
    
        co_yield "Create a third `weak_ptr`, `wp3`, from `p1` to share ownership with `wp1`.";
        NAMESPACE_MY weak_ptr<int_wrapper> wp3(p1);
    
        co_yield "Compare `wp1` and `wp3` using `owner_before` to verify shared ownership.";
        co_yield{ !wp1.owner_before(wp3) && !wp3.owner_before(wp1),
            std::format("`wp1` and `wp3` should have the same ownership order, but `owner_before` indicates otherwise.") };
    
        co_yield "Compare `wp1` and `p1` using `owner_before` to verify shared ownership between `weak_ptr` and `shared_ptr`.";
        co_yield{ !wp1.owner_before(p1) && !p1.owner_before(wp1),
            std::format("`wp1` and `p1` should have the same ownership order, but `owner_before` indicates otherwise.") };
    
        co_yield "Compare `wp1` and `wp2` using `owner_before` to check ownership order.";
        bool wp1_before_wp2 = wp1.owner_before(wp2);
        bool wp2_before_wp1 = wp2.owner_before(wp1);
        co_yield{ wp1_before_wp2 != wp2_before_wp1,
            std::format("`wp1` and `wp2` should have a consistent ownership order, but `owner_before` is inconsistent. wp1_before_wp2: {}, wp2_before_wp1: {}", wp1_before_wp2, wp2_before_wp1) };
    }
    co_yield nullptr;
    
#endif
    co_return;
}

case_t enable_shared_from_this() {
#ifdef DISMISS_ENABLE_SHARED_FROM_THIS
    co_yield { case_t::state::DISMISSED, "test for `enable_` has been dismissed." };
#else
    {
        co_yield "Define a class `MyClass` inheriting from `NAMESPACE_MY enable_shared_from_this<MyClass>`.";
        struct MyClass : public NAMESPACE_MY enable_shared_from_this<MyClass> {
            int value;
            MyClass(int v) : value(v) {}
        };

        co_yield "Create a shared_ptr `p` using `NAMESPACE_MY make_shared` for `MyClass` with value `123`.";
        auto p = NAMESPACE_MY make_shared<MyClass>(123);
        co_yield{ p && p->value == 123,
            std::format("`p->value` should be `123`, but got `{}`.", p ? std::to_string(p->value) : std::string("not vaild")) };

        co_yield "Call `shared_from_this()` on `p` to obtain another shared_ptr `p2`.";
        auto p2 = p->shared_from_this();
        co_yield{ p2 && p2->value == 123,
            std::format("`p2->value` should be `123`, but got `{}`.", p2 ? std::to_string(p2->value) : std::string("not vaild")) };

        co_yield "Verify that `p` and `p2` point to the same object and share the same `use_count`.";
        co_yield{ p.get() == p2.get(),
            std::format("`p` and `p2` should point to the same object, but got different addresses.") };
        co_yield{ p.use_count() == p2.use_count() && p.use_count() >= 2,
            std::format("`p.use_count()` should be at least `2` after obtaining `p2`, but got `{}`.", p.use_count()) };

        {
            MyClass stack_obj(456);
            bool exception_thrown = false;
            try {
                auto p3 = stack_obj.shared_from_this();
            }
            catch (const std::bad_weak_ptr&) {
                exception_thrown = true;
            }
            co_yield{ exception_thrown,
                std::format("Calling `shared_from_this()` on an object not managed by a `shared_ptr` should throw `std::bad_weak_ptr`.") };
        }
    }
    co_yield nullptr;
#endif
    co_return;
}

case_t type_casting() {
#ifdef DISMISS_TYPE_CASTING
    co_yield { case_t::state::DISMISSED, "type_casting tests dismissed" };
#else
    struct Base { 
        virtual ~Base() = default; 
        int base_marker = 0xBA5E;
    };
    
    struct Derived : Base { 
        int derived_marker = 0xD4D4;
        virtual void anchor() {}
    };
    
    struct Unrelated { virtual ~Unrelated() = default; };

    co_yield nullptr; 
    
    //--------------------------------------------------
    // Scenario 1: static_pointer_cast valid conversion
    co_yield "Cast shared_ptr<Derived> to it's base class pointer";
    {
        auto orig = NAMESPACE_MY make_shared<Derived>();
        auto base_ptr = NAMESPACE_MY static_pointer_cast<Base>(orig);
        co_yield{ base_ptr.get() == orig.get(),
            std::format("Address mismatch! Original: {:x} Cast: {:x}", 
                        (uintptr_t)orig.get(), (uintptr_t)base_ptr.get()) };

        co_yield{ orig.use_count() == 2 && base_ptr.use_count() == 2,
            std::format("Bad refcount. Original: {} Cast: {} (expected 2)", 
                        orig.use_count(), base_ptr.use_count()) };

        co_yield{ base_ptr->base_marker == 0xBA5E,
            std::format("Base data corrupted. Value: {:x}", 
                        base_ptr->base_marker) };
    }
    
    co_yield nullptr; // Clear scenario 1
    
    //--------------------------------------------------
    // Scenario 2: dynamic_pointer_cast success case
    co_yield "Cast shared_ptr<Base> to it's valid derived class pointer";
    {
        auto true_derived = NAMESPACE_MY make_shared<Derived>();
        NAMESPACE_MY shared_ptr<Base> base_ptr = true_derived;
        
        auto recast = NAMESPACE_MY dynamic_pointer_cast<Derived>(base_ptr);
        co_yield{ recast != nullptr,
            "Nullptr returned for valid dynamic downcast" };
            
        co_yield{ recast->derived_marker == 0xD4D4,
            std::format("Derived data corrupted. Value: {:x}", 
                        recast->derived_marker) };
        
        co_yield{ recast.use_count() == 3 && base_ptr.use_count() == 3,
            std::format("Bad refcount. cast: {} base: {} (expected 3)", 
                        recast.use_count(), base_ptr.use_count()) };
    }

    co_yield nullptr; // Clear scenario 2
    
    //--------------------------------------------------
    // Scenario 3: dynamic_pointer_cast failure case  
    co_yield "Cast shared_ptr<Base> to it's INVALID derived class pointer";
    {
        auto base_ptr = NAMESPACE_MY make_shared<Base>();
        
        auto bad_cast = NAMESPACE_MY dynamic_pointer_cast<Derived>(base_ptr);
        co_yield{ bad_cast == nullptr,
            "Non-null returned for invalid downcast" };
        
        co_yield{ base_ptr.use_count() == 1,
            std::format("Refcount corrupted. Value: {}", 
                        base_ptr.use_count()) };
    }

    co_yield nullptr; // Clear scenario 3

    //--------------------------------------------------
    // Scenario 4: reinterpret_pointer_cast
    co_yield "Reinterpret from shared_ptr<int> to shared_ptr<long>";
    {
        auto orig = NAMESPACE_MY make_shared<int>(0xCAFE);
        auto alien_ptr = NAMESPACE_MY reinterpret_pointer_cast<long>(orig);
        
        co_yield{ (void*)alien_ptr.get() == (void*)orig.get(),
            std::format("Address mismatch! Original: {:x} Cast: {:x}", 
                (uintptr_t)orig.get(), (uintptr_t)alien_ptr.get()) };
        
        co_yield{ orig.use_count() == 2 && alien_ptr.use_count() == 2,
            std::format("Bad refcount. Original: {} Alien: {} (expected 2)",
                        orig.use_count(), alien_ptr.use_count()) };
    }
    
    co_yield nullptr; // Clear scenario 4
    
    //--------------------------------------------------
    // Scenario 5: const_pointer_cast
    co_yield "Cast shared_ptr<int> to shared_ptr<const int> by const_pointer_cast";
    {
        auto mutable_ptr = NAMESPACE_MY make_shared<int>(0xBEEF);
        NAMESPACE_MY shared_ptr<const int> const_ptr = mutable_ptr;
        
        auto unconst_ptr = NAMESPACE_MY const_pointer_cast<int>(const_ptr);
        co_yield{ unconst_ptr.get() == mutable_ptr.get(),
            std::format("Address mismatch after cast in and out. Original: {:x} Dual-Cast: {:x}", 
                (uintptr_t)mutable_ptr.get(), (uintptr_t)unconst_ptr.get()) };
            
        co_yield{ mutable_ptr.use_count() == 3 && 
                const_ptr.use_count() == 3 && 
                unconst_ptr.use_count() == 3,
            std::format("Refcount corruption. Counts(Original: {}, const: {}, Dual-Cast: {}; expected 3)",
                        mutable_ptr.use_count(), 
                        const_ptr.use_count(),
                        unconst_ptr.use_count()) };
    }

    co_yield nullptr;

    //--------------------------------------------------
    // Scenario 6: Implicit conversion of unique_ptr from derived to base class
    {
        struct Base { 
            int base_marker = 0xBA5E;
            virtual ~Base() = default; // Ensure proper cleanup for derived objects
        };
        
        struct Derived : Base { 
            int derived_marker = 0xD4D4;
            bool* destructor_called; // Pointer to a flag to track destruction
            Derived(bool* flag) : destructor_called(flag) {}
            ~Derived() { *destructor_called = true; } // Set flag on destruction
        };

        bool derived_destructor_called = false; // Flag to track if Derived destructor is called

        co_yield "Create a unique_ptr<Derived> with a flag to track destruction";
        auto derived_ptr = NAMESPACE_MY make_unique<Derived>(&derived_destructor_called);
        derived_ptr->base_marker = 0xBA5E;
        derived_ptr->derived_marker = 0xD4D4;

        co_yield "Implicitly convert unique_ptr<Derived> to unique_ptr<Base>";
        NAMESPACE_MY unique_ptr<Base> base_ptr = std::move(derived_ptr);
        co_yield{ base_ptr && base_ptr->base_marker == 0xBA5E,
            std::format("Base data corrupted after implicit conversion. Value: {:x} (expected: 0xBA5E)", 
                        base_ptr ? base_ptr->base_marker : -1) };

        co_yield{ !derived_ptr,
            std::format("derived_ptr should be empty after move, but it is not.") };

        co_yield "reset base_ptr to trigger destruction";
        base_ptr.reset();
        co_yield{ derived_destructor_called,
            std::format("Derived destructor should be called, but it was not.") };
    }
    co_yield nullptr; // Clear scenario 6

    //--------------------------------------------------
    // Scenario 7: shared_ptr with non-virtual destructor in base class
    {
        struct Base { 
            int base_marker = 0xBA5E;
            ~Base() = default; // Non-virtual destructor
        };
        
        struct Derived : Base { 
            int derived_marker = 0xD4D4;
            bool* destructor_called; // Pointer to a flag to track destruction
            Derived(bool* flag) : destructor_called(flag) {}
            ~Derived() { *destructor_called = true; } // Set flag on destruction
        };

        bool derived_destructor_called = false;

        co_yield "Create a shared_ptr<Derived> with a flag to track destruction";
        auto derived_ptr = NAMESPACE_MY make_shared<Derived>(&derived_destructor_called);
        co_yield "and cast it to shared_ptr<Base>";
        NAMESPACE_MY shared_ptr<Base> base_ptr = derived_ptr;

        co_yield{ base_ptr && base_ptr->base_marker == 0xBA5E,
            std::format("Base data corrupted after conversion. Value: {:x} (expected: 0xBA5E)", 
                        base_ptr ? base_ptr->base_marker : -1) };

        co_yield{ derived_ptr.use_count() == 2 && base_ptr.use_count() == 2,
            std::format("Refcount corrupted after conversion. Derived: {} (expected: 2), Base: {} (expected: 2)", 
                        derived_ptr.use_count(), base_ptr.use_count()) };

        co_yield "reset the shared_ptr<Derived> and let shared_ptr<Base> manage the object";
        derived_ptr.reset();
        co_yield{ derived_ptr.use_count() == 0 && base_ptr.use_count() == 1,
            std::format("Refcount corrupted after reset. Derived: {} (expected: 0), Base: {} (expected: 1)", 
                        derived_ptr.use_count(), base_ptr.use_count()) };
        co_yield{ !derived_destructor_called,
            std::format("Derived destructor should not be called, but it was.") };

        co_yield "reset the shared_ptr<Base>";
        base_ptr.reset();
        co_yield{ derived_destructor_called,
            std::format("Derived destructor should be called, but it was not.") };
    }
    co_yield nullptr; // Clear scenario 7
#endif
    co_return;
}
        

    } // namespace my::test
} // namespace my


int main()
{
    my::test::test t;
    t.new_case(my::test::unique_ptr(), "unique_ptr");
    t.new_case(my::test::unique_ptr_ctor(), "unique_ptr constructor");
    t.new_case(my::test::shared_ptr(), "shared_ptr");
    t.new_case(my::test::weak_ptr(), "weak_ptr");
    t.new_case(my::test::enable_shared_from_this(), "enable_shared_from_this");
    t.new_case(my::test::type_casting(), "type_casting");
}
