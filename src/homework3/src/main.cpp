#include <iostream>
#include <opencv2/opencv.hpp>

#include "global.hpp"
#include "rasterizer.hpp"
#include "Triangle.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "OBJ_Loader.h"

Matrix4f get_view_matrix(Vector3f eye_pos)
{
    Matrix4f view = Matrix4f::Identity();

    Matrix4f translate;
    translate << 1,0,0,-eye_pos[0],
                 0,1,0,-eye_pos[1],
                 0,0,1,-eye_pos[2],
                 0,0,0,1;

    view = translate*view;

    return view;
}

Matrix4f get_model_matrix(float angle)
{
    Matrix4f rotation;
    angle = angle * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
                0, 1, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1;

    Matrix4f scale;
    scale << 2.5, 0, 0, 0,
              0, 2.5, 0, 0,
              0, 0, 2.5, 0,
              0, 0, 0, 1;

    Matrix4f translate;
    translate << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return translate * rotation * scale;
}

Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // TODO: Use the same projection matrix from the previous assignments
    Matrix4f projection = Matrix4f::Identity();
    // 计算lr,nf,tb的值，随后根据分解矩阵求解
    // 需要注意，t必须是正数，因为t表示近平面上边界在y方向的坐标（高度），这是一个长度，必须是正数
    // 同样，n表示近平面离相机的距离。因为GAMES101中认为相机是看向-z轴，为了表示数轴上的真实位置，可以进行取反处理
    float l, r, n, f, t, b;
    // 这里可以取反，表示坐标值
    n = zNear;
    f = zFar;

    // 注意，n是在数轴上更大的值，因此应该对应t，因为t必须要是正的，因此这里取-n
    // t,b 表示视锥体的上下边界，应该使用距离（正数）计算
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

Vector3f vertex_shader(const vertex_shader_payload& payload)
{
    return payload.position;
}

Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    Vector3f return_color = (payload.normal.head<3>().normalized() + Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    Vector3f result;
    result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    return result;
}

static Vector3f reflect(const Vector3f& vec, const Vector3f& axis)
{
    auto costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}

struct light
{
    Vector3f position;
    Vector3f intensity;
};

Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
    Vector3f return_color = {0, 0, 0};
    if (payload.texture)
    {
        // TODO: Get the texture value at the texture coordinates of the current fragment

    }
    Vector3f texture_color;
    texture_color << return_color.x(), return_color.y(), return_color.z();

    Vector3f ka = Vector3f(0.005, 0.005, 0.005);
    Vector3f kd = texture_color / 255.f;
    Vector3f ks = Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Vector3f amb_light_intensity{10, 10, 10};
    Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Vector3f color = texture_color;
    Vector3f point = payload.view_pos;
    Vector3f normal = payload.normal;

    Vector3f result_color = {0, 0, 0};

    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.

    }

    return result_color * 255.f;
}

Vector3f phong_fragment_shader(const fragment_shader_payload& payload)
{
    Vector3f ka = Vector3f(0.005, 0.005, 0.005);
    Vector3f kd = payload.color;
    Vector3f ks = Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Vector3f amb_light_intensity{10, 10, 10};
    Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Vector3f color = payload.color;
    Vector3f point = payload.view_pos;
    Vector3f normal = payload.normal;

    Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        
    }

    return result_color * 255.f;
}



Vector3f displacement_fragment_shader(const fragment_shader_payload& payload)
{
    
    Vector3f ka = Vector3f(0.005, 0.005, 0.005);
    Vector3f kd = payload.color;
    Vector3f ks = Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Vector3f amb_light_intensity{10, 10, 10};
    Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Vector3f color = payload.color; 
    Vector3f point = payload.view_pos;
    Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;
    
    // TODO: Implement displacement mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Position p = p + kn * n * h(u,v)
    // Normal n = normalize(TBN * ln)


    Vector3f result_color = {0, 0, 0};

    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.


    }

    return result_color * 255.f;
}


Vector3f bump_fragment_shader(const fragment_shader_payload& payload)
{
    
    Vector3f ka = Vector3f(0.005, 0.005, 0.005);
    Vector3f kd = payload.color;
    Vector3f ks = Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Vector3f amb_light_intensity{10, 10, 10};
    Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Vector3f color = payload.color; 
    Vector3f point = payload.view_pos;
    Vector3f normal = payload.normal;


    float kh = 0.2, kn = 0.1;

    // TODO: Implement bump mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Normal n = normalize(TBN * ln)


    Vector3f result_color = {0, 0, 0};
    result_color = normal;

    return result_color * 255.f;
}

int main(int argc, const char** argv)
{
    std::vector<Triangle*> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = "output.png";
    objl::Loader Loader;
    std::string obj_path = "../models/spot/";

    // Load .obj File
    bool loadout = Loader.LoadFile("../models/spot/spot_triangulated_good.obj");
    for(auto mesh:Loader.LoadedMeshes)
    {
        for(int i=0;i<mesh.Vertices.size();i+=3)
        {
            Triangle* t = new Triangle();
            for(int j=0;j<3;j++)
            {
                t->setVertex(j,Vector4f(mesh.Vertices[i+j].Position.X,mesh.Vertices[i+j].Position.Y,mesh.Vertices[i+j].Position.Z,1.0));
                t->setNormal(j,Vector3f(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z));
                t->setTexCoord(j,Vector2f(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y));
            }
            TriangleList.push_back(t);
        }
    }

    rst::rasterizer r(700, 700);

    auto texture_path = "hmap.jpg";
    r.set_texture(Texture(obj_path + texture_path));

    std::function<Vector3f(fragment_shader_payload)> active_shader = phong_fragment_shader;

    if (argc >= 2)
    {
        command_line = true;
        filename = std::string(argv[1]);

        if (argc == 3 && std::string(argv[2]) == "texture")
        {
            std::cout << "Rasterizing using the texture shader\n";
            active_shader = texture_fragment_shader;
            texture_path = "spot_texture.png";
            r.set_texture(Texture(obj_path + texture_path));
        }
        else if (argc == 3 && std::string(argv[2]) == "normal")
        {
            std::cout << "Rasterizing using the normal shader\n";
            active_shader = normal_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "phong")
        {
            std::cout << "Rasterizing using the phong shader\n";
            active_shader = phong_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "bump")
        {
            std::cout << "Rasterizing using the bump shader\n";
            active_shader = bump_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "displacement")
        {
            std::cout << "Rasterizing using the bump shader\n";
            active_shader = displacement_fragment_shader;
        }
    }

    Vector3f eye_pos = {0,0,10};

    r.set_vertex_shader(vertex_shader);
    r.set_fragment_shader(active_shader);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        r.draw(TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imwrite(filename, image);

        return 0;
    }

    while(key != 27)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        //r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
        r.draw(TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imshow("image", image);
        cv::imwrite(filename, image);
        key = cv::waitKey(10);

        if (key == 'a' )
        {
            angle -= 0.1;
        }
        else if (key == 'd')
        {
            angle += 0.1;
        }

    }
    return 0;
}
