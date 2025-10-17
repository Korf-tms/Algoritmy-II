#include<iostream>
#include<vector>
#include<boost/multiprecision/cpp_dec_float.hpp>

using std::vector;

// Evaluate polynomial using Horner's method
template<typename T, typename U>
T horner(const vector<T>& coeffs, const U& x){
    T result = coeffs.back();
    for(size_t i = coeffs.size() - 2; i < coeffs.size(); --i){
        result = result * x + coeffs[i];
    }
    return result;
}

// Naive polynomial evaluation
template<typename T, typename U>
T naive(const vector<T>& coeffs, const U& x){
    T result = 0;
    T x_power = 1;
    for(size_t i = 0; i < coeffs.size(); ++i){
        result += coeffs[i] * x_power;
        x_power *= x;
    }
    return result;
}

// Extremely inefficient version for testing purposes
template<typename T, typename U>
T superNaive(const vector<T>& coeffs, const U& x){
    T result = 0;
    for(size_t i = 0; i < coeffs.size(); ++i){
        T term = coeffs[i];
        for(size_t j = 0; j < i; ++j){
            term *= x;
        }
        result += term;
    }
    return result;
}

// Generate Wilkinson polynomial coefficients
template<typename T>
vector<T> wilkonsPolynomial(size_t degree){
    vector<T> coeffs(degree + 1, 0);
    coeffs[0] = 1;
    // straigthforwatd generation of coefficients as successive multiplications
    for(size_t k = 1; k <= degree; ++k){
        std::vector<T> new_coeffs(k + 1, 0);
        for(int i = 0; i < k; ++i){
            new_coeffs[i + 1] += coeffs[i];                 // multiply by x
            new_coeffs[i] += -coeffs[i] * static_cast<T>(k); // multiply by -i
        }
        coeffs = new_coeffs;
    }
    return coeffs;
}

template<typename T>
void printVector(const vector<T>& vec){
    for(const auto& v : vec){
        std::cout << v << " ";
    }
    std::cout << "\n";
}


void testHorner(){
    using boost::multiprecision::cpp_dec_float_50;
    using std::cout;

    size_t degree = 20;
    auto coeffs = wilkonsPolynomial<cpp_dec_float_50>(degree);
    
    cpp_dec_float_50 x = 18.0 + pow(2, -22);

    auto horner_result = horner(coeffs, x);
    auto naive_result = naive(coeffs, x);
    auto super_naive_result = superNaive(coeffs, x);


    auto coeffs_float = wilkonsPolynomial<double>(degree);
    double x_double = static_cast<double>(x);

    double horner_result_float = horner(coeffs_float, x_double);
    double naive_result_float = naive(coeffs_float, x_double);
    double super_naive_result_float = superNaive(coeffs_float, x_double);

    cout << "Using double." << "\n";
    cout << "Horner's method result: " << horner_result_float << "\n";
    cout << "Naive method result: " << naive_result_float << "\n";
    cout << "Super naive method result: " << super_naive_result_float << "\n\n";
    cout << "Using cpp_dec_float_50." << "\n";
    cout << "Horner's method result: " << horner_result << "\n";
    cout << "Naive method result: " << naive_result << "\n";
    cout << "Super naive method result: " << super_naive_result << "\n";
}

int main(){
    testHorner();
    return 0;
}