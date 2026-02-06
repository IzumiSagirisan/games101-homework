# homework2
# 生成可执行文件：homework2.exe

# 获取所有源文件
file(GLOB_RECURSE HW2_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
file(GLOB_RECURSE HW2_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# 创建可执行文件
add_executable(homework2 ${HW2_SOURCES} ${HW2_HEADERS})

# 包含头文件目录
target_include_directories(homework2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接 Eigen
if (TARGET Eigen3::Eigen)
    target_link_libraries(homework2 PRIVATE Eigen3::Eigen)
endif ()

# 链接 OpenCV
if (OpenCV_FOUND)
    target_include_directories(homework2 PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(homework2 PRIVATE ${OpenCV_LIBS})
endif ()
