#include <vector>
#include <iostream>
#include <cmath>

using std::vector, std::cout, std::size_t, std::pow;

class myHeap{
    private:
    vector<int> data;

    void heapify(const size_t parent){
        size_t child1 = 2*parent + 1;
        size_t child2 = 2*parent + 2;
        auto children = {child1, child2};

        size_t largest = parent;
        for(const auto child : children){
            if(child < data.size() and data.at(child) > data.at(largest)){
                largest = child;
            }
        }
        if(parent != largest){
            std::swap(data[parent], data[largest]);
            heapify(largest);
        }
    }

    void makeHeap(){
        for(int i=data.size()/2 - 1; i >=0; i--){
            heapify(i);
        }
    }

    public:

    myHeap(const vector<int>& input_data){
        data = input_data;
        makeHeap();        
    }

    myHeap(vector<int>&& input_data){
        data = std::move(input_data);
        makeHeap();
    }

    bool isEmpty() const{
        return data.empty();
    }

    void makeEmpty(){
        data.resize(0);
    }

    void insert(const int value){
        data.push_back(value);
        
        // now to restore the heap property
        // instead of heapify, we will go from bottom to the top
        size_t n = data.size();
        if( n == 1 ){
            return;
        }
        
        size_t current_index = n - 1;
        size_t parent_index = (current_index - 1)/2;
        
        while( current_index > 0 and data.at(parent_index) < data.at(current_index)){
            std::swap(data.at(parent_index), data.at(current_index));
            current_index = parent_index;
            parent_index = (current_index - 1)/2;
        }
    }
    
    int getMax(){
        int value = data[0];
        data[0] = data[data.size()-1];
        data.resize(data.size() - 1);

        // restore heap property
        heapify(0);

        return value;
    }

    void print() const {
        for(const auto& item : data){
            cout << item << " "; 
        }
        cout << "\n";
    };

    void printByLevels() const {
        size_t index = 0;
        size_t n = data.size();
        int level_size = 1;
        while(index < n){
            for(size_t i = 0; index < n and i < level_size; i++){
                cout << data[index] << " ";
                index += 1;
            }
            cout << "\n";
            level_size *= 2;
        }
    }
};

int main() {
    vector<int> data_from_slides = {2, 9, 7, 6, 5, 8};
    myHeap heap(data_from_slides);

    heap.print();
    cout << "\n";
    heap.printByLevels();
    cout << "\n";

    vector<int> data2 = {11, 33, 456, 9, -1, -2, -3, 500, 460, 6};
    for(auto item : data2){
        heap.insert(item);
    }
    heap.print();
    cout << "\n";
    heap.printByLevels();
    cout << "\n";

    // this new heap has the same input data but note that the resulting data vector is different
    // vector<int> combined_data = {2, 9, 7, 6, 5, 8, 11, 33, 456, 9, -1, -2, -3, 500, 460, 6}; 
    vector<int> combined_data = data_from_slides;
    combined_data.insert(combined_data.end(), data2.begin(), data2.end());
    myHeap heap2(combined_data);
    heap2.printByLevels();
    return 0;
}