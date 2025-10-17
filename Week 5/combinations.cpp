#include<iostream>
#include<vector>
#include<algorithm>

using std::vector;

// Combinations from basics, same algorithm as in Discrete Mathematics
vector<vector<int>> combinationsDIM(const int n, const int k){
    vector<vector<int>> res;
    if(k > n || k <= 0){
        return res;
    }
    vector<int> current;
    current.reserve(k);
    for(int i = 0; i < k; ++i){
        current.push_back(i);
    }
    while(true){
        res.push_back(current);
        // scan to find the rightmost element that can be incremented
        int i = k - 1;
        while(i >= 0 && current[i] == n - k + i){
            --i;
        }
        if(i < 0){
            break; // all combinations have been generated
        }
        current[i] += 1; // increment on found position
        // reset all elements to the right
        for(int j = i + 1; j < current.size(); j++){
            current[j] = current[j - 1] + 1;
        }
    }
    return res;
}

// Combinations using std::next_permutation
// generates combinations in reverse lexicographical order
// change order in initial bitmask and use std::prev_permutation to get combinations in lexicographical order
vector<vector<int>> combinationsNextPerm(const int n, const int k){
    vector<vector<int>> res;
    if(k > n || k <= 0){
        return res;
    }
    vector<int> bitmask(n, 0);
    // initiate bitmask with k ones at the end
    for(int i = 0; i < k; ++i){
        bitmask[n - 1 - i] = 1;
    }
    // next_permutation generates next item in lexicographical order
    // and will loop over all selections of k items from n (1 - selected, 0 - not selected)
    do{
        vector<int> current;
        for(int i = 0; i < n; ++i){
            if(bitmask[i] == 1){
                current.push_back(i);
            }
        }
        res.push_back(current);
    }while(std::next_permutation(bitmask.begin(), bitmask.end()));
    return res;
}

void printVecOfVec(const vector<vector<int>>& vec){
    for(const auto& v : vec){
        for(const auto& item : v){
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
}


int main(){
    auto c1 = combinationsDIM(5, 3);
    auto c2 = combinationsNextPerm(5, 3);
    printVecOfVec(c1);
    std::cout << "----" << std::endl;
    printVecOfVec(c2);
    return 0;
}
