#include<iostream>
#include<vector>
#include<cmath>

using std::vector, std::cout;

using ScalarType = double;
using Matrix = vector<vector<ScalarType>>;

template<typename T>
void printVector(const vector<T>& vec){
    for(const T& item : vec){
        cout << item << " ";
    }
    cout << "\n";
}

void printMatrix(const Matrix& mat){
    for(const auto& row : mat){
        printVector(row);
    }
}

// solve using Gaussian elimination
vector<ScalarType> solveAxb(Matrix A, vector<ScalarType> b){
    // we can easily turn this into PA = LU-decomposition
    Matrix L(A.size(), vector<ScalarType>(A.size(), 0.0));
    for(size_t i = 0; i < L.size(); i++){
        L[i][i] = 1;
    }

    // Turn into upper triangular matrix
    for(size_t i = 0; i < A.size(); i++){ // loop over columns
        // find pivot
        size_t pivot = i;
        ScalarType pivotValue = std::abs(A.at(i).at(i));
        for(size_t l = i + 1; l < A.size(); l++){
            if( std::abs(A.at(l).at(i)) > pivotValue ){
                pivot = l;
                pivotValue = std::abs(A.at(l).at(i));
            }
        }

        if(pivot != i){
            std::swap(A[i], A[pivot]);
            std::swap(b[i], b[pivot]);
        }

        for(size_t j = i + 1; j < A.size(); j++){ //loop over rows
            ScalarType temp = A.at(j).at(i) / A.at(i).at(i);
            L[j][i] = temp;
            for(size_t k = i; k < A.size(); k++){
                A[j][k] = A.at(j).at(k) - temp * A.at(i).at(k);
            }
            b[j] = b.at(j) - temp * b.at(i);
        }
    }

    Matrix U = A;

    // back-propagation
    for(int i = A.size() - 1; i >=0; i--){
        double sum = 0;
        for(size_t j = i + 1; j < A.size(); j++){
            sum += A.at(i).at(j) * b.at(j);
        }
        b[i] = (b.at(i) - sum)/A.at(i).at(i);
    }
    return b;
    // return {L, U};
}


void testAxb(){
    Matrix mat = {{1, 2},
                  {2, 1}};
    vector<ScalarType> rhs = {1, 1};
    printMatrix(mat);
    auto x = solveAxb(mat, rhs);
    printVector(x);
}


int main(){
    testAxb();
    return 0;
}