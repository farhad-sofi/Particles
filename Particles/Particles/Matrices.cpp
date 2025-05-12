#include "Matrices.h"

namespace Matrices
{
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;
        a.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            a[i].resize(cols, 0);
        }
    }
    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getCols())
        {
            throw runtime_error("Error: dimensions must agree");
        }

        Matrix c(a.getRows(), a.getCols());
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                c(i, j) = a(i, j) + b(i, j);
            }
        }
        return c;
    }

    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getRows())
        {
            throw runtime_error("Error: dimensions must agree");
        }
        Matrix c(a.getRows(), b.getCols());

        for (int k = 0; k < b.getCols(); k++)
        {
            for (int i = 0; i < a.getRows(); i++)
            {
                double sum = 0.0;
                for (int j = 0; j < a.getCols(); j++)
                {
                    sum += a(i, j) * b(j, k);
                }
                c(i, k) = sum;
            }
        }
        return c;
    }

    bool operator==(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getCols())
        {
            return false;
        }
        if (a.getRows() != b.getRows())
        {
            return false;
        }
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) < 0.001)
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getCols())
        {
            return true;
        }
        if (a.getRows() != b.getRows())
        {
            return true;
        }
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) > 0.001)
                {
                    return true;
                }
            }
        }
        return false;
    }

    ostream& operator<<(ostream& os, const Matrix& a)
    {
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                os << setw(10) << a(i, j) << ' ';
            }
            cout << endl;
        }
    }
    
    int Matrices::RotationMatrix(double theta)
    {
        Matrix(2, 2) = { cos(theta), -sin(theta),
                        sin(theta), cos(theta) };


    }
    
    int Matrices::ScalingMatrix(double scale)
    {

    }
   
    int Matrices::TranslationMatrix(double xShift, double yShift, int nCols)
    {

    }


}