#include<iostream>
#include<vector>
#include<algorithm>
#include<map>

using std::vector;

bool containsOnlyUniqueValues(const vector<int>& vec){
    // note that being sorted is crucial here
    for(size_t i = 1; i < vec.size(); i++){
        if(vec.at(i-1) == vec.at(i)){
            return false;
        }
    } 
    return true;
}

int modusFromSortedArray(const vector<int>& vec){
    int modus;
    int frequency = 0;
    int i = 0;
    int runLength;
    int runValue;
    
    while(i < vec.size()){
        runLength = 1;
        runValue = vec.at(i);
        // compute frequency of the item runValue
        while(i + runLength < vec.size() and vec.at(i + runLength) == runValue){
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

int modusFromFrequencyMap(const vector<int>& vec){
    std::map<int, int> frequencyMap; // pairs of number -> frequency

    // construct the map
    for(const int item : vec){
        frequencyMap[item] += 1;
    }

    // find max in the map
    int modusFrequency = 0;
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

int main(){
    testModus();
    return 0;
}