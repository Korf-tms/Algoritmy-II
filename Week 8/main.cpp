#include<vector>
#include<optional>
#include<string>
#include<algorithm>
#include<iostream>

using std::vector, std::string;

class naiveHashTable{
    private:
    // we will only consider growing the table, no shrinking
    vector<vector<string> > table;
    size_t number_of_keys;
    double maxOccupancy;  // ratio of items to outer table size
    double resizeFactor;  // how much to resize the outer table

    size_t hashFunction(const string& key, const size_t modFactor) const {
        return std::hash<string>{}(key) % modFactor;
    }

    bool needsResizeWhenAddingAkey() const {
        return number_of_keys / static_cast<double>(table.size()) > maxOccupancy;
    }

    void resize(){
        size_t newSize = static_cast<size_t>(table.size() * resizeFactor);
        auto oldTable = std::move(table);
        table.assign(newSize, {});

        // rehash all data after changing the size of the table
        for(const auto& bucket : oldTable){
            for(const auto& item : bucket){
                insert(item);
            }
        }
    }

    public:

    naiveHashTable(size_t size, double maxOcc, double resizeF) :
        table(size), number_of_keys(0), maxOccupancy(maxOcc), resizeFactor(resizeF) {}

    bool lookUp(const string& key) const {
        size_t index = hashFunction(key, table.size());
        return std::find(table[index].begin(), table[index].end(), key) != table[index].end();
    }

    bool insert(const string& key){
        if( lookUp(key) ){ return false;}
        number_of_keys += 1;
        // note we are resizing before adding the new key
        if( needsResizeWhenAddingAkey() ){
            resize();
        }
        size_t index = hashFunction(key, table.size());
        table[index].push_back(key);
        return true;
    }

    bool remove(const string& key){
        if( not lookUp(key) ){ return false;}
        size_t index = hashFunction(key, table.size());
        auto key_it = std::find(table[index].begin(), table[index].end(), key);
        // table[index].erase(key_it);
        std::swap(table[index].back(), *key_it);
        table[index].pop_back();
        number_of_keys -= 1;
        return true;
    }

    void print() const {
        for(size_t bucket = 0; bucket < table.size(); bucket++){
            std::cout << bucket << ": ";
            for(const auto& item : table[bucket]){
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
        return;
    }
};


enum State{EMPTY, DELETED, OCCUPIED};

struct Entry{
    string key;
    State state;
};

class linearProbingTable{
    private:
    vector<Entry> table;
    size_t number_of_keys;
    double maxOccupancy;  // ratio of items to outer table size
    double resizeFactor;  // how much to resize the outer table

    bool needsResizeWhenAddingAkey() const {
        // TODO: resize based on too many DELETED in the table
        return (number_of_keys + 1) / static_cast<double>(table.size()) > maxOccupancy;
    }

    void resize(){
        size_t newSize = static_cast<size_t>(table.size() * resizeFactor);
        
        vector<Entry> oldTable = std::move(table);
        table.assign(newSize, Entry{"", EMPTY}); // Re-initialize the table with new size
        number_of_keys = 0; 

        // Re-hash all previously occupied elements into the new, larger table.
        // This also cleans up all DELETED slots.
        for(const auto& entry : oldTable){
            if(entry.state == OCCUPIED){
                insert(entry.key); // use insert logic for re-hashing
            }
        }
    }

    size_t hashFunction(const string& key, size_t size) const {
        return std::hash<string>{}(key) % size;
    }

    public:

    linearProbingTable(size_t size, double resizeF, double maxOcc=0.7) :
        number_of_keys(0), maxOccupancy(maxOcc), resizeFactor(resizeF) {
            table.assign(size, Entry{"", EMPTY});
        }

    bool lookUp(const string& key) const {
        size_t initialIndex = hashFunction(key, table.size());
        for(size_t i = 0; i < table.size(); i++){
            size_t currentIndex = (initialIndex + i) % table.size();
            const Entry& entry = table.at(currentIndex);

            if(entry.state == EMPTY){
                return false;
            }
            if(entry.state == OCCUPIED and entry.key == key){
                return true;
            }
            // else we continue on our merry way
        }
        return false; // probed the table and found nothing
    }

    bool insert(const string& key){
        if(needsResizeWhenAddingAkey()){
            resize();
        }

        size_t initialIndex = hashFunction(key, table.size());
        std::optional<size_t> insertIndex = std::nullopt;

        for(size_t i = 0; i < table.size(); i++){
            size_t currentIndex = (initialIndex + i) % table.size();
            Entry& entry = table.at(currentIndex);

            if(entry.state == OCCUPIED and entry.key == key){
                return false; // key is already there, nothing to insert
            }

            if(entry.state == DELETED){
                // potential place for insertion, if the key is in fact not somewhere farther away
                // we save the position as candidate for insertion
                if( not insertIndex.has_value() ){
                    insertIndex = currentIndex;
                }
            }

            if(entry.state == EMPTY){
                // empty slot means either free estate or go back to first found insertInder at DELETED position
                // insertion is deff happening
                if( not insertIndex.has_value() ){
                    insertIndex = currentIndex;
                }
                table[insertIndex.value()].key = key;
                table[insertIndex.value()].state = OCCUPIED;
                number_of_keys += 1;
                return true;
            }
        }

        // getting here means there was no EMPTY slot, we can safely insert to the first found DELETED slot
        if( insertIndex.has_value() ){
            table[insertIndex.value()].key = key;
            table[insertIndex.value()].state = OCCUPIED;
            number_of_keys += 1;
            return true;
        }
        std::cerr << "This should never happen\n";
        return false;
    }
    
    bool remove(const string& key){
        size_t initialIndex = hashFunction(key, table.size());
        for (size_t i = 0; i < table.size(); ++i) {
            size_t currentIndex = (initialIndex + i) % table.size();
            Entry& entry = table[currentIndex];

            if (entry.state == EMPTY) {
                return false; // Key not in table
            }
            if (entry.state == OCCUPIED && entry.key == key) {
                entry.state = DELETED; // Mark as deleted, don't set to empty
                entry.key.clear();     // "clear" the string
                number_of_keys -= 1;
                return true;
            }
        }
        return false; // Key not found after full table pass
    }

    void print() const {
        std::cout << "Table with size: " << table.size() << " and " << number_of_keys << "keys.\n";
        for(size_t i = 0; i < table.size(); i++){
            std::cout << i << ": ";
            switch (table[i].state)
            {
            case EMPTY: std::cout << "empty\n"; break;
            case DELETED: std::cout << "deleted\n"; break;
            case OCCUPIED: std::cout << table[i].key << "\n"; break;
            default:
                std::cout << "what?\n";
                break;
            }
        }
    }

};


int main(){
    // generated demo
    linearProbingTable lp_table(10, 0.7, 2.0);

    std::cout << "Inserting initial keys...\n";
    lp_table.insert("apple");
    lp_table.insert("banana"); // collides with apple
    lp_table.insert("cherry");
    lp_table.insert("date");
    lp_table.insert("grape");
    lp_table.insert("fig");
    lp_table.insert("lemon"); // 7th element, triggers resize (7+1)/10 > 0.7
    lp_table.print();

    std::cout << "Looking up 'banana': " << (lp_table.lookUp("banana") ? "Found" : "Not Found") << std::endl;
    std::cout << "Looking up 'mango': " << (lp_table.lookUp("mango") ? "Found" : "Not Found") << std::endl;
    
    std::cout << "\nRemoving 'banana'...\n";
    lp_table.remove("banana");
    lp_table.print();

    std::cout << "Looking up 'cherry' (should still be findable after 'banana' deletion): " 
              << (lp_table.lookUp("cherry") ? "Found" : "Not Found") << std::endl;

    std::cout << "\nInserting 'mango' (should fill the DELETED spot)...\n";
    lp_table.insert("mango");
    lp_table.print();
    
    return 0;
}
