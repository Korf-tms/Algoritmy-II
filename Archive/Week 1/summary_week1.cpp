#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using std::vector, std::cout, std::sort, std::size_t, std::unordered_set, std::unordered_map;


vector<int> strangeSums2(const vector<int> in_vec){
    vector<int> result(2, 0);

    for(const int item : in_vec ){
        if( item % 2 == 0){
            result[0] += item;
        }else{
            result[1] += item;
        }
    }
    return result;
}


vector<int> strangeSumsK(const vector<int>& in_vec, int d){
    vector<int> result(d, 0);
    int remainder;

    for(const auto item : in_vec){
        remainder = item % d;
        result[remainder] += item;
    }
    return result;
}

// Implementace prednaskove ulohy na nalezeni modu pomoci trideni
int modusByPresort(vector<int> data){
    std::sort(data.begin(), data.end());
    int index = 0;
    int modus_candidate;
    int modus_candidate_counts = -1;

    int current_candidate_counts;
    int current_candidate;

    while(index < data.size()){
        current_candidate = data[index];
        current_candidate_counts = 0;

        while(index < data.size() and data[index] == current_candidate){
            current_candidate_counts += 1;
            index += 1;
        }
        if(current_candidate_counts > modus_candidate_counts) {
            modus_candidate = current_candidate;
            modus_candidate_counts = current_candidate_counts;
        }
    }
    return modus_candidate;
}


// Implementace hledani modu pomoci hashamapy
int modusByUnorderedMap(vector<int> data){
    unordered_map<int, int> multiplicities;
    for (int item : data){
        multiplicities[item] += 1; // zakerne pokud item neni v mape!
    }
    int modus_candidate;
    int modus_candidat_count = -1;
    for(const auto& [number, count] : multiplicities){
        if ( count > modus_candidat_count ){
            modus_candidat_count = count;
            modus_candidate = number;
        }
    }
    return modus_candidate;
}

// Implementace overeni unikatnosti prvku pole pomoci sortu
bool containsUniqueElements(vector<int> in_vec){
    sort(in_vec.begin(), in_vec.end()); // n log(n)

    for(size_t i = 1; i < in_vec.size(); i++){
        if(in_vec[i] == in_vec[i-1]){
            return false;
        }
    }
    return true;
}

// Implementace overeni unikanoasti prvku pole pomoci unordered_map (pomoci set to je lepsi, proc?)
bool containsUniqueElementsMap(const vector<int>& in_vec){
    unordered_map<int, int> dict;

    for (auto item : in_vec){
        if ( dict.count(item) == 1){
            return false;
        }
        dict[item] = 1;
    }
    return true;
}

// Implementace overeni unikatnosti prvku pole pomoci unordered_set
bool containsUniqueElementsSet(vector<int> in_vec){
    unordered_set<int> as_set = unordered_set(in_vec.begin(), in_vec.end());

    return as_set.size() == in_vec.size() ;
}

// reseni ulohy 7, sekce Excercises 6.1 z Levitina
bool twoPointerSum(const vector<int>& in_vec, const int target){
    int lower = 0;
    int upper = in_vec.size() - 1;
    int suma;

    while(true){
        suma = in_vec.at(lower) + in_vec.at(upper);
        if (suma == target){
            return true;
        }
        if (suma < target){
            lower += 1;
        }
        if (suma > target){
            upper -= 1;
        }
        if (lower == upper){
            return false;
        }
    }
}

int main() {
    vector<int> data1 = {1, 1, 2, 2, 2, 3, 4, 4, 5, 6, 6,};
    vector<int> data2 = {1, 2, 3};
    vector<int> data3 = {1};

    // Doporucuji si nahodne vygenerovat velka data a zkouset totez
    const auto data_list = {data1, data2, data3};
    for (const auto& data : data_list){
        cout << modusByPresort(data) << " " << modusByUnorderedMap(data) << "\n";
        cout << containsUniqueElementsSet(data) << " " << containsUniqueElementsMap(data) << "\n";
    }

    const auto target_sums = {44, 10, 2, 11, 7};
    for( const int& target : target_sums){
        cout << twoPointerSum(data1, target) << "\n";
    }

    return 0;
}