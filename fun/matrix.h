//
// Created by tom on 30.9.24.
//

#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

using std::vector;

class Matrix
{
public:
    vector<vector<double> > data;
    size_t rows;
    size_t columns;
    Matrix();
    Matrix(size_t row, size_t col);
    Matrix(size_t row, size_t col, double value);
    explicit Matrix(vector<vector<double> >&& matrix_data );
    Matrix(const Matrix& matrix);
    Matrix(size_t rows, size_t cols, const vector<double>& values);

    void print() const;
    double& operator()(size_t row, size_t col);
    double operator()(size_t row, size_t col) const;

    inline void checkDimensionAddition(const Matrix& other) const;
    inline void checkDimensionMultiplication(const Matrix& other) const;

    Matrix& operator=(const Matrix& other);
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
};

#endif //MATRIX_H
