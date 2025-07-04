#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using std::cout, std::vector;


// reseni z posledniho cviceni tydne
// cool vlastni struktura, ktera ovsem generuje komplikace
struct riverBank{
    bool wolf; //true if present
    bool goat;
    bool cabbage;
    bool ferryman;

    riverBank() = default;

    riverBank(bool inWolf, bool inGoat, bool inCabbage, bool inFerryman) :
        wolf(inWolf), goat(inGoat), cabbage(inCabbage), ferryman(inFerryman) {};

    bool isValid() const{
        if( ferryman ){
            return true;
        }
        if( (goat and cabbage) or (goat and wolf) ){
            return false;
        }
        return true;
    }

    bool operator[](const int i) const{
        if(i == 0){
            return wolf;
        }
        if( i == 1){
            return goat;
        }
        if( i == 2){
            return cabbage;
        }
        throw std::invalid_argument("input value must be 0, 1, 2");
    }

    bool& operator[](const int i) {
        if(i == 0){
            return wolf;
        }
        if( i == 1){
            return goat;
        }
        if( i == 2){
            return cabbage;
        }
        throw std::invalid_argument("input value must be 0, 1, 2");
    }

    bool operator==(const riverBank& other) const{
        if( wolf != other.wolf ){
            return false;
        }
        if( goat != other.goat ){
            return false;
        }
        if( cabbage != other.cabbage ){
            return false;
        }
        if( ferryman != other.ferryman){
            return false;
        }
        return true;
    }
};


class State{
    riverBank left;
    riverBank right;     
    public:

    State(const riverBank& inLeft, const riverBank& inRight){
        left = inLeft;
        right = inRight;
    }

    State(){
        right = riverBank(false, false, false, false);
        left =  riverBank(true, true, true, true);
    }
    
    vector<State> generateChildren() const {
        vector<State> result;

        // prevoznik jede sam
        riverBank potentialLeft = left;
        riverBank potentialRight = right;
        potentialLeft.ferryman = not potentialLeft.ferryman;
        potentialRight.ferryman = not potentialRight.ferryman;
        if(potentialLeft.isValid() and potentialRight.isValid()){
            State newState(potentialLeft, potentialRight);
            result.push_back(newState);
        }

        // prevoznik nekoho veze
        for(int i = 0; i < 3; i++){
            riverBank potentialLeft = left;
            riverBank potentialRight = right;
            // kdo je na stejnem brehu jako prevoznik, tak muze prejet
            if( (left.ferryman and left[i]) or (right.ferryman and right[i]) ){
                potentialLeft.ferryman = not potentialLeft.ferryman; 
                potentialRight.ferryman = not potentialRight.ferryman;
                potentialLeft[i] = not potentialLeft[i];
                potentialRight[i] = not potentialRight[i];
            }
            if(potentialLeft.isValid() and potentialRight.isValid()){
                State newState(potentialLeft, potentialRight);
                result.push_back(newState);
            }
        }
        return result;
    };

    bool isFinal() const{
        const riverBank finalRight =  riverBank(true, true, true, true);
        return right == finalRight;

    }

    bool operator==(State other){
        return (left == other.left) and (right == other.right);
    }

};

int main(){
    // bylo by fajn vyuzit set/unordered set, ale to znamena definovat dalsi operatory
    // jako operator< nebo hashovaci funkci
    vector<State> visited; 
    std::queue<State> BFS_queue;
    State initialState;

    int counter = 0;
    bool solutionFound = false;

    BFS_queue.push(initialState);
    visited.push_back(initialState);

    while(not solutionFound and not BFS_queue.empty()){
        counter += 1;
        size_t length = BFS_queue.size();
        for(size_t i = 0; (i < length) and not solutionFound; i++){
            State current = BFS_queue.front();
            BFS_queue.pop();
            auto children = current.generateChildren();
            for(const auto& child : children){
                if( child.isFinal() ){
                    solutionFound = true;
                    break;
                }
                if( std::find(visited.begin(), visited.end(), child) == visited.end() ){
                    visited.push_back(child);
                    BFS_queue.push(child);
                }
            }
        }
    }

    cout << solutionFound << "\n";
    cout << "moves " << counter << "\n";

    return 0;
}