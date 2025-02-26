#pragma once
#include <memory>
#include <utility>
#include <atomic>
#include <stdexcept>

namespace my {

#ifndef DISMISS_UNIQUE_PTR
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr;
#endif

#ifndef DISMISS_SHARED_AND_WEAK_PTR
template <typename T>
class shared_ptr;

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args);

template <typename T>
class weak_ptr;
#endif

#ifndef DISMISS_ENABLE_SHARED_FROM_THIS
template <typename T>
class enable_shared_from_this;
#endif



} // namespace my