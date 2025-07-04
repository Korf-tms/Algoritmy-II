#ifndef AVL_TREE_BINARY_TREE
#define AVL_TREE_BINARY_TREE

#include <vector>
#include <iostream>

using std::cout, std::vector;

struct Node {
    int value;
    Node *left;
    Node *right;
    int height;

    explicit Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class binarySearchTree {
private:
    Node *root;
    const bool is_avl;

    int height(const Node *node) const;

    int balanceFactor(const Node *node) const;

    Node *rotateRight(Node *r);

    Node *rotateLeft(Node *r);

    // insert helper function for nonempty tree
    Node *insert(Node *entryPoint, int value);

    void deleteTree(Node *entryPoint);

    void inOrderToVec(const Node *entryPoint, vector<int> &data) const;

    void postOrderToVec(const Node *entryPoint, vector<int> &data) const;

    void preOrderToVec(const Node *entryPoint, vector<int> &data) const;

    bool hasValue(const Node *entryPoint, int value) const;

public:
    explicit binarySearchTree(bool param = false);

    void insert(int value);

    [[nodiscard]] vector<int> inOrderToVec() const;

    [[nodiscard]] vector<int> postOrderToVec() const;

    [[nodiscard]] vector<int> preOrderToVec() const;

    bool hasValue(int value) const;

    int height() const;

    ~binarySearchTree();

    void levelOrderTraversal() const;
};

#endif //AVL_TREE_BINARY_TREE
