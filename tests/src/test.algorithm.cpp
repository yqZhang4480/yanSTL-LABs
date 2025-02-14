#include "co_yantest.hpp"
#include "int_wrapper.hpp"
#include "yan_algorithm.hpp"
#include <algorithm>
#include <numeric>
#include <random>
#include "tabulate/table.hpp"

//#define USE_STD

#ifdef USE_STD
#define NAMESPACE_MY ::std::
#undef DISMISS_FIND
#undef DISMISS_COUNT_IF
#undef DISMISS_TRANSFORM
#undef DISMISS_ACCUMULATE
#undef DISMISS_MISMATCH
#undef DISMISS_LOWER_BOUND
#undef DISMISS_PARTITION
#undef DISMISS_NTH_ELEMENT
#undef DISMISS_SORT
#undef DISMISS_NEXT_PERMUTATION
#else
#define NAMESPACE_MY ::my::
#endif

namespace my
{
namespace test
{
std::vector<int_wrapper> generate_data(size_t size, int mode) {
    std::vector<int_wrapper> data(size);
    for (size_t i = 0; i < size; ++i) {
        data[i] = int_wrapper(i);
    }

    if (mode == 0) { // 随机
        std::shuffle(data.begin(), data.end(), std::mt19937(std::random_device{}()));
    }
    else if (mode == 1) { // 已排序
        // 不做任何改变
    }
    else if (mode == 2) { // 逆序
        std::reverse(data.begin(), data.end());
    }
    else if (mode == 3) { // 绝大部分相等
        for (size_t i = 0; i < size; ++i) {
            data[i] = int_wrapper((i % 10) / 9);
        }
    }
    return data;
}

// 运行测试
template <size_t N>
void run_sort_test(const std::string& test_name, std::vector<int_wrapper> data, tabulate::Table& table) {
    double my_sort_time;
    int my_copy_count;
    int my_move_count;
    int my_compare_count;

    double std_sort_time;
    int std_copy_count;
    int std_move_count;
    int std_compare_count;

    auto data2 = data;
    {
        auto c = int_wrapper::counter_scope();
        auto start = std::chrono::high_resolution_clock::now();
        if constexpr (N == 0)
        {
            std::sort(data2.begin(), data2.end());
        }
        else if (N == 1)
        {
            std::partition(data2.begin(), data2.end(), [&p = data2[0]](const auto& e) { return e < p; });
        }
        else if (N == 2)
        {
            std::nth_element(data2.begin(), data2.begin() + data2.size() / 2, data2.end());
        }
        else if (N == 3)
        {
            std::next_permutation(data2.begin(), data2.end());
        }
        auto end = std::chrono::high_resolution_clock::now();
        std_sort_time = std::chrono::duration<double, std::milli>(end - start).count();
        std_copy_count = int_wrapper::copy_count;
        std_move_count = int_wrapper::move_count;
        std_compare_count = int_wrapper::compare_count;
    }

    {
        auto c = int_wrapper::counter_scope();
        auto start = std::chrono::high_resolution_clock::now();
        if constexpr (N == 0)
        {
#ifndef DISMISS_SORT
            NAMESPACE_MY sort(data.begin(), data.end());
#endif // !DISMISS_SORT
        }
        else if (N == 1)
        {
#ifndef DISMISS_PARTITION
            NAMESPACE_MY partition(data.begin(), data.end(), [&p = data[0]](const auto& e) { return e < p; });
#endif // !DISMISS_PARTITION
        }
        else if (N == 2)
        {
#ifndef DISMISS_NTH_ELEMENT
            NAMESPACE_MY nth_element(data.begin(), data.begin() + data.size() / 2, data.end());
#endif // !DISMISS_NTH_ELEMENT
        }
        else if (N == 3)
        {
#ifndef DISMISS_NEXT_PERMUTATION
            NAMESPACE_MY next_permutation(data.begin(), data.end());
#endif // !DISMISS_NEXT_PERMUTATION
        }

        auto end = std::chrono::high_resolution_clock::now();
        my_sort_time = std::chrono::duration<double, std::milli>(end - start).count();
        my_copy_count = int_wrapper::copy_count;
        my_move_count = int_wrapper::move_count;
        my_compare_count = int_wrapper::compare_count;
    }

    if (N != 2 && N != 1)
    {
        if (std::mismatch(data.begin(), data.end(), data2.begin()).first != data.end())
        {
            throw 0;
        }
    }

    table.add_row({ test_name,
                    (my_copy_count <= std_copy_count ? "[*] " : "[ ] ") + std::to_string(my_copy_count),
                    std::to_string(std_copy_count),
                    (my_move_count <= std_move_count ? "[*] " : "[ ] ") + std::to_string(my_move_count),
                    std::to_string(std_move_count),
                    (my_compare_count <= std_compare_count ? "[*] " : "[ ] ") + std::to_string(my_compare_count),
                    std::to_string(std_compare_count),
                    (my_sort_time < std_sort_time ? "[*] " : "[ ] ") + std::format("{:.2f}", my_sort_time),
                    std::format("{:.2f}", std_sort_time) });
}

template <size_t N>
bool run_benchmark()
{
    try
    {
        tabulate::Table table;
        table.add_row({ "Test", "Copies", "", "Moves", "", "Compares", "", "Time(ms)", "" });
        table.add_row({ "", "MINE", "STD", "MINE", "STD", "MINE", "STD", "MINE", "STD" });
        run_sort_test<N>("Random 1E3", generate_data(1000, 0), table);
        run_sort_test<N>("Random 1E4", generate_data(10000, 0), table);
        run_sort_test<N>("Sorted 1E3", generate_data(1000, 1), table);
        run_sort_test<N>("Sorted 1E4", generate_data(10000, 1), table);
        run_sort_test<N>("Reversed 1E3", generate_data(1000, 2), table);
        run_sort_test<N>("Reversed 1E4", generate_data(10000, 2), table);
        run_sort_test<N>("MostlyEq 1E2", generate_data(100, 3), table);
        run_sort_test<N>("MostlyEq 1E3", generate_data(1000, 3), table);

        for (size_t row = 0; row < 10; ++row) {
            if (row == 0 || row == 1) {
                for (size_t col = 0; col < 9; ++col) {
                    table[row][col].format().font_style({ tabulate::FontStyle::italic });
                }
            }

            if (row > 0) {
                table[row][0].format().font_style({ tabulate::FontStyle::italic });
                for (size_t col = 1; col < 9; col += 2) {
                    table[row][col].format().font_color({ tabulate::Color::yellow });
                }
            }
        }

        std::cerr << "\n" << table << std::endl;
        return true;
    }
    catch (...)
    {
        return false;
    }

}


}
}

namespace my
{
namespace test
{
case_t find() {
#ifdef DISMISS_FIND
    co_yield{ case_t::state::DISMISSED, "test for `find` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY find for integer 3 in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        auto it = NAMESPACE_MY find(v.begin(), v.end(), 3);
        co_yield{ it != v.end() && *it == 3,
            std::format("Expected to find 3, but found {}.", *it) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY find for missing integer 6 in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        auto it = NAMESPACE_MY find(v.begin(), v.end(), 6);
        co_yield{ it == v.end(), "Expected to not find 6, but found it." };
    }
    co_yield nullptr;

    co_return;
#endif
}

case_t count_if() {
#ifdef DISMISS_COUNT_IF
    co_yield{ case_t::state::DISMISSED, "test for `count_if` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY count_if for even numbers in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        int count = NAMESPACE_MY count_if(v.begin(), v.end(), [](int val) { return val % 2 == 0; });
        co_yield{ count == 2, std::format("Expected 2 even numbers, but got {}.", count) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY count_if for numbers greater than 3 in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        int count = NAMESPACE_MY count_if(v.begin(), v.end(), [](int val) { return val > 3; });
        co_yield{ count == 2, std::format("Expected 2 numbers greater than 3, but got {}.", count) };
    }
    co_yield nullptr;

    co_return;
#endif
}

case_t transform() {
#ifdef DISMISS_TRANSFORM
    co_yield{ case_t::state::DISMISSED, "test for `transform` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY transform to square each element in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        std::vector<int> result(v.size());
        NAMESPACE_MY transform(v.begin(), v.end(), result.begin(), [](int val) { return val * val; });
        co_yield{ result == std::vector<int>{1, 4, 9, 16, 25},
            "Expected squares of [1,2,3,4,5], but got different result." };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY transform to double each element in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        std::vector<int> result(v.size());
        NAMESPACE_MY transform(v.begin(), v.end(), result.begin(), [](int val) { return val * 2; });
        co_yield{ result == std::vector<int>{2, 4, 6, 8, 10},
            "Expected double of [1,2,3,4,5], but got different result." };
    }
    co_yield nullptr;
#endif
    co_return;
}

case_t accumulate() {
#ifdef DISMISS_ACCUMULATE
    co_yield{ case_t::state::DISMISSED, "test for `accumulate` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY accumulate with integer summation.";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        int sum = NAMESPACE_MY accumulate(v.begin(), v.end(), 0);
        co_yield{ sum == 15,
            std::format("Sum of [1,2,3,4,5] should be 15, but got {}.", sum) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY accumulate with multiplication.";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        int product = NAMESPACE_MY accumulate(v.begin(), v.end(), 1, std::multiplies<>());
        co_yield{ product == 120,
            std::format("Product of [1,2,3,4,5] should be 120, but got {}.", product) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY accumulate with custom lambda operation.";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        int custom_result = NAMESPACE_MY accumulate(v.begin(), v.end(), 0, [](int acc, int val) {
            return acc + val * val; // Sum of squares
            });
        co_yield{ custom_result == 55,
            std::format("Sum of squares of [1,2,3,4,5] should be 55, but got {}.", custom_result) };
    }
    co_yield nullptr;
#endif

    co_return;
}

case_t mismatch() {
#ifdef DISMISS_MISMATCH
    co_yield{ case_t::state::DISMISSED, "test for `mismatch` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY mismatch for first mismatch between [1,2,3] and [1,2,4].";
    {
        std::vector<int> v1 = { 1, 2, 3 };
        std::vector<int> v2 = { 1, 2, 4 };
        auto result = NAMESPACE_MY mismatch(v1.begin(), v1.end(), v2.begin());
        co_yield{ result.first != v1.end() && result.second != v2.end() && *result.first == 3 && *result.second == 4,
            std::format("Expected mismatch at [3] vs [4], but got {} vs {}.", *result.first, *result.second) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY mismatch for no mismatch between [1,2,3] and [1,2,3].";
    {
        std::vector<int> v1 = { 1, 2, 3 };
        std::vector<int> v2 = { 1, 2, 3 };
        auto result = NAMESPACE_MY mismatch(v1.begin(), v1.end(), v2.begin());
        co_yield{ result.first == v1.end() && result.second == v2.end(),
            "Expected no mismatch, but found one." };
    }
    co_yield nullptr;
#endif
    co_return;
}

case_t lower_bound() {
#ifdef DISMISS_LOWER_BOUND
    co_yield{ case_t::state::DISMISSED, "test for `lower_bound` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY lower_bound for 4 in sorted [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        auto it = NAMESPACE_MY lower_bound(v.begin(), v.end(), 4);
        co_yield{ it != v.end() && *it == 4, std::format("Expected lower bound for 4, but found {}.", *it) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY lower_bound for 6 in sorted [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        auto it = NAMESPACE_MY lower_bound(v.begin(), v.end(), 6);
        co_yield{ it == v.end(), "Expected lower bound for 6 to be end, but it wasn't." };
    }
    co_yield nullptr;
#endif

    co_return;
}

case_t partition() {
#ifdef DISMISS_PARTITION
    co_yield{ case_t::state::DISMISSED, "test for `partition` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY partition for even numbers in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        auto it = NAMESPACE_MY partition(v.begin(), v.end(), [](int val) { return val % 2 == 0; });
        co_yield{ it == v.begin() + 2, "Partitioning at the wrong point." };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY partition for greater than 3 in [1,2,3,4,5].";
    {
        std::vector<int> v = { 1, 2, 3, 4, 5 };
        auto it = NAMESPACE_MY partition(v.begin(), v.end(), [](int val) { return val > 3; });
        co_yield{ it == v.begin() + 2, "Partitioning at the wrong point." };
    }

    co_yield nullptr;

    co_yield{ run_benchmark<1>(), "run benchmark failed." };
#endif
    co_return;
}

case_t nth_element() {
#ifdef DISMISS_NTH_ELEMENT
    co_yield{ case_t::state::DISMISSED, "test for `nth_element` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY nth_element for 3rd smallest element in [5,4,3,2,1].";
    {
        std::vector<int> v = { 5, 4, 3, 2, 1 };
        NAMESPACE_MY nth_element(v.begin(), v.begin() + 2, v.end());
        co_yield{ v[2] == 3, std::format("Expected 3rd element to be 3, but got {}.", v[2]) };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY nth_element for 1st smallest element in [5,4,3,2,1].";
    {
        std::vector<int> v = { 5, 4, 3, 2, 1 };
        NAMESPACE_MY nth_element(v.begin(), v.begin(), v.end());
        co_yield{ v[0] == 1, std::format("Expected 1st element to be 1, but got {}.", v[0]) };
    }
    co_yield nullptr;

    co_yield{ run_benchmark<2>(), "run benchmark failed." };

#endif

    co_return;
}

case_t sort() {
#ifdef DISMISS_SORT
    co_yield{ case_t::state::DISMISSED, "test for `sort` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY sort on unsorted [5,4,3,2,1].";
    {
        std::vector<int> v = { 5, 4, 3, 2, 1 };
        NAMESPACE_MY sort(v.begin(), v.end());
        co_yield{ v == std::vector<int>{1, 2, 3, 4, 5}, "Expected sorted order [1,2,3,4,5], but got different." };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY sort with custom comparator for descending order.";
    {
        std::vector<int> v = { 5, 4, 3, 2, 1 };
        NAMESPACE_MY sort(v.begin(), v.end(), std::greater<>());
        co_yield{ v == std::vector<int>{5, 4, 3, 2, 1}, "Expected descending order [5,4,3,2,1], but got different." };
    }
    co_yield nullptr;

    co_yield{ run_benchmark<0>(), "run benchmark failed." };

#endif

    co_return;
}

case_t next_permutation() {
#ifdef DISMISS_NEXT_PERMUTATION
    co_yield{ case_t::state::DISMISSED, "test for `next_permutation` has been dismissed." };
#else
    co_yield "Testing NAMESPACE_MY next_permutation on [1,2,3].";
    {
        std::vector<int> v = { 1, 2, 3 };
        bool has_next = NAMESPACE_MY next_permutation(v.begin(), v.end());
        co_yield{ has_next && v == std::vector<int>{1, 3, 2}, "Expected next permutation [1,3,2], but got different." };
    }
    co_yield nullptr;

    co_yield "Testing NAMESPACE_MY next_permutation on last permutation [3,2,1].";
    {
        std::vector<int> v = { 3, 2, 1 };
        bool has_next = NAMESPACE_MY next_permutation(v.begin(), v.end());
        co_yield{ !has_next && v == std::vector<int>{1, 2, 3}, "Expected no next permutation, but got one." };
    }
    co_yield nullptr;

#endif

    co_return;
}

}
}

int main()
{
    my::test::test t;
    t.new_case(my::test::find(), "FIND");
    t.new_case(my::test::count_if(), "COUNT_IF");
    t.new_case(my::test::transform(), "TRANSFORM");
    t.new_case(my::test::accumulate(), "ACCUMULATE");
    t.new_case(my::test::mismatch(), "MISMATCH");
    t.new_case(my::test::lower_bound(), "LOWER_BOUND");
    t.new_case(my::test::partition(), "PARTITION");
    t.new_case(my::test::nth_element(), "NTH_ELEMENT");
    t.new_case(my::test::sort(), "SORT");
    t.new_case(my::test::next_permutation(), "NEXT_PERMUTATION");

}