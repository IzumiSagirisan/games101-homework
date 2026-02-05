# homework1 - 光栅化器
# 生成可执行文件：homework1.exe

# 获取所有源文件
file(GLOB_RECURSE HW1_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
file(GLOB_RECURSE HW1_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# 创建可执行文件
add_executable(homework1 ${HW1_SOURCES} ${HW1_HEADERS})

# 包含头文件目录
target_include_directories(homework1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接 Eigen
if (TARGET Eigen3::Eigen)
    target_link_libraries(homework1 PRIVATE Eigen3::Eigen)
endif ()

# 链接 OpenCV
if (OpenCV_FOUND)
    target_include_directories(homework1 PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(homework1 PRIVATE ${OpenCV_LIBS})
endif ()
