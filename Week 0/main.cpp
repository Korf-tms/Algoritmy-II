#include<iostream>
#include<vector>
#include<algorithm>

using std::vector, std::cout;

std::pair<int, int> findSecondSmallest(const vector<int>& vec){
    int smallest = vec.at(1);
    int secondSmallest = vec.at(0);
    if( smallest > secondSmallest ){
        std::swap(smallest, secondSmallest);
    }

    for(size_t i = 2; i < vec.size(); i++){
        // cislo je mensi nez nejmensi, posunu obe
        if( vec.at(i) < smallest){
            secondSmallest = smallest;
            smallest = vec.at(i);
            continue;
        }
        // cislo je mezi nejmensim a druhym nejmensim, posunu to vetsi
        if( vec.at(i) < secondSmallest){
            secondSmallest = vec.at(i);
        }
    }
    return {smallest, secondSmallest};
}

void testSmallest(){
    vector<int> data = {44, -2, 1, 11, 4, 5, 6, 3, 2, 5, 7, 8, 9, 10,};
    const auto smallestPair = findSecondSmallest(data);
    
    // knihovni sort pro kontrolu
    std::sort(data.begin(), data.end());
    
    if(data.at(0) != smallestPair.first or data.at(1) != smallestPair.second){
        cout << "wrong smallest pair\n";
    }else{
        cout << "ok smallest pair\n";
    }
}

// efektivne nefunguje, prilis mnoho rekurzivnich volani
long long fibonacciRecurrentNaive(const int n){
    if(n == 0){return 0;}
    if(n == 1){return 1;}
    return fibonacciRecurrentNaive(n - 1) + fibonacciRecurrentNaive(n - 2);
}

// "opravena" rekurze o  ukladani vypoctu
struct fibonacciRecurrent{
    private:
    vector<long long> cache;

    long long compute(const int n){
        if( n < cache.size() ){
            return cache.at(n);
        }
        long long newValue = compute(n-1) + compute(n-2);
        cache.push_back(newValue);
        return newValue;
    }

    public:

    fibonacciRecurrent(){
        cache.push_back(0);
        cache.push_back(1);
    }

    long long operator()(const int n){
        if( n < 0){
            std::cerr << "Vstup musi byt kladny\n";
            return -1;
        }
        return compute(n);
    }
};

// iterativni implementace
long long fibonacciIterative(const int n){
    if(n == 0){return 0;}
    if(n == 1){return 1;}
    long long last = 1;
    long long previous = 1;
    long long newValue;

    for(int i = 2; i < n; i++){
        newValue = last + previous;
        previous = last;
        last = newValue;
        // lze provest i bez pomocne promenne za cenu operace navic
        // last += previous;
        // previous = last - previous;
    }
    return last;
}

class phiNum{
    private:
    // mocneni radove v log(n) slozitosti
    static phiNum power(phiNum base, int exponent) {
        phiNum result(1, 0);
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            exponent /= 2;
        }
        return result;
    }

    public:
    long long intPart;
    long long phiPart;

    phiNum() : intPart(0), phiPart(0) {};
    phiNum(long long a) : intPart(a), phiPart(0) {};
    phiNum(long long a, long long b) : intPart(a), phiPart(b) {};

    // scitani funguje po slozkach
    phiNum operator+(const phiNum& other){
        return phiNum(intPart + other.intPart, phiPart + other.phiPart);
    }

    // nasobeni, s pomoci vztahu φ² = φ + 1
    phiNum operator*(const phiNum& other) const {
        const long long a = intPart;
        const long long b = phiPart;
        const long long c = other.intPart;
        const long long d = other.phiPart;

        // (a + bφ)*(c + dφ) = (ac + bd) + (ad + bc + bd)φ
        long long new_int_part = a * c + b * d;
        long long new_phi_part = a * d + b * c + b * d;

        return phiNum(new_int_part, new_phi_part);
    }

    // vypocet ze vztahu φ^n = F(n)φ + F(n-1)
    static long long FiboPhiNum(const int n){
        phiNum phi = phiNum(0, 1);

        phiNum phi_n = phiNum::power(phi, n);
        return phi_n.phiPart;
    }

};

void testFibonacci(const int n=10){
    fibonacciRecurrent fib;

    for(int i = 0; i < n; i++){
        cout << i << ": ";
        cout << fibonacciIterative(i) << " ";
        cout << fib(i) << " ";
        cout << phiNum::FiboPhiNum(i);
        cout << "\n";
    }
}

int main(){
    testSmallest();
    testFibonacci();
    return 0;
}