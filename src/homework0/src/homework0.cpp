/*************************************************
 * 描述：
 *
 * File：src/homework0/include:homework0
 * Author：liangyunfei@mozihealthcare.cn
 * Date：2026-02-05
 * Update：
 * ************************************************/
#include "../include/homework0.h"

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>

#define M_PI 3.1415926

Homework0::Homework0() {
}

void Homework0::run() {
    Eigen::Vector3f point;
    point.x() = 2;
    point.y() = 1;
    point.z() = 1;

    // 构造逆时针旋转45°矩阵
    double angle = 45 * M_PI / 180.0;
    Eigen::Matrix3f rotationMatrix = Eigen::Matrix3f::Identity();
    rotationMatrix(0, 0) = static_cast<float>(cos(angle));
    rotationMatrix(0, 1) = static_cast<float>(-sin(angle));
    rotationMatrix(1, 0) = static_cast<float>(sin(angle));
    rotationMatrix(1, 1) = static_cast<float>(cos(angle));

    // 左乘矩阵
    Eigen::Vector3f rotateResult = rotationMatrix * point;

    // 构造平移矩阵
    Eigen::Matrix3f translationMatrix = Eigen::Matrix3f::Identity();
    translationMatrix(0, 2) = static_cast<float>(1.0);
    translationMatrix(1, 2) = static_cast<float>(2.0);

    // 左乘矩阵
    Eigen::Vector3f translationResult = translationMatrix * rotateResult;

    // 输出坐标
    std::cout << "x: " << translationResult.x() << " y: " << translationResult.y() << std::endl;
}
