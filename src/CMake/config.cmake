# 平台宏定义
if (WIN32)
    add_compile_definitions(PLAT_WIN)
elseif (UNIX)
    add_compile_definitions(PLAT_LINUX)
endif ()

# 是否开启Debug 代码
option(IS_SHOW_DEBUG_CODE "is show debug info" ON)
#添加Debug宏
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND IS_SHOW_DEBUG_CODE)
    add_compile_definitions(DEBUG)
endif ()

# 编译选项 设置编译字符集为utf-8
if (MSVC)
    add_compile_options("/utf-8")
endif ()