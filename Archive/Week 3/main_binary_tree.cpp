#include <iostream>
#include <vector>
#include "binary_tree.h"
#include <fstream>
#include <sstream>
#include <queue>

using std::vector, std::cout;


void printVec(const vector<int>& vec){
    for(auto& item : vec){cout << item << " ";} cout << "\n";
}

void testFromLecture(){
    binarySearchTree bst(true);
    cout << "Test from class\n";

    const auto input_numbers = {5, 6, 8, 3, 2, 4, 7};

    for(const auto item : input_numbers){
        cout << item << "\n";
        bst.insert(item);
    }
    auto vecInOrder = bst.inOrderToVec();
    auto vecPreOrder = bst.preOrderToVec();
    auto vecPostOrder = bst.postOrderToVec();
    auto vecs = {vecInOrder, vecPreOrder, vecPreOrder};

    cout << "InOrder, PreOrder, PostOrder\n";
    for (const auto& vec : vecs) {
        for (const auto &item: vec) {
            cout << item << " ";
        }
        cout << "\n";
    }
    cout << "\n";

}

void testFromWikipedia(){
    binarySearchTree bst;

    cout << "Test from Wikipedia\n";
    const auto inputs = {5, 1, 0, 6, 3, 2, 4, 8, 7};

    for (auto item : inputs){
        bst.insert(item);
    }

    auto vecInOrder = bst.inOrderToVec();
    auto vecPreOrder = bst.preOrderToVec();
    auto vecPostOrder = bst.postOrderToVec();

    auto vecs = {vecPreOrder, vecInOrder, vecPostOrder};

    char mapping[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    for (const auto& vec : vecs) {
        for (const auto &item: vec) {
            cout << mapping[item] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    bst.levelOrderTraversal();  
}

std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<int> numbersVec;

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return numbersVec;
    }

    std::string line;
    if (std::getline(file, line)) {
        std::stringstream lineAsStream(line);
        int num;
        while (lineAsStream >> num) {
            numbersVec.push_back(num);
        }
    }

    file.close();
    return numbersVec;
}

int main(int argc, char* argv[]) {
    // testFromLecture();
    // testFromWikipedia();

    std::string filename = argv[1];

    auto inputs = readIntegersFromFile(filename);
    binarySearchTree bst;
    for(int item : inputs){
        bst.insert(item);
    }
    auto po = bst.postOrderToVec();
    auto pre = bst.preOrderToVec();
    auto in = bst.inOrderToVec();

    auto vecs = {po, pre, in};

    for(const auto& vec: vecs){
        printVec(vec);
    }

    bst.levelOrderTraversal();
    return 0;
}
