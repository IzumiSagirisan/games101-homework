# homework0 - 向量与矩阵基础
# 生成可执行文件：homework0.exe

# 获取所有源文件
file(GLOB_RECURSE HW0_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
file(GLOB_RECURSE HW0_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# 创建可执行文件
add_executable(homework0 ${HW0_SOURCES} ${HW0_HEADERS})

# 包含头文件目录
target_include_directories(homework0 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接 Eigen（如果已配置）
if (TARGET Eigen3::Eigen)
    target_link_libraries(homework0 PRIVATE Eigen3::Eigen)
endif ()
