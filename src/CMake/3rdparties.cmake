## 第三方库统一配置文件

# 平台化 3rd 根目录
if (WIN32)
    set(_3RD_ROOT ${CMAKE_SOURCE_DIR}/src/3rd/windows)
elseif (UNIX)
    set(_3RD_ROOT ${CMAKE_SOURCE_DIR}/src/3rd/linux)
else ()
    message(FATAL_ERROR "Unsupported platform for 3rd dependencies")
endif ()

# -------------------------------------------------------------
# Eigen 配置
# Eigen 是 header-only 库，只需设置头文件路径即可
# 使用方式：下载 Eigen 到 src/3rd/windows/eigen 或 src/3rd/linux/eigen
# 或者使用系统已安装的 Eigen（通过 find_package）
# -------------------------------------------------------------
option(USE_SYSTEM_EIGEN "Use system installed Eigen library" OFF)

if (USE_SYSTEM_EIGEN)
    # 使用系统安装的 Eigen
    find_package(Eigen3 3.3 REQUIRED NO_MODULE)
    message(STATUS "Using system Eigen: ${EIGEN3_INCLUDE_DIR}")
else ()
    # 使用本地 Eigen（需要手动下载到 3rd 目录）
    set(EIGEN3_INCLUDE_DIR ${_3RD_ROOT}/eigen)
    if (EXISTS ${EIGEN3_INCLUDE_DIR})
        message(STATUS "Using local Eigen: ${EIGEN3_INCLUDE_DIR}")
        # 创建一个接口库，方便 target_link_libraries 使用
        add_library(Eigen3::Eigen INTERFACE IMPORTED)
        set_target_properties(Eigen3::Eigen PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${EIGEN3_INCLUDE_DIR}
        )
    else ()
        message(STATUS "Eigen not found at ${EIGEN3_INCLUDE_DIR}")
        message(STATUS "To use Eigen:")
        message(STATUS "  1. Download Eigen from https://eigen.tuxfamily.org/")
        message(STATUS "  2. Extract to ${EIGEN3_INCLUDE_DIR}")
        message(STATUS "  Or set USE_SYSTEM_EIGEN=ON to use system Eigen")
    endif ()
endif ()