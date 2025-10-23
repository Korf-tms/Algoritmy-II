#include<vector>
#include<set>
#include<iostream>
#include<queue>
#include<string>
#include<map>

using std::vector, std::set, std::string;

class State {
    private:
    set<string> finalState = {"K", "V", "Z", "P"};

    bool isValid(std::set<string>& internalState) const {
            bool hasP = internalState.count("P");

             if (hasP) {
                return true;
            }
            if (internalState.count("V") and internalState.count("K")) {
                return false;
            }
            if (internalState.count("K") and internalState.count("Z")) {
                return false;
            }
            return true;
        }

    public:
    std::set<string> bank1, bank2;

    State() : bank1({"K", "V", "Z", "P"}), bank2() {}
    State(std::set<string>& b1, std::set<string>& b2) : bank1(b1), bank2(b2) {}

    bool operator<(const State& other) const {
        std::string left, right;
        for(const auto& item : bank1){
            left += item;
        }
        for(const auto& item : other.bank1){
            right += item;
        }
        return left < right;
    }

    bool isFinal() const {
        return finalState == bank2;
    }

    vector<State> generateChildren() const {
        vector<State> res;

        std::set<string> activeBank;
        std::set<string> targetBank;
        bool bank1HasP = bank1.find("P") != bank1.end();
        if( bank1HasP ){
            activeBank = bank1;
            targetBank = bank2;
        } else{
            activeBank = bank2;
            targetBank = bank1;
        }

        // transport P alone
        std::set<string> proposedA, proposedT;
        proposedA = activeBank;
        proposedT = targetBank;
        proposedA.erase("P");
        proposedT.insert("P");

        if( isValid(proposedA) and isValid(proposedT)){
            if( bank1HasP ){
                res.emplace_back(proposedA, proposedT);
            } else{
                res.emplace_back(proposedT, proposedA);
            }
        }

        // take other stuff
        for(const auto& item : activeBank){
            proposedA = activeBank;
            proposedT = targetBank;
            if(item == "P"){
                continue;
            }
            proposedA.erase("P");
            proposedA.erase(item);
            proposedT.insert("P");
            proposedT.insert(item);
            if( isValid(proposedA) and isValid(proposedT)){
                if( bank1HasP ){
                    res.emplace_back(proposedA, proposedT);
                } else{
                    res.emplace_back(proposedT, proposedA);
                }
            }
        }
        return res;
    }

    void print() const {
        for(const auto& item : bank1){
            std::cout << item;
        }
        std::cout << " ";
        for(const auto& item : bank2){
            std::cout << item;
        }
        std::cout << "\n";
    }

    bool operator==(const State& other) const{
        return (bank1 == other.bank1) and (bank2 == other.bank2);
    }
};

vector<State> solveKVZ(){
    State start;

    std::map<State, State> child2parentMap;
    std::queue<State> bfsQ;

    bfsQ.push(start);
    child2parentMap[start] = start;

    // TODO: fix visited and drop useless data
    std::set<State> visited; // super naive, should at least keep only visited from last layer
    visited.insert(start);
    bool victoryFlag = false;

    vector<State> path;

    while(not bfsQ.empty() and not victoryFlag){
        size_t numNodes = bfsQ.size();
        for(size_t i = 0; i < numNodes and not victoryFlag; i++){
            State currentNode = bfsQ.front();
            bfsQ.pop();
            visited.insert(currentNode);

            auto children = currentNode.generateChildren();

            for(auto child : children){
                if( visited.find(child) != visited.end()){
                    continue;
                }
                bfsQ.push(child);
                child2parentMap[child] = currentNode;
                visited.insert(child);
                if( child.isFinal()){
                    path.push_back(child);
                    State parent = child2parentMap[child];
                    while( not (child == parent) ){
                        path.push_back(parent);
                        child = parent;
                        parent = child2parentMap[child];
                    }
                    victoryFlag = true;
                    break;
                }
            }
        }
    }
    return path;
}

int main(){
    const auto res = solveKVZ();
    for(const auto& line : res){
        line.print();
    }
    return 0;
}
