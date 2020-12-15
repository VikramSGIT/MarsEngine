#pragma once

#include <cstring>
#include <iostream>
#include <cmath>

namespace oglm
{
    template<typename T>
    struct Matrix4
    {
        T mat[4][4] =
        {
            {1.0, 0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0, 0.0},
            {0.0, 0.0, 0.0, 1.0}
        };
        Matrix4 operator*(const Matrix4& matrix4)
        {
            Matrix4 mat1;
            mat1.mat[0][0] = this->mat[0][0] * matrix4.mat[0][0] + this->mat[0][1] * matrix4.mat[1][0] + this->mat[0][2] * matrix4.mat[2][0] + this->mat[0][3] * matrix4.mat[3][0];
            mat1.mat[0][1] = this->mat[0][0] * matrix4.mat[0][1] + this->mat[0][1] * matrix4.mat[1][1] + this->mat[0][2] * matrix4.mat[2][1] + this->mat[0][3] * matrix4.mat[3][1];
            mat1.mat[0][2] = this->mat[0][0] * matrix4.mat[0][2] + this->mat[0][1] * matrix4.mat[1][2] + this->mat[0][2] * matrix4.mat[2][2] + this->mat[0][3] * matrix4.mat[3][2];
            mat1.mat[0][3] = this->mat[0][0] * matrix4.mat[0][3] + this->mat[0][1] * matrix4.mat[1][3] + this->mat[0][2] * matrix4.mat[2][3] + this->mat[0][3] * matrix4.mat[3][3];
            mat1.mat[1][0] = this->mat[1][0] * matrix4.mat[0][0] + this->mat[1][1] * matrix4.mat[1][0] + this->mat[1][2] * matrix4.mat[2][0] + this->mat[1][3] * matrix4.mat[3][0];
            mat1.mat[1][1] = this->mat[1][0] * matrix4.mat[0][1] + this->mat[1][1] * matrix4.mat[1][1] + this->mat[1][2] * matrix4.mat[2][1] + this->mat[1][3] * matrix4.mat[3][1];
            mat1.mat[1][2] = this->mat[1][0] * matrix4.mat[0][2] + this->mat[1][1] * matrix4.mat[1][2] + this->mat[1][2] * matrix4.mat[2][2] + this->mat[1][3] * matrix4.mat[3][2];
            mat1.mat[1][3] = this->mat[1][0] * matrix4.mat[0][3] + this->mat[1][1] * matrix4.mat[1][3] + this->mat[1][2] * matrix4.mat[2][3] + this->mat[1][3] * matrix4.mat[3][3];
            mat1.mat[2][0] = this->mat[2][0] * matrix4.mat[0][0] + this->mat[2][1] * matrix4.mat[1][0] + this->mat[2][2] * matrix4.mat[2][0] + this->mat[2][3] * matrix4.mat[3][0];
            mat1.mat[2][1] = this->mat[2][0] * matrix4.mat[0][1] + this->mat[2][1] * matrix4.mat[1][1] + this->mat[2][2] * matrix4.mat[2][1] + this->mat[2][3] * matrix4.mat[3][1];
            mat1.mat[2][2] = this->mat[2][0] * matrix4.mat[0][2] + this->mat[2][1] * matrix4.mat[1][2] + this->mat[2][2] * matrix4.mat[2][2] + this->mat[2][3] * matrix4.mat[3][2];
            mat1.mat[2][3] = this->mat[2][0] * matrix4.mat[0][3] + this->mat[2][1] * matrix4.mat[1][3] + this->mat[2][2] * matrix4.mat[2][3] + this->mat[2][3] * matrix4.mat[3][3];
            mat1.mat[3][0] = this->mat[3][0] * matrix4.mat[0][0] + this->mat[3][1] * matrix4.mat[1][0] + this->mat[3][2] * matrix4.mat[2][0] + this->mat[3][3] * matrix4.mat[3][0];
            mat1.mat[3][1] = this->mat[3][0] * matrix4.mat[0][1] + this->mat[3][1] * matrix4.mat[1][1] + this->mat[3][2] * matrix4.mat[2][1] + this->mat[3][3] * matrix4.mat[3][1];
            mat1.mat[3][2] = this->mat[3][0] * matrix4.mat[0][2] + this->mat[3][1] * matrix4.mat[1][2] + this->mat[3][2] * matrix4.mat[2][2] + this->mat[3][3] * matrix4.mat[3][2];
            mat1.mat[3][3] = this->mat[3][0] * matrix4.mat[0][3] + this->mat[3][1] * matrix4.mat[1][3] + this->mat[3][2] * matrix4.mat[2][3] + this->mat[3][3] * matrix4.mat[3][3];

            return mat1;
        }

        T& GetX() { return this->mat[3][0]; }
        T& GetY() { return this->mat[3][1]; }
        T& GetZ() { return this->mat[3][2]; }

        T& GetScaleX() { return this->mat[0][0]; }
        T& GetScaleY() { return this->mat[1][0]; }
        T& GetScaleZ() { return this->mat[2][0]; }

    };

