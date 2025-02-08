#pragma once
#include "yan_type_traits.hpp"
#include <utility>
#include <exception>
#include <format>

namespace my
{

using size_t = std::size_t;

class _alloc_proxy
{
public:
    class memory_leak : public std::exception
    {
    public:
        memory_leak() :
            std::exception("memory leak") {}
        memory_leak(size_t bytes, size_t allocations) :
            std::exception(std::format("memory leaked with {} bytes in {} allocations", bytes, allocations).c_str()) {}
    };

    static _alloc_proxy& get_instance()
    {
        static _alloc_proxy instance;
        return instance;
    }

    // ���� size �ֽڵ��ڴ�
    void* allocate(size_t size)
    {
        void* p = ::operator new(size);
        return p;
    }

    // ����ptrָ����ڴ档Ϊ�˼�¼���ṩӦ�������յ��ֽ�����
    void deallocate(void* ptr, size_t size)
    {
        ::operator delete(ptr);
    }

    void reset()
    {
        if (current_allocated_bytes != 0 || current_allocations != 0)
        {
            throw memory_leak(current_allocated_bytes, current_allocations);
        }
        reset_uncheck();
    }

    size_t current_allocated_bytes; // ��ǰ�ѷ���δ���յ��ֽ���
    size_t total_allocated_bytes;   // �ܹ��ѷ�����ֽ���
    size_t current_allocations;     // ��δ���յķ�������:= allocate��deallocate�ĵ��ô���֮�
    size_t total_allocations;       // �ܹ��ѷ���ķ����� ��:= allocate�ĵ��ô�����

    void reset_uncheck()
    {
        current_allocated_bytes = 0;
        total_allocated_bytes = 0;
        current_allocations = 0;
        total_allocations = 0;
    }
private:
    _alloc_proxy()
        : current_allocated_bytes(0), total_allocated_bytes(0),
          current_allocations(0), total_allocations(0) {}
    ~_alloc_proxy()
    {
    }
    _alloc_proxy(const _alloc_proxy&) = delete;
    _alloc_proxy& operator=(const _alloc_proxy&) = delete;
};

template <class Ptr, class Size = size_t>
struct allocation_result {
    Ptr ptr;
    Size count;
};

template <typename T>
class allocator
{
public:
    // Member types
    using value_type = T;
    using size_type = size_t;
    using difference_type = size_type;
    using propagate_on_container_move_assignment = std::true_type;

    // Member functions
    // ���������n��Ԫ�ص�δ��ʼ�������洢�ռ䡣
    [[nodiscard]] constexpr T* allocate(size_type n)
    {
        return static_cast<T*>(_proxy().allocate(n));
    }
    // �������ٿ�����n��Ԫ�أ�ʵ���Ͽ����ɲ�С��n����С��2���ݸ�Ԫ�ص�δ��ʼ�������洢�ռ䡣
    [[nodiscard]] constexpr allocation_result<T*, size_type>
        allocate_at_least(size_type n)
    {
        size_type actual = std::bit_ceil(n);
        T* p = static_cast<T*>(_proxy().allocate(actual));
        return { p, actual };
    }
    // ����p��ָʾ�ġ�������n��Ԫ�صĴ洢�ռ䡣
    constexpr void deallocate(T* p, size_type n)
    {
        _proxy().deallocate(p, n * sizeof(T));
    }

    // �ж�ͬһ��ģ�嶨��ĸ�������ʵ�����͵����������Ƿ���ȡ�
    template<typename U>
    constexpr bool operator==(const allocator<U>&) const noexcept
    {
        return true;
    }

private:
    static _alloc_proxy& _proxy()
    {
        return _alloc_proxy::get_instance();
    }
};



template <typename Alloc>
struct get_pointer_type {
    using type = Alloc::value_type*;
};

template <typename Alloc> requires requires { typename Alloc::pointer; }
struct get_pointer_type<Alloc> {
    using type = typename Alloc::pointer;
};

template <typename Alloc, typename Pointer>
struct get_const_pointer_type {
    using type = std::pointer_traits<Pointer>::template rebind<const typename Alloc::value_type>;
};

template <typename Alloc, typename Pointer> requires requires { typename Alloc::const_pointer; }
struct get_const_pointer_type<Alloc, Pointer> {
    using type = typename Alloc::const_pointer;
};

template <typename Alloc, typename Pointer>
struct get_void_pointer_type {
    using type = std::pointer_traits<Pointer>::template rebind<void>;
};

template <typename Alloc, typename Pointer> requires requires { typename Alloc::void_pointer; }
struct get_void_pointer_type<Alloc, Pointer> {
    using type = typename Alloc::void_pointer;
};

template <typename Alloc, typename Pointer>
struct get_const_void_pointer_type {
    using type = std::pointer_traits<Pointer>::template rebind<const void>;
};

template <typename Alloc, typename Pointer> requires requires { typename Alloc::const_void_pointer; }
struct get_const_void_pointer_type<Alloc, Pointer> {
    using type = typename Alloc::const_void_pointer;
};

template <typename Alloc, typename Pointer>
struct get_difference_type_type {
    using type = std::pointer_traits<Pointer>::difference_type;
};

template <typename Alloc, typename Pointer> requires requires { typename Alloc::difference_type; }
struct get_difference_type_type<Alloc, Pointer> {
    using type = typename Alloc::difference_type;
};

template <typename Alloc, typename difference_type>
struct get_size_type_type {
    using type = std::make_unsigned<difference_type>::type;
};

template <typename Alloc, typename difference_type> requires requires { typename Alloc::size_type; }
struct get_size_type_type<Alloc, difference_type> {
    using type = typename Alloc::size_type;
};

template <typename Alloc>
struct get_propagate_on_container_copy_assignment_type {
    using type = std::false_type;
};

template <typename Alloc> requires requires { typename Alloc::propagate_on_container_copy_assignment; }
struct get_propagate_on_container_copy_assignment_type<Alloc> {
    using type = typename Alloc::propagate_on_container_copy_assignment;
};

template <typename Alloc>
struct get_propagate_on_container_move_assignment_type {
    using type = std::false_type;
};

template <typename Alloc> requires requires { typename Alloc::propagate_on_container_move_assignment; }
struct get_propagate_on_container_move_assignment_type<Alloc> {
    using type = typename Alloc::propagate_on_container_move_assignment;
};

template <typename Alloc>
struct get_propagate_on_container_swap_type {
    using type = std::false_type;
};

template <typename Alloc> requires requires { typename Alloc::propagate_on_container_swap; }
struct get_propagate_on_container_swap_type<Alloc> {
    using type = typename Alloc::propagate_on_container_swap;
};

template <typename Alloc>
struct get_is_always_equal_type {
    using type = std::is_empty<Alloc>::type;
};

template <typename Alloc> requires requires { typename Alloc::is_always_equal; }
struct get_is_always_equal_type<Alloc> {
    using type = typename Alloc::is_always_equal;
};

template <class Newfirst, class T>
struct replace_first_parameter;

template <class Newfirst, template <class, class...> class T, class _First, class... Rest>
struct replace_first_parameter<Newfirst, T<_First, Rest...>> { // given T<_First, Rest...>, replace _First
    using type = T<Newfirst, Rest...>;
};

template <typename Alloc, typename T>
struct get_rebind_alloc_type {
    using type = typename replace_first_parameter<T, Alloc>::type;
};

template <typename Alloc, typename T> requires requires { typename Alloc::template rebind<T>::other; }
struct get_rebind_alloc_type<Alloc, T> {
    using type = typename Alloc::template rebind<T>::other;
};


template <typename Alloc>
struct allocator_traits
{
    // Member types
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename get_pointer_type<Alloc>::type;
    using const_pointer = typename get_const_pointer_type<Alloc, pointer>::type;
    using void_pointer = typename get_void_pointer_type<Alloc, pointer>::type;
    using const_void_pointer = typename get_const_void_pointer_type<Alloc, pointer>::type;
    using difference_type = typename get_difference_type_type<Alloc, pointer>::type;
    using size_type = typename get_size_type_type<Alloc, difference_type>::type;
    using propagate_on_container_copy_assignment = typename get_propagate_on_container_copy_assignment_type<Alloc>::type;
    using propagate_on_container_move_assignment = typename get_propagate_on_container_move_assignment_type<Alloc>::type;
    using propagate_on_container_swap = typename get_propagate_on_container_swap_type<Alloc>::type;
    using is_always_equal = typename get_is_always_equal_type<Alloc>::type;

    // Member alias templates
    template <typename T>
    using rebind_alloc = get_rebind_alloc_type<Alloc, T>;
    template <typename T>
    using rebind_traits = std::allocator_traits<rebind_alloc<T>>;

    // Member functions
    // ʹ�� a ���� n �� value_type ���͵�Ԫ������Ĵ洢�ռ�
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n)
    {
        return a.allocate(n);
    }

    // �������ʾ���ڴ棨��� allocator û�и÷��������������ʾ�� allocate��
    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
    {
        pointer p;
        if constexpr (requires (Alloc aa) { aa.allocate(n, hint); })
        {
            p = a.allocate(n, hint);
        }
        else
        {
            p = a.allocate(n);
        }
        return p;
    }

    // �������ٿ�����n��Ԫ�ص�δ��ʼ�������洢�ռ䡣Ĭ�Ϸ���{a.allocate(n), n}��
    [[nodiscard]] static constexpr allocation_result<pointer, size_type>
        allocate_at_least(Alloc& a, size_type n)
    {
        if constexpr (requires (Alloc aa) { aa.allocate_at_least(n); })
        {
            return a.allocate_at_least(n);
        }
        else
        {
            return { a.allocate(n), n };
        }
    }

    // �ͷ��ڴ�
    static constexpr void deallocate(Alloc& a, pointer p, size_type n)
    {
        a.deallocate(p, n);
    }

    // ���ڴ��Ϲ������
    template <typename T, typename... Args>
    static constexpr void construct(Alloc& a, T* p, Args&&... args)
    {
        if constexpr (requires (Alloc aa) { aa.construct(p, std::forward<Args>(args)...); })
        {
            a.construct(p, std::forward<Args>(args)...);
        }
        else
        {
            ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
        }
    }

    // ���ٶ���
    template <typename T>
    static constexpr void destroy(Alloc& a, T* p)
    {
        if constexpr (requires (Alloc aa) { aa.destroy(p); })
        {
            a.destroy(p);
        }
        else
        {
            p->~T();
        }
    }

    // ��ȡ���ɷ����Ԫ������
    static constexpr size_type max_size(const Alloc& a) noexcept
    {
        if constexpr (requires (Alloc aa) { aa.max_size(); })
        {
            return a.max_size();
        }
        else
        {
            return std::numeric_limits<size_type>::max() / sizeof(value_type);
        }
    }

    // ����a��select_on_container_copy_construction������
    // ��Allocδʵ�ָú������򷵻�a�����庬�彫�ں���ʵ�����
    static constexpr Alloc select_on_container_copy_construction(const Alloc& a)
    {
        if constexpr (requires (Alloc aa) { aa.select_on_container_copy_construction(); })
        {
            return a.select_on_container_copy_construction();
        }
        else
        {
            return a;
        }
    }
};
}
