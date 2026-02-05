/*************************************************
 * 描述：
 *
 * File：src/homework0/include:eigenexample
 * Author：arcstar
 * Date：2026-02-05
 * Update：
 * ************************************************/
#include "../include/eigenexample.h"

#include <Eigen/Dense>
#include <iostream>


EigenExample::EigenExample() {
}

void EigenExample::print() {
    // Example of vector
    std::cout << "Example of vector" << std::endl;

    // 定义向量
    Eigen::Vector3f v(1.0f, 2.0f, 3.0f);
    Eigen::Vector3f w(1.0f, 0.0f, 0.0f);

    // 输出向量
    std::cout << "Example of output" << std::endl;
    std::cout << v << std::endl;

    // 向量加
    std::cout << "Example of add" << std::endl;
    std::cout << v + w << std::endl;

    // 向量标量相乘
    std::cout << "Example of scalar multiply" << std::endl;
    std::cout << v * 3.0f << std::endl;
    std::cout << 2.0f * v << std::endl;

    // 向量点积
    std::cout << "Example of matrix multiply" << std::endl;
    std::cout << v.cross(w) << std::endl;

    // Example of matrix
    std::cout << "Example of matrix" << std::endl;

    // 矩阵定义
    Eigen::Matrix3f i, j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;

    // 矩阵输出
    std::cout << "Example of output" << std::endl;
    std::cout << i << std::endl;

    // 矩阵加法
    std::cout << "Example of matrix add" << std::endl;
    std::cout << i + j << std::endl;
    std::cout << "Example of scalar multiply" << std::endl;
    std::cout << i * 2.0 << std::endl;
    std::cout << "Example of matrix multiply" << std::endl;
    std::cout << i * j << std::endl;
    std::cout << "Example of matrix vector multiply" << std::endl;
    std::cout << i * v << std::endl;
}
