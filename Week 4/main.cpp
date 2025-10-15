#include<vector>
#include<exception>
#include<iostream>
#include<algorithm>

using std::vector;

class Heap{
    private:
    vector<int> data;
    
    size_t parent(size_t index) const {
        if(index == 0){return 0;} // note that heap root has no parent
        return (index - 1) / 2;
    }

    void heapify(size_t index){
        size_t largest = index;
        size_t children[2] = {2*index + 1, 2*index + 2};

        for(const auto child : children){
            if(child >= data.size()){ break;} // indices can be out of bounds
            if(data[child] > data[largest]){
                largest = child;
            }
        }
        if(largest != index){
            std::swap(data[index], data[largest]);
            heapify(largest);  // restore heap property on subtree
        }
    }

    void makeHeap(){
        // build heap from bottom to top
        for(int i = data.size() / 2 - 1; i >= 0; i--){
            heapify(i);
        }
    }

    public:
    Heap() = default;

    Heap(const vector<int>& input){
        data = input;
        makeHeap();
    }

    Heap(vector<int>&& input){
        data = std::move(input);
        makeHeap();
    }

    void insert(const int value){
        size_t currentIndex = data.size();
        data.push_back(value);
        size_t parentIndex = parent(currentIndex);

        // repair heap property by pushing the new number up
        while(currentIndex != 0 and data[currentIndex] > data[parentIndex]){
            std::swap(data[currentIndex], data[parentIndex]);
            currentIndex = parentIndex;
            parentIndex = parent(currentIndex);
        }
    }

    int getMax(){
        if( data.empty() ){
            throw std::out_of_range("Trying to extract item from empty heap");
        }
        int maxValue = data[0];

        data[0] = data.back();
        data.pop_back();
        if( data.size() > 1){
            heapify(0);
        }

        return maxValue;
    }

    void print() const {
        for(const int item : data){
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    void clear() {
        data.clear();
    }

};


void test(){
    vector<int> data = {10, 11, 1, 2, 3, 100, 200};
    Heap heap;
    for(const int item : data){
        heap.insert(item);
    }
    Heap heap2(std::move(data));
    std::cout << "Note that these constructions do not lead to same internal vector.\n";
    heap2.print();
    heap.print();

}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
    test();
    return 0;
}