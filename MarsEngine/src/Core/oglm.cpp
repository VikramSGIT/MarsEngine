#include "oglm.h"
static float mod(const float a, const float b)
{
    return (a-b) > 0 ? (a-b) : (b-a);
}


void oglm::TransposeM(oglm::Matrix4& matrix, const unsigned int row, const unsigned int column)
    {
        float temp;
        for (unsigned int i = 0; i < row; i++)
            for(unsigned int j = 0; j < column; j++)
            {
                if(i != j)
                {
                    temp = matrix.mat[i][j];
                    matrix.mat[i][j] = matrix.mat[j][i];
                    matrix.mat[j][i] = temp;
                }
            }
    }

    void oglm::conColMajor4M(oglm::Matrix4 matrix)
    {
        TransposeM(matrix, 4,4);
    }

    oglm::Matrix4 oglm::genOrtho(const float left,const float right, const float bottom, const float top, const float Z2, const float Z1)
    {
        oglm::Matrix4 matrix;
        if((top != bottom) && (left != right) && (Z1 != Z2))
        {
    //Scaling the vertex
            oglm::ScaleX(matrix,2/(right - left));
            oglm::ScaleY(matrix,2/(top - bottom));
            oglm::ScaleZ(matrix,-2/(Z1 - Z2));
    //Transulation
            oglm::TransX(matrix, -(right + left)/(right - left));
            oglm::TransY(matrix, -(top + bottom)/(top - bottom));
            oglm::TransZ(matrix, -(Z1 + Z2)/(Z1 - Z2));

        }
        else {std::cout << "Invalid Orthogonal Matrix" << std::endl;}

        return matrix;
    }

    void oglm::ScaleX(Matrix4& matrix, const float X)
    {
        matrix.mat[0][0] = X;
    }
    void oglm::ScaleY(Matrix4& matrix, const float Y)
    {
        matrix.mat[1][1] = Y;
    }
    void oglm::ScaleZ(Matrix4& matrix, const float Z)
    {
        matrix.mat[2][2] = Z;
    }

    void oglm::TransX(Matrix4& matrix, const float X)
    {
        matrix.mat[3][0] = X;
    }
    void oglm::TransY(Matrix4& matrix, const float Y)
    {
        matrix.mat[3][1] = Y;
    }
    void oglm::TransZ(Matrix4& matrix, const float Z)
    {
        matrix.mat[3][2] = Z;
    }