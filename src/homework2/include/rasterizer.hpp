//
// Created by goksu on 4/6/19.
//

#pragma once

#include <Eigen/Eigen>
#include <algorithm>
#include "global.hpp"
#include "Triangle.hpp"
using namespace Eigen;

namespace rst
{
    enum class Buffers
    {
        Color = 1,
        Depth = 2
    };

    inline Buffers operator|(Buffers a, Buffers b)
    {
        return Buffers((int)a | (int)b);
    }

    inline Buffers operator&(Buffers a, Buffers b)
    {
        return Buffers((int)a & (int)b);
    }

    enum class Primitive
    {
        Line,
        Triangle
    };

    /*
     * For the curious : The draw function takes two buffer id's as its arguments. These two structs
     * make sure that if you mix up with their orders, the compiler won't compile it.
     * Aka : Type safety
     * */
    struct pos_buf_id
    {
        int pos_id = 0;
    };

    struct ind_buf_id
    {
        int ind_id = 0;
    };

    struct col_buf_id
    {
        int col_id = 0;
    };

    class rasterizer
    {
    public:
        rasterizer(int w, int h);
        pos_buf_id load_positions(const std::vector<Vector3f>& positions);
        ind_buf_id load_indices(const std::vector<Vector3i>& indices);
        col_buf_id load_colors(const std::vector<Vector3f>& colors);

        void set_model(const Matrix4f& m);
        void set_view(const Matrix4f& v);
        void set_projection(const Matrix4f& p);

        void set_pixel(const Vector3f& point, const Vector3f& color);

        void clear(Buffers buff);

        void draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type);

        std::vector<Vector3f>& frame_buffer() { return frame_buf; }

    private:
        void draw_line(Vector3f begin, Vector3f end);

        void rasterize_triangle(const Triangle& t);

        // VERTEX SHADER -> MVP -> Clipping -> /.W -> VIEWPORT -> DRAWLINE/DRAWTRI -> FRAGSHADER

    private:
        Matrix4f model;
        Matrix4f view;
        Matrix4f projection;

        std::map<int, std::vector<Vector3f>> pos_buf;   // 顶点位置缓冲，存储所有三角形的顶点位置
        std::map<int, std::vector<Vector3i>> ind_buf;   // 索引缓冲，告诉渲染器那些顶点组成一个三角形
        std::map<int, std::vector<Vector3f>> col_buf;   // 颜色缓冲，告诉渲染器三角形的颜色是什么

        std::vector<Vector3f> frame_buf;    // 帧缓冲，表示屏幕上的每个像素点对应的颜色值

        std::vector<float> depth_buf;
        int get_index(int x, int y);

        int width, height;

        int next_id = 0;
        int get_next_id() { return next_id++; }
    };
}
