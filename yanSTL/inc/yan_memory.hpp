#pragma once

// 当完成相关实现时，删除对应的宏定义。
#define DISMISS_UNIQUE_PTR
#define DISMISS_SHARED_AND_WEAK_PTR
#define DISMISS_ENABLE_SHARED_FROM_THIS
#define DISMISS_TYPE_CASTING

#include "memory/common.hpp"

#ifndef DISMISS_UNIQUE_PTR
#include "memory/unique_ptr.hpp"
#endif

#ifndef DISMISS_SHARED_AND_WEAK_PTR
#include "memory/shared_ptr.hpp"
#endif
