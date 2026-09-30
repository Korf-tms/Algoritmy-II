#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using itemType = int;

struct Node {
    int key;
    Node* left;
    Node* right;
    int heigth;

    Node(int k) : key(k), left(nullptr), right(nullptr), heigth(0) {}
};

class AVLTree {
    private:
    Node* root;

    int heigth(Node* node){
        // wrapper to guard from -> on nullptr
        if( node == nullptr ) return -1;
        return node->heigth;
    }

    int balanceFactor(Node* node){
        if( node == nullptr ) return 0;
        return heigth(node->left) - heigth(node->right);
    }

    void updateHeigth(Node* node){
        // NOTE: the places where is function is actuall called are somewhat nontrivial
        if( node == nullptr ) return;
        node->heigth = 1 + std::max(heigth(node->left), heigth(node->right));
    }

    Node* rotateRight(Node* r) {
        Node* c = r->left;
        Node* t2 = c->right;

        c->right = r;
        r->left = t2;

        updateHeigth(r);
        updateHeigth(c);

        return c;
    }

    Node* rotateLeft(Node* r){
        Node* c = r->right;
        Node* t2 = c->left;

        c->left = r;
        r->right = t2;

        updateHeigth(r);
        updateHeigth(c);

        return c;
    }

    Node* balance(Node* node) {
        updateHeigth(node);
        int bf = balanceFactor(node);
        // Left heavy
        if (bf > 1) {
            // Left-right heavy
            if (balanceFactor(node->left) < 0){
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        // Right heavy
        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                // Right-left heavy
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    // insertion helper function, returns the (potentially new) root of the subtree
    Node* insert(Node* node, itemType key){
        if( node == nullptr ){
            return new Node(key);
        }
        if(key < node->key){
            node->left = insert(node->left, key);
        } else if (key > node->key){
            node->right = insert(node->right, key);
        } else {
            // this implicitely assumes that not(key < node->key) and not(key > node->key) implies key == node->key
            // which is a property of the total order relation on integers, might not hold for other types
            return node;
        }

        return balance(node);
    }

    bool containsKey(Node* node, itemType key){
        if( node == nullptr ) return false;
        if( key < node->key){
            return containsKey(node->left, key);
        } else if (key > node->key){
            return containsKey(node->right, key);
        } else {
            // again, this assumes that not(key < node->key) and not(key > node->key) implies key == node->key
            return true;
        }
    }

    // function that removes a specific key, retuns the (potentially new) root of the subtree
    Node* erase(Node* node, itemType key){
        // Nodes with one or no children are easy to delete
        // Nodes with two children are deleted by replacing their key by either 
        // the largest key in their left subtree or the smallest key in their right subtree, and then deleting that node instead
        // balancing should be correctly applied on the way back up the recursion tree
        return node; // TODO: implement this function
    }

    void inorderPrint(Node* node) const {
        if(node == nullptr) return;
        inorderPrint(node->left);
        std::cout << node->key << " ";
        inorderPrint(node->right);
    }

    void preorderPrint(Node* node) const {
        if(node == nullptr) return;
        std::cout << node->key << " ";
        preorderPrint(node->left);
        preorderPrint(node->right);
    }

    void postorderPrint(Node* node) const {
        if(node == nullptr) return;
        postorderPrint(node->left);
        postorderPrint(node->right);
        std::cout << node->key << " ";
    }

    // function that clears the tree, deleting all nodes
    void clear(Node* node){
        if( node == nullptr ) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    public:

    AVLTree() : root(nullptr) {}

    ~AVLTree(){
        clear(root);
    }

    void insert(itemType key) {
        root = insert(root, key);
    }

    void erase(itemType key) {
        root = erase(root, key);
    }

    bool containsKey(itemType key){
        return containsKey(root, key);
    }

    void printPreorder() const {
        preorderPrint(root);
        std::cout << "\n";
    }

    void printPostorder() const {
        postorderPrint(root);
        std::cout << "\n";
    }

    void printInorder() const {
        inorderPrint(root);
        std::cout << "\n";
    }

    void printLevelOrder() const {
        if (root == nullptr){
            std::cerr << "Tree is empty\n";
            return;
        }

        std::queue<Node*> q;
        q.push(root);

        while(not q.empty()){
            size_t nodeCount = q.size();

            bool foundNewNode = false;
            for(size_t i = 0; i < nodeCount; i++){
                Node* currentNode = q.front();
                q.pop();

                if(currentNode != nullptr){
                    std::cerr << currentNode->key << " ";
                } else {
                    std::cerr << "# ";
                }
                if (currentNode != nullptr){
                    if(currentNode->left != nullptr or currentNode->right != nullptr){foundNewNode = true;}
                    q.push(currentNode->left);
                    q.push(currentNode->right);
                }
            }
            std::cerr << "\n";
            if(not foundNewNode){
                break;
            }
        }
    }

};

std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<int> numbersVec;

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return numbersVec;
    }

    std::string line;

    // change if to while to read the whole file
    if (std::getline(file, line)) {
        std::stringstream lineAsStream(line);
        int num;
        while (lineAsStream >> num) {  // NOTE: this assumes that the input file has the data we want
            numbersVec.push_back(num);
        }
    }

    file.close();
    return numbersVec;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
    std::vector<int> data = {5, 6, 8, 3, 2, 4, 7};

    AVLTree tree;
    for(const int item : data){
        tree.insert(item);
    }

    tree.printPostorder();
    tree.printPreorder();
    tree.printInorder();
    tree.printLevelOrder();

    return 0;
}
