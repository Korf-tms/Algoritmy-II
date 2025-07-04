#include<vector>
#include<iostream>
#include<functional>
#include<string>

using std::vector, std::cout, std::string;

enum State{EMPTY, DELETED, OCCUPIED};

struct Entry{
    string key;
    State state;
};    

class linearProbingTable{
    vector<Entry> table;

    size_t hashFunction(const string& key, size_t size){
        return std::hash<string>{}(key) % size;
    }

    public:

    // chtelo by to jeste pridat nastavovani deleted na empty
    bool insert(const string& key){
        size_t index = hashFunction(key, table.size());
        
        int first_deleted = -1;
        size_t probingIndex = 0;
        while( true ){
            int currentIndex = (index + probingIndex) % table.size();
            switch (table[currentIndex].state)
            {
            case OCCUPIED:
                if(table[currentIndex].key == key){
                    return false;
                }
                break;
            case EMPTY:
                if( first_deleted == -1){
                    table[currentIndex].key = key;
                    table[currentIndex].state = OCCUPIED;
                }else{
                    table[first_deleted].key = key;
                    table[first_deleted].state = OCCUPIED;
                }
                break;
            case DELETED:
                if( first_deleted == -1){
                first_deleted = currentIndex;
                }
                break;
            default:
                break;
            }
            probingIndex += 1;
            probingIndex = probingIndex % table.size();
        }
    }

};


class naiveHashTable{
    vector<vector<string>> table;
    int numberOfKeys;
    double maxOccupancy;
    double resizeFactor;

    size_t hashFunction(const string& key, size_t size){
        return std::hash<string>{}(key) % size;
    }

    bool needsResize(){
        return numberOfKeys / static_cast<double>(table.size()) > maxOccupancy;
    }

    void resize(){
        size_t newSize = static_cast<size_t>(table.size() * resizeFactor);
        vector<vector<string>> newTable(newSize);

        for(const auto& bucket : table){
            for(const auto& key : bucket){
                size_t index = hashFunction(key, newSize);
                newTable[index].push_back(key);
            }
        }
        table = newTable; // copy :(
        // table = std::move(newTable)
    }

    public:
    
    naiveHashTable(size_t size, double maxOcc, double resizeF) :
        table(vector<vector<string>>(size)), numberOfKeys(0), maxOccupancy(maxOcc), resizeFactor(resizeF)  {}

    bool insert(const string& key){
        if( needsResize() ){
            resize();
        }
        size_t index = hashFunction(key, table.size());
        if ( find(table[index].begin(), table[index].end(), key) == table[index].end() ){
            numberOfKeys += 1;
            table[index].push_back(key);
            return true;
        }
        return false;
    }

    bool remove(const string& key){
        size_t index = hashFunction(key, table.size());
        auto index_it = find(table[index].begin(), table[index].end(), key);
        if(index_it != table[index].end()){
            numberOfKeys -= 1;
            table[index].erase(index_it);
            return true;
        }
        return false;
    }

    bool lookup(const string& key){
        size_t index = hashFunction(key, table.size());
        auto index_it = find(table[index].begin(), table[index].end(), key);
        return index_it != table[index].end();
    }

    void print(){
        size_t position = 0;
        for(const auto& bucket : table){
            cout << position << ": ";
            position += 1;
            for(const auto& item : bucket){
                cout << item << " ";
            }
            cout << "\n";
        }
        return;
    }

};

int main() {
    naiveHashTable table(10, 0.7, 2);

    const auto inputs = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o"};
    for(const auto& item : inputs){
        table.insert(item);
    }
    table.print();
    return 0;
}
