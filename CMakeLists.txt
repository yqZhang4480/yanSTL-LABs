# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("yanSTL")

include_directories("yanSTL/inc")
include_directories("tests/inc")

add_executable(hello "tests/src/test.hello.cpp")
add_executable(lab1  "tests/src/test.type_traits.cpp")
add_executable(lab2  "tests/src/test.allocator.cpp")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_link_libraries(lab1 PRIVATE pthread)
  target_link_libraries(lab2 PRIVATE pthread)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hello PROPERTY CXX_STANDARD 20)
  set_property(TARGET lab1 PROPERTY CXX_STANDARD 20)
  set_property(TARGET lab2 PROPERTY CXX_STANDARD 20)
endif()
