#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Matrix4f get_view_matrix(Vector3f eye_pos) {
    Matrix4f view = Matrix4f::Identity();

    Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1, -eye_pos[2],
            0, 0, 0, 1;

    view = translate * view;

    return view;
}

Matrix4f get_model_matrix(float rotation_angle) {
    Matrix4f model = Matrix4f::Identity();
    float rad = rotation_angle * MY_PI / 180.0f;

    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    model(0, 0) = cos(rad);
    model(0, 1) = -sin(rad);
    model(1, 0) = sin(rad);
    model(1, 1) = cos(rad);

    return model;
}

Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                float zNear, float zFar) {
    // Students will implement this function

    Matrix4f projection = Matrix4f::Identity();
    // Create the projection matrix for the given parameters.
    // Then return it.

    // 计算lr,nf,tb的值，随后根据分解矩阵求解
    // 需要注意，t必须是正数，因为t表示近平面上边界在y方向的坐标（高度），这是一个长度，必须是正数
    // 同样，n表示近平面离相机的距离。因为GAMES101中认为相机是看向-z轴，为了表示数轴上的真实位置，可以进行取反处理
    float l, r, n, f, t, b;
    n = -zNear;
    f = -zFar;

    // 注意，n是在数轴上更大的值，因此应该对应t，因为t必须要是正的，因此这里取-n
    t = -n * tan(eye_fov * MY_PI / 180 / 2);
    b = -t;

    r = aspect_ratio * t;
    l = aspect_ratio * b;

    // 构造透视投影转正交投影矩阵
    Matrix4f perspToOrtho = Matrix4f::Zero();
    perspToOrtho(0, 0) = n;
    perspToOrtho(1, 1) = n;
    perspToOrtho(2, 2) = n + f;
    perspToOrtho(2, 3) = - n * f;
    perspToOrtho(3, 2) = 1;

    Matrix4f orthoTranslate = Matrix4f::Identity();
    orthoTranslate(0, 3) = -(l + r) / 2;
    orthoTranslate(1, 3) = -(t + b) / 2;
    orthoTranslate(2, 3) = -(f + n) / 2;

    Matrix4f orthoScale = Matrix4f::Identity();
    orthoScale(0, 0) = 2 / (r - l);
    orthoScale(1, 1) = 2 / (t - b);
    orthoScale(2, 2) = 2 / (n - f);

    // 矩阵合成
    projection = orthoScale * orthoTranslate * perspToOrtho;

    return projection;
}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle) {
    axis.normalize();

    Matrix3f rotation = Matrix3f::Zero();
    // 逐步构造公式
    float rad = angle * MY_PI / 180;

    Matrix3f i = Matrix3f::Identity();
    Matrix3f part1 = cos(rad) * i;

    Matrix3f part2 = (1 - cos(rad)) * (axis * axis.transpose());

    Matrix3f nn = Matrix3f::Zero();
    nn(0, 1) = -axis.z();
    nn(0, 2) = axis.y();
    nn(1, 0) = axis.z();
    nn(1, 2) = -axis.x();
    nn(2, 0) = -axis.y();
    nn(2, 1) = axis.x();
    Matrix3f part3 = sin(rad) * nn;

    rotation = part1 + part2 + part3;

    Matrix4f result = Matrix4f::Identity();
    result.block<3, 3>(0, 0) = rotation;

    return result;
}

int main(int argc, const char **argv) {
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
    }

    rst::rasterizer r(700, 700);

    Vector3f eye_pos = {0, 0, 5};

    std::vector<Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        } else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
