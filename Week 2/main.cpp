#include<iostream>
#include<vector>
#include<cmath>  // std::abs

using std::vector, std::cout;

using ScalarType = double;  // better typedef
using Matrix = vector<vector<ScalarType>>;  // implemented as vector of rows!


// prints vector to read the solution
template<typename T>
void printVector(const vector<T>& vec){
    for(const T& item : vec){
        cout << item << " ";
    }
    cout << "\n";
}

// prints matrix to check the LU
void printMatrix(const Matrix& mat){
    for(const auto& row : mat){
        printVector(row);
    }
}

// multiplies two matrices, to check the LU decomposition
Matrix multiplyMatrices(const Matrix& A, const Matrix& B){
    Matrix result(A.size(), vector<ScalarType>(B[0].size(), 0.0));
    for(size_t i = 0; i < A.size(); i++){
        for(size_t j = 0; j < B[0].size(); j++){
            for(size_t k = 0; k < B.size(); k++){
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

// P[i] = original row index now sitting at row i; undoes that row permutation in-place
void applyInversePermutation(Matrix& mat, const vector<size_t>& P){
    Matrix result(mat.size());
    for(size_t i = 0; i < P.size(); i++){
        result[P[i]] = mat[i]; // this is inverse, since P[i] = original row index now sitting at row i; direct would be mat[i] = result[P[i]];
    }
    mat = result;
}

// pulls the unit-lower-triangular L out of a combined LU matrix
Matrix extractL(const Matrix& LU){
    Matrix L(LU.size(), vector<ScalarType>(LU.size(), 0.0));
    for(size_t i = 0; i < LU.size(); i++){
        L[i][i] = 1;
        for(size_t j = 0; j < i; j++){
            L[i][j] = LU[i][j];
        }
    }
    return L;
}

// pulls the upper-triangular U out of a combined LU matrix
Matrix extractU(const Matrix& LU){
    Matrix U(LU.size(), vector<ScalarType>(LU.size(), 0.0));
    for(size_t i = 0; i < LU.size(); i++){
        for(size_t j = i; j < LU.size(); j++){
            U[i][j] = LU[i][j];
        }
    }
    return U;
}

// bundles everything solveAxb computes: PA = LU (L and U combined into one matrix) and the solution x
struct LUResult{
    Matrix LU; // strictly-lower part is L's multipliers, diagonal-and-above is U
    vector<size_t> P; // P[i] = original row index now sitting at row i
    vector<ScalarType> x; // soluction vector
};

// solve using Gaussian elimination and computes PA = LU
LUResult solveAxb(const Matrix& Ain, const vector<ScalarType>& bin){
    Matrix A = Ain; // local copy of A will be transformed into LU container
    vector<ScalarType> b = bin; // local copy of b will be transformed into a solution vector

    // P[i] = original row index now sitting at row i (P[i] = i => no changes)
    vector<size_t> P(A.size());
    for(size_t i = 0; i < P.size(); i++){
        P[i] = i;
    }

    // Turn A into upper triangular matrix
    for(size_t i = 0; i < A.size(); i++){ // loop over rows
        // find pivot as the row with highest absolute value
        size_t pivot = i;
        ScalarType pivotValue = std::abs(A[i][i]);
        for(size_t l = i + 1; l < A.size(); l++){ // loop over rows below in fixed column i
            if( std::abs(A[l][i]) > pivotValue ){
                pivot = l;
                pivotValue = std::abs(A[l][i]);
            }
        }

        // swap rows, if needed
        if(pivot != i){
            std::swap(P[i], P[pivot]); // tracks the permutations
            std::swap(A[i], A[pivot]); 
            std::swap(b[i], b[pivot]);
        }

        // actual elimination step
        for(size_t j = i + 1; j < A.size(); j++){ // loop over rows below
            ScalarType temp = A[j][i] / A[i][i]; // compute coefficient for each row
            A[j][i] = temp; // store the multiplier in place of the zero it would become, this slot is now part of L
            for(size_t k = i + 1; k < A.size(); k++){ // start the loop on the first nonzero, does not touch L values
                A[j][k] = A[j][k] - temp * A[i][k];
            }
            b[j] = b[j] - temp * b[i];
        }
    }

    // back-propagation
    // b is used as a vector containing solution
    for(int i = A.size() - 1; i >=0; i--){
        double sum = 0;
        for(size_t j = i + 1; j < A.size(); j++){ // loop over columns to the right
            sum += A[i][j] * b[j]; // contributions from alredy solved variables
        }
        b[i] = (b[i] - sum)/A[i][i]; // modify rhs and trivially solve
    }

    return {A, P, b};
}


void testAxb(){
    Matrix mat = {{1, 2, 0},
                  {0, 2, 1},
                  {3, 1, 0}};
    
    vector<ScalarType> rhs = {1, 1, 1};

    printMatrix(mat);
    LUResult result = solveAxb(mat, rhs);

    // extraction to demonstrate that PA = LU decomposition is correct
    Matrix L = extractL(result.LU);
    Matrix U = extractU(result.LU);

    std::cout << "PA = LU decomposition:\nL\n";
    printMatrix(L);
    std::cout << "U\n";
    printMatrix(U);
    std::cout << "P\n";
    printVector(result.P);

    Matrix Linv = L;
    applyInversePermutation(Linv, result.P); // A = (P^{-1}L)U
    Matrix reconstructed = multiplyMatrices(Linv, U);
    std::cout << "A = P^{-1}LU reconstructed\n";
    printMatrix(reconstructed);

    std::cout << "Solution x:\n";
    printVector(result.x);
}


int main(){
    testAxb();
    return 0;
}