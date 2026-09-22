#include<iostream>
#include<vector>
#include<algorithm> // std::sort
#include<map>
#include<set>

using std::vector;

// check if all values in the vector are unique
bool uniqueValuesByPresort(const vector<int>& vec){
    // transform by sorting
    auto localVec = vec;
    std::sort(localVec.begin(), localVec.end());

    // conquer by checking only neighboring values
    for(size_t i = 1; i < localVec.size(); i++){
        if(localVec[i-1] == localVec[i]){
            return false;
        }
    } 
    return true;
}

// check uniqueness by using a set
bool uniqueValuesBySet(const vector<int>& vec){
    std::set<int> setOfValues;

    for(const int item : vec){
        // if(setOfValues.find(item) != setOfValues.end()){
        if(setOfValues.count(item) == 1){
            return false;
        }
        setOfValues.insert(item);
    }
    return true;
}

// check uniqueness by using a frequency map
bool uniqueValuesByFrequencyMap(const vector<int>& vec){
    std::map<int, unsigned int> frequencyMap; // pairs of number -> frequency

    for(const int item : vec){
        frequencyMap[item] += 1;
        if(frequencyMap[item] > 1){
            return false;
        }
    }
    return true;
}

// find the modus (most frequent value) in an array of ints
int modusFromSortedArray(const vector<int>& vec){
    int modus; // solution
    int frequency = 0;
    size_t i = 0; // position of the start of the item block
    int runLength;  // counts how many same items we encounter in a row
    int runValue;  // current value of the item block

    // transform
    auto localVec = vec;
    std::sort(localVec.begin(), localVec.end());
    
    // conquer
    while(i < localVec.size()){
        runLength = 1;
        runValue = localVec[i];
        // compute frequency of the item runValue
        while(i + runLength < localVec.size() and localVec[i + runLength] == runValue){
            runLength += 1;
        }
        // update modus if needed
        if(runLength > frequency){
            frequency = runLength;
            modus = runValue;
        }
        i += runLength;
    }
    return modus;
}

// constucts a frequency map to find modus
int modusFromFrequencyMap(const vector<int>& vec){
    std::map<int, unsigned int> frequencyMap; // pairs of number -> frequency

    // construct the map
    // NOTE: C++ has a default behaviour for operatorp[] that constucts a new entry if the key does not exist yet. The default value for unsigned int is 0.
    for(const int item : vec){
        frequencyMap[item] += 1;
    }

    // find max in the map
    size_t modusFrequency = 0;
    int modus;
    for(const auto& [number, frequency] : frequencyMap){
        if(frequency > modusFrequency){
            modus = number;
            modusFrequency = frequency;
        }
    }
    return modus;
}

void testModus(){
    vector<int> data = {1, 2, 3, 1, 2, 3, 3, 3, 3, 5, 5, 2, 1, 3, 4};

    int modusFM = modusFromFrequencyMap(data);

    std::sort(data.begin(), data.end());
    int modusSA = modusFromSortedArray(data);
    std::cout << modusFM << " " << modusSA << "\n";
}

void testUniqueValues(){
    vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << uniqueValuesByPresort(data) << "\n";
    std::cout << uniqueValuesBySet(data) << "\n";
    std::cout << uniqueValuesByFrequencyMap(data) << "\n";

    data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1};
    std::cout << uniqueValuesByPresort(data) << "\n";
    std::cout << uniqueValuesBySet(data) << "\n";
    std::cout << uniqueValuesByFrequencyMap(data) << "\n";
}

int main(){
    testModus();
    testUniqueValues();
    return 0;
}