#include <iostream>
#include <vector>

using std::vector, std::cout, std::swap;


// simple class for matrices
class Matrix{
    public:
    int rows;
    int columns;
    vector<vector<double>> data;

    // "empty" matrix constructor
    Matrix() : rows(0), columns(0), data() {};

    // constructor from explicit data
    Matrix(const int r, const int c, const vector<vector<double>>& dat) :
        rows(r), columns(c), data(dat) {}
    

    void print() const{
        for(const auto& row: data){
            for(const auto& item : row){
                cout << item << " ";
            }
            cout << "\n";
        }
    }

    double& operator()(int i, int j){
        return data[i][j];
    }

    double operator()(int i, int j) const {
        return data[i][j];
    }

    Matrix multiplyMatrix(const Matrix& other) const{
        if( rows != other.columns ){
            throw std::invalid_argument("Matrices do not have conforming sizes for multiplication."); 
        }

        vector<vector<double>> result_data(rows, vector<double>(other.columns, 0.0));
        int result_rows = rows;
        int result_columns = other.columns;
        Matrix result(result_rows, result_columns, result_data);

        for(size_t i = 0; i < result.rows; i ++){
            for(size_t j = 0; j < result.columns; j++){
                for(size_t k = 0; k < rows; k++){
                    result(i, j) += data[i][k]*other(k, j);
                }  
            }
        }
        return result;
    }

    // super inefficient way to explicitely construct transposed matrix
    // better design is to have a is_transposed flag and modify every method accordingly
    Matrix createTranspose() const{
        Matrix result;
        result.rows = columns;
        result.columns = rows;
        result.data = vector<vector<double>>(result.rows, vector<double>(result.columns, 0));

        for(size_t i = 0; i < rows; i++){
            for(size_t j = 0; j < columns; j++){
                result(j, i) = data[i][j];
            }
        }
        return result;
    }
};

void printVec(const vector<double>& in_vec){
    for(const auto& item : in_vec){
        cout << item << " ";
    }
    cout << "\n";
}

// Gaussian elimination, assumes that the input matrix is invertible
vector<double> gaussianEliminationSimple(Matrix mat, vector<double> rhs)
{
    if (mat.rows != mat.columns){
        throw std::invalid_argument("gaussianEliminationSimple: matrix must be square");
        }
    const size_t n = mat.rows;

    // forward elimination
    for (size_t i = 0; i < n; i++)
    {
        // find pivot
        size_t pivot = i;
        for (size_t l = i + 1; l < n; l++)
        {
            if (abs(mat(l, i)) > abs(mat(pivot, i)))
            {
                pivot = l;
            }
        }

        // swap current and pivot row
        if (pivot != i )
        {
            std::swap(mat.data[i], mat.data[pivot]);
            std::swap(rhs[i], rhs[pivot]);
        }

        // place for checking if the pivot is zero the actual check is tricky,
        // so we defer to respective classes on numerical mathematics

        for (size_t j = i + 1; j < n; j++)
        {
            const double factor = mat(j, i) / mat(i, i);
            for (size_t k = i; k < n; k++)
            {
                mat(j, k) -= factor * mat(i, k);
            }
            rhs[j] -= rhs[i] * factor;
        }
    }

    // backwards substitution
    for (int i = n - 1; i >= 0; i--)
    {
        double sum = rhs[i];
        for (size_t j = i + 1; j < n; j++)
        {
            sum -= mat(i, j)*rhs[j];
        }
        rhs[i] = sum / mat(i, i);

    }
    return rhs;
}


// PLU decomposition function with partial pivoting, assumes that A is invertible
// Permutation is given as matrix P such that PA = LU
void pluDecomposition(const Matrix& A, Matrix& L, Matrix& U, Matrix& P) {
    if ( A.rows != A.columns){
        throw std::invalid_argument("Matrix for PLU decomposition must be square.");
    }
    size_t n = A.rows;

    // Initialize L as an identity matrix and U as a copy of A
    L.columns = n;
    L.rows = n;
    L.data = std::vector<std::vector<double>>(n, std::vector<double>(n, 0));
    U = A;

    // Setup L with 1 on diagonal and initialize permutation matrix P as an identity permutation
    for (size_t i = 0; i < n; i++) {
        L(i, i) = 1; // Set the diagonal of L to 1
    }
    P = L;

    // LU decomposition with pivoting
    for (size_t i = 0; i < n; i++) {
        // Pivoting: find the row with the largest value in column i (partial pivoting)
        size_t pivot = i;
        for (size_t j = i + 1; j < n; j++) {
            if (std::abs(U(j, i)) > std::abs(U(pivot, i))) {
                pivot = j;
            }
        }

        // Swap rows in U and update the permutation matrix P
        if (pivot != i) {
            std::swap(U.data[i], U.data[pivot]);
            std::swap(P.data[i], P.data[pivot]);

            // swap values in j-th column of L
            for (size_t j = 0; j < i; j++) {
                std::swap(L(i, j), L(pivot, j));
            }
        }

        // Perform the elimination process for rows below the pivot row
        for (size_t j = i + 1; j < n; j++) {
            L(j, i) = U(j,i) / U(i, i); // Calculate the multiplier for the L matrix

            // Update the j-th row of U matrix
            for (size_t k = i; k < n; k++) {
                U(j, k) -= L(j, i) * U(i, k);
            }
        }
    }
}

void test(const Matrix& in_mat, const vector<double>& rhs){
    vector<double> solution;
    
    cout << "Working with matrix\n";
    in_mat.print();
    solution = gaussianEliminationSimple(in_mat, rhs);

    cout << "solution\n"; 
    printVec(solution);

    Matrix L, U, P;
    pluDecomposition(in_mat, L, U, P);

    cout << "\nL-factor\n";
    L.print();
    cout << "\nU-factor\n";
    U.print();
    cout << "\nP-factor\n";
    P.print();
    cout << "\n";

    Matrix A = L.multiplyMatrix(U);
    cout << "LU\n";
    A.print();
    cout << "\n";

    Matrix B = P.multiplyMatrix(in_mat);
    cout << "PA\n";
    B.print();

    Matrix Pt = P.createTranspose();
    Matrix decompositionComposedBack = Pt.multiplyMatrix(A);

    cout << "\nP^{-1}LU\n";
    decompositionComposedBack.print();

}


int main() {
    Matrix smallMatrix(3, 3, {{2, -1, 1},
                            {4, 1, -1},
                            {1, 1, 1}
                            });
    vector<double> b = {1, 5, 0};
    
    test(smallMatrix, b);

    Matrix biggerMatrix(4, 4, {{
        {4, 3, 2, 1},
        {3, 2, 1, 0},
        {2, 1, 0, 1},
        {1, 0, 1, 2}
    }});
    b = {1, -4, 1, 5};

    test(biggerMatrix, b);

    return 0;
}