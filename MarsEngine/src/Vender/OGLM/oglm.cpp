#include "oglm.h"
    static float mod(const float a, const float b)
    {
        return (a-b) > 0 ? (a-b) : (b-a);
    }


    void oglm::Transpose(oglm::Matrix4& matrix, const unsigned int& rc)
    {
        float temp;
        for (unsigned int i = 0; i/2 < rc; i++)
            for(unsigned int j = 1 + i; (j/2 + 2) < rc; j++)
            {
                temp = matrix.mat[i][j];
                matrix.mat[i][j] = matrix.mat[j][i];
                matrix.mat[j][i] = temp;
            }
    }

    oglm::Matrix4 oglm::GenOrtho(const float& left,const float& right, const float& bottom, const float& top, const float& Z2, const float& Z1)
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

    void oglm::ScaleX(Matrix4& matrix, const float& X)
    {
        matrix.mat[0][0] = X;
    }
    void oglm::ScaleY(Matrix4& matrix, const float& Y)
    {
        matrix.mat[1][1] = Y;
    }
    void oglm::ScaleZ(Matrix4& matrix, const float& Z)
    {
        matrix.mat[2][2] = Z;
    }

    void oglm::TransX(Matrix4& matrix, const float& X)
    {
        matrix.mat[3][0] = X;
    }
    void oglm::TransY(Matrix4& matrix, const float& Y)
    {
        matrix.mat[3][1] = Y;
    }
    void oglm::TransZ(Matrix4& matrix, const float& Z)
    {
        matrix.mat[3][2] = Z;
    }

    oglm::Matrix4 oglm::Rotate(oglm::Matrix4 matrix, const double& radX, const double& radY, const double& radZ)
    {
        Matrix4 X, Y, Z;
        X.mat[1][1] =  cos(radX);
        X.mat[1][2] = -sin(radX);
        X.mat[2][1] =  sin(radX);
        X.mat[2][2] =  cos(radX);

        Y.mat[0][0] =  cos(radY);
        Y.mat[0][2] =  sin(radY);
        Y.mat[2][0] = -sin(radY);
        Y.mat[2][2] =  cos(radY);
        
        Z.mat[0][0] =  cos(radZ);
        Z.mat[0][1] = -sin(radZ);
        Z.mat[1][0] =  sin(radZ);
        Z.mat[1][1] =  cos(radZ);

        matrix = matrix * X;
        matrix = matrix * Y;
        matrix = matrix * Z;
        Transpose(matrix, 4);

        return matrix;
    }

    oglm::Matrix4 oglm::Transulate(oglm::Matrix4 matrix, const float& X, const float& Y, const float& Z)
    {
        Matrix4 trans;
        trans.mat[3][0] = X;
        trans.mat[3][1] = Y;
        trans.mat[3][2] = Z;

        matrix = matrix * trans;
        return matrix;
    }

    oglm::Matrix4 oglm::Scale(oglm::Matrix4 matrix, const float& X, const float& Y, const float& Z)
    {
        Matrix4 scale;
        scale.mat[0][0] = X;
        scale.mat[1][1] = Y;
        scale.mat[2][2] = Z;

        matrix = matrix * scale;
        return matrix;
    }

    oglm::Matrix4 oglm::GenIdentity()
    {
        Matrix4 i;
        return i;
    }
