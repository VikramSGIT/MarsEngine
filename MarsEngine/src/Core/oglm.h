#pragma once

#include <cstring>
#include <iostream>

namespace oglm
{
    struct Matrix4
    {
        float mat[4][4] = 
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        Matrix4 operator*(const Matrix4& matrix4)
        {
            Matrix4 mat1;
            mat1.mat[0][0] = this->mat[0][0]*matrix4.mat[0][0] + this->mat[0][1]*matrix4.mat[1][0] + this->mat[0][2]*matrix4.mat[2][0] + this->mat[0][3]*matrix4.mat[3][0];
            mat1.mat[0][1] = this->mat[0][0]*matrix4.mat[0][1] + this->mat[0][1]*matrix4.mat[1][1] + this->mat[0][2]*matrix4.mat[2][1] + this->mat[0][3]*matrix4.mat[3][1];
            mat1.mat[0][2] = this->mat[0][0]*matrix4.mat[0][2] + this->mat[0][1]*matrix4.mat[1][2] + this->mat[0][2]*matrix4.mat[2][2] + this->mat[0][3]*matrix4.mat[3][2];
            mat1.mat[0][3] = this->mat[0][0]*matrix4.mat[0][3] + this->mat[0][1]*matrix4.mat[1][3] + this->mat[0][2]*matrix4.mat[2][3] + this->mat[0][3]*matrix4.mat[3][3];
            mat1.mat[1][0] = this->mat[1][0]*matrix4.mat[0][0] + this->mat[1][1]*matrix4.mat[1][0] + this->mat[1][2]*matrix4.mat[2][0] + this->mat[1][3]*matrix4.mat[3][0];
            mat1.mat[1][1] = this->mat[1][0]*matrix4.mat[0][1] + this->mat[1][1]*matrix4.mat[1][1] + this->mat[1][2]*matrix4.mat[2][1] + this->mat[1][3]*matrix4.mat[3][1];
            mat1.mat[1][2] = this->mat[1][0]*matrix4.mat[0][2] + this->mat[1][1]*matrix4.mat[1][2] + this->mat[1][2]*matrix4.mat[2][2] + this->mat[1][3]*matrix4.mat[3][2];
            mat1.mat[1][3] = this->mat[1][0]*matrix4.mat[0][3] + this->mat[1][1]*matrix4.mat[1][3] + this->mat[1][2]*matrix4.mat[2][3] + this->mat[1][3]*matrix4.mat[3][3];
            mat1.mat[1][0] = this->mat[2][0]*matrix4.mat[0][0] + this->mat[2][1]*matrix4.mat[1][0] + this->mat[2][2]*matrix4.mat[2][0] + this->mat[2][3]*matrix4.mat[3][0];
            mat1.mat[2][1] = this->mat[2][0]*matrix4.mat[0][1] + this->mat[2][1]*matrix4.mat[1][1] + this->mat[2][2]*matrix4.mat[2][1] + this->mat[2][3]*matrix4.mat[3][1];
            mat1.mat[2][2] = this->mat[2][0]*matrix4.mat[0][2] + this->mat[2][1]*matrix4.mat[1][2] + this->mat[2][2]*matrix4.mat[2][2] + this->mat[2][3]*matrix4.mat[3][2];
            mat1.mat[2][3] = this->mat[2][0]*matrix4.mat[0][3] + this->mat[2][1]*matrix4.mat[1][3] + this->mat[2][2]*matrix4.mat[2][3] + this->mat[2][3]*matrix4.mat[3][3];
            mat1.mat[3][0] = this->mat[3][0]*matrix4.mat[0][0] + this->mat[3][1]*matrix4.mat[1][0] + this->mat[3][2]*matrix4.mat[2][0] + this->mat[3][3]*matrix4.mat[3][0];
            mat1.mat[3][1] = this->mat[3][0]*matrix4.mat[0][1] + this->mat[3][1]*matrix4.mat[1][1] + this->mat[3][2]*matrix4.mat[2][1] + this->mat[3][3]*matrix4.mat[3][1];
            mat1.mat[3][2] = this->mat[3][0]*matrix4.mat[0][2] + this->mat[3][1]*matrix4.mat[1][2] + this->mat[3][2]*matrix4.mat[2][2] + this->mat[3][3]*matrix4.mat[3][2];
            mat1.mat[3][3] = this->mat[3][0]*matrix4.mat[0][3] + this->mat[3][1]*matrix4.mat[1][3] + this->mat[3][2]*matrix4.mat[2][3] + this->mat[3][3]*matrix4.mat[3][3];

            return mat1;
        }
    };

    struct vec2
    {
        float x = 0.0f, y = 0.0f;

    };

    struct vec3
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;
    };

    struct vec4
    {
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

    };

    template<typename T>
    bool Normalize(T& var, float normal) 
    {
        T* vec = &var;
        float* arr = (float*)(vec);
        if (arr != nullptr)
        {
            for(int i = 0; i < sizeof(T)/sizeof(float) ; i++)
                *(arr + i) = (*(arr + i))/normal;
            return true;
        }
        return false;
    }


    void TransposeM(Matrix4& matrix, const unsigned int row, const unsigned int column);
    Matrix4 genOrtho(const float top,const float bottom, const float right, const float left, const float front, const float back);
    void conColMajor4M(Matrix4 matrix);

    //Scaling
    void ScaleX(Matrix4& matrix, const float X);
    void ScaleY(Matrix4& matrix, const float Y);
    void ScaleZ(Matrix4& matrix, const float Z);

    //Transulation
    void TransX(Matrix4& matrix, const float X);
    void TransY(Matrix4& matrix, const float Y);
    void TransZ(Matrix4& matrix, const float Z);
};