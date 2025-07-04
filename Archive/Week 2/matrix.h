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

    // constructor for empty matrix
    Matrix();
    
    // constructor for zero matrix of given sizes
    Matrix(size_t row, size_t col);
    
    // constructor for diagonal matrix of given sizes
    Matrix(size_t row, size_t col, double value);
    
    // constructor fom matrix data
    explicit Matrix(vector<vector<double> >&& matrix_data );
    
    // constructor from a given matrix
    Matrix(const Matrix& other);

    // constructor from given row-wise data
    Matrix(size_t rows, size_t cols, const vector<double>& values);

    void print() const;
    double& operator()(size_t row, size_t col);
    double operator()(size_t row, size_t col) const;

    inline void checkDimensionAddition(const Matrix& other) const;
    inline void checkDimensionMultiplication(const Matrix& other) const;

    Matrix& operator=(const Matrix& other);
    
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    
    // multiplication to the right result = this * other
    Matrix operator*(const Matrix& other) const; 
    
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
};

#endif //MATRIX_H