    template<typename T>
    struct vec2
    {
        T x = 0.0f, y = 0.0f;
    };
    template<typename T>
    struct vec3
    {
        T x = 0.0f, y = 0.0f, z = 0.0f;
        vec3() = default;
        vec3(const T& X, const T& Y, const T& Z)
        {
            x = X;
            y = Y;
            z = Z;
        }
        vec3(const vec2<T>& XY, const T& Z)
        {
            x = XY.x;
            y = XY.y;
            z = Z;
        }
    };
    template<typename T>
    struct vec4
    {
        T x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
        vec4() = default;
        vec4(const T& X, const T& Y, const T& Z, const T& W)
        {
            x = X;
            y = Y;
            z = Z;
            w = W;
        }
        vec4(const vec2<T>& XY, const T& Z, const T& W)
        {
            x = XY.x;
            y = XY.y;
            z = Z;
            w = W;
        }
        vec4(const vec3<T>& XYZ, const T& W)
        {
            x = XYZ.x;
            y = XYZ.y;
            z = XYZ.z;
            w = W;
        }
    };

    template<typename T>
    bool Normalize(T& var, float normal)
    {
        T* vec = &var;
        float* arr = (float*)(vec);
        if (arr != nullptr)
        {
            for (int i = 0; i < sizeof(T) / sizeof(float); i++)
                *(arr + i) = (*(arr + i)) / normal;
            return true;
        }
        return false;
    }

    template<typename T>
    void Transpose(Matrix4<T>& matrix, const unsigned int& rc);

    template<typename T>
    static void Transpose(Matrix4<T>& matrix, const unsigned int& rc)
    {
        float temp;
        for (unsigned int i = 0; i / 2 < rc; i++)
            for (unsigned int j = 1 + i; (j / 2 + 2) < rc; j++)
            {
                temp = matrix.mat[i][j];
                matrix.mat[i][j] = matrix.mat[j][i];
                matrix.mat[j][i] = temp;
            }
    }

    template<typename T>
    static Matrix4<T> GenOrtho(const T& left, const T& right, const T& bottom, const T& top, const T& Z2, const T& Z1)
    {
        Matrix4<T> matrix;
        if ((top != bottom) && (left != right) && (Z1 != Z2))
        {
            //Scaling the vertex
            ScaleX(matrix, 2 / (right - left));
            ScaleY(matrix, 2 / (top - bottom));
            ScaleZ(matrix, -2 / (Z1 - Z2));
            //Transulation
            TransX(matrix, -(right + left) / (right - left));
            TransY(matrix, -(top + bottom) / (top - bottom));
            TransZ(matrix, -(Z1 + Z2) / (Z1 - Z2));

        }
        else { std::cout << "Invalid Orthogonal Matrix" << std::endl; }

        return matrix;
    }

    template<typename T>
    static Matrix4<T> GenIdentity()
    {
        Matrix4<T> mat;
        return mat;
    }
    template<typename T>
    static void ScaleX(Matrix4<T>& matrix, const T& X)
    {
        matrix.mat[0][0] = X;
    }
    template<typename T>
    static void ScaleY(Matrix4<T>& matrix, const T& Y)
    {
        matrix.mat[1][1] = Y;
    }
    template<typename T>
    static void ScaleZ(Matrix4<T>& matrix, const T& Z)
    {
        matrix.mat[2][2] = Z;
    }
    template<typename T>
    static void TransX(Matrix4<T>& matrix, const T& X)
    {
        matrix.mat[3][0] = X;
    }
    template<typename T>
    static void TransY(Matrix4<T>& matrix, const T& Y)
    {
        matrix.mat[3][1] = Y;
    }
    template<typename T>
    static void TransZ(Matrix4<T>& matrix, const T& Z)
    {
        matrix.mat[3][2] = Z;
    }

    template<typename T>
    static Matrix4<T> Rotate(Matrix4<T> matrix, const T& radX, const T& radY, const T& radZ)
    {
        Matrix4<T> X, Y, Z;
        X.mat[1][1] = cos(radX);
        X.mat[1][2] = -sin(radX);
        X.mat[2][1] = sin(radX);
        X.mat[2][2] = cos(radX);

        Y.mat[0][0] = cos(radY);
        Y.mat[0][2] = sin(radY);
        Y.mat[2][0] = -sin(radY);
        Y.mat[2][2] = cos(radY);

        Z.mat[0][0] = cos(radZ);
        Z.mat[0][1] = -sin(radZ);
        Z.mat[1][0] = sin(radZ);
        Z.mat[1][1] = cos(radZ);

        matrix = matrix * X;
        matrix = matrix * Y;
        matrix = matrix * Z;
        Transpose(matrix, 4);

        return matrix;
    }

    template<typename T>
    Matrix4<T> Transulate(Matrix4<T> matrix, const T& X, const T& Y, const T& Z)
    {
        Matrix4<T> trans;
        trans.mat[3][0] = X;
        trans.mat[3][1] = Y;
        trans.mat[3][2] = Z;

        matrix = matrix * trans;
        return matrix;
    }

    template<typename T>
    Matrix4<T> Scale(Matrix4<T> matrix, const T& X, const T& Y, const T& Z)
    {
        Matrix4<T> scale;
        scale.mat[0][0] = X;
        scale.mat[1][1] = Y;
        scale.mat[2][2] = Z;

        matrix = matrix * scale;
        return matrix;

    }
}