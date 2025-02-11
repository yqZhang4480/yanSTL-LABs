#include "co_yantest.hpp"
#include "int_wrapper.hpp"
#include "yan_memory.hpp"
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

case_t unique_ptr() {
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

    co_yield "Testing custom deleter...";
    bool deleter_called = false;
    auto custom_deleter = [&](int* ptr) { delete ptr; deleter_called = true; };
    {
        NAMESPACE_MY unique_ptr<int, decltype(custom_deleter)> p10(new int(99), custom_deleter);
    }
    co_yield{ deleter_called, std::format("Custom deleter should be called on destruction, but it seems not.") };

    co_yield nullptr;
    co_return;
}

case_t shared_ptr() {
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
    co_return;
}

case_t weak_ptr() {
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
    co_return;
}


case_t enable_shared_from_this() {
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
    co_return;
}

    } // namespace my::test
} // namespace my


int main()
{
    my::test::test t;
    t.new_case(my::test::unique_ptr(), "unique_ptr");
    t.new_case(my::test::shared_ptr(), "shared_ptr");
    t.new_case(my::test::weak_ptr(), "weak_ptr");
}