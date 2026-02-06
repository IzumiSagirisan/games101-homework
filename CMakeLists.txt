cmake_minimum_required(VERSION 3.16)
project(games101_homework)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 默认使用Debug模式
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# 默认使用全部导出
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# 可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/RelWithDebInfo)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/bin/MinSizeRel)

# 动态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/RelWithDebInfo)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/bin/MinSizeRel)

# 静态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/RelWithDebInfo)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/bin/MinSizeRel)

# 在 MSVC 下为 Release/RelWithDebInfo 生成 PDB，便于问题定位
if (MSVC)
    add_compile_options($<$<CONFIG:Release>:/Zi>)
    add_link_options($<$<CONFIG:Release>:/DEBUG>)
    add_link_options($<$<CONFIG:RelWithDebInfo>:/DEBUG>)
endif ()

# 允许在构建的可执行文件中写入rpath
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

# 添加cmake自定义模块路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/src/CMake")

# -------------------------------------------------------------
# 全局公共配置 & 第三方依赖
# 必须在 add_subdirectory() 之前引入，保证子模块能直接使用接口库/宏
# -------------------------------------------------------------
include(src/CMake/main.cmake)

# -------------------------------------------------------------
# 添加子目录
# -------------------------------------------------------------
# 各个作业模块 - 每个作业生成一个独立的 exe
# 取消注释以启用对应作业
add_subdirectory(src/homework0)
add_subdirectory(src/homework1)
add_subdirectory(src/homework2)
# add_subdirectory(src/homework3)
# add_subdirectory(src/homework4)
# add_subdirectory(src/homework5)
# add_subdirectory(src/homework6)
# add_subdirectory(src/homework7)
