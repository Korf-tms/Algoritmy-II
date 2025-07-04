#include "binary_tree.h"
#include<queue>


int binarySearchTree::height(const Node *node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int binarySearchTree::balanceFactor(const Node *node) const {
    if (node == nullptr) { // technically not needed
        return 0;
    }
    return height(node->left) - height(node->right);
}

Node *binarySearchTree::rotateRight(Node *r) {
    Node *c = r->left;
    Node *t2 = c->right; // left-left chain

    c->right = r;
    r->left = t2;

    // bruteforce update of height
    r->height = std::max(height(r->left), height(r->right)) + 1;
    c->height = std::max(height(c->left), height(c->right)) + 1;

    return c;
}

Node *binarySearchTree::rotateLeft(Node *r) {
    Node *c = r->right;
    Node *t2 = c->left;

    c->left = r;
    r->right = t2;

    // bruteforce update of height
    r->height = std::max(height(r->left), height(r->right)) + 1;
    c->height = std::max(height(c->left), height(c->right)) + 1;

    return c;
}

// insert helper function for nonempty tree
Node *binarySearchTree::insert(Node *entryPoint, int value) {
    // insert on the right place
    if (entryPoint == nullptr) {
        return new Node(value);
    }

    // lower goes to the left
    if (value < entryPoint->value) {
        entryPoint->left = insert(entryPoint->left, value);
    }
    // higher goes to the right
    if (value > entryPoint->value) {
        entryPoint->right = insert(entryPoint->right, value);
    }
    if (value == entryPoint->value) {
        throw std::invalid_argument("Duplicate values are not allowed");
    }
    // TODO: entryPoint is probably no longer a good name
    // update height
    entryPoint->height = std::max(height(entryPoint->left), height(entryPoint->right)) + 1;

    if (is_avl) {
        // rebalance, if needed
        int balance = balanceFactor(entryPoint);

        if (balance > 1 and value < entryPoint->left->value) {
            cout << "R" << entryPoint->value << "\n";
            return rotateRight(entryPoint);
        }
        if (balance < -1 and value > entryPoint->right->value) {
            cout << "L" << entryPoint->value << "\n";
            return rotateLeft(entryPoint);
        }

        // Left Right Case (LR)
        if (balance > 1 && value > entryPoint->left->value) {
            cout << "LR" << entryPoint->value << "\n";
            entryPoint->left = rotateLeft(entryPoint->left); // Update left child
            return rotateRight(entryPoint); // Return the new root
        }

        // Right Left Case (RL)
        if (balance < -1 && value < entryPoint->right->value) {
            cout << "RL" << entryPoint->value << "\n";
            entryPoint->right = rotateRight(entryPoint->right); // Update right child
            return rotateLeft(entryPoint); // Return the new root
        }
    }

    return entryPoint;

}

void binarySearchTree::deleteTree(Node *entryPoint) {
    if (entryPoint == nullptr) {
        return;
    }
    deleteTree(entryPoint->left);
    deleteTree(entryPoint->right);
    delete entryPoint;
}

void binarySearchTree::inOrderToVec(const Node *entryPoint, vector<int> &data) const {
    if (entryPoint == nullptr) {
        return;
    }
    inOrderToVec(entryPoint->left, data);
    data.push_back(entryPoint->value);
    inOrderToVec(entryPoint->right, data);
}

void binarySearchTree::postOrderToVec(const Node *entryPoint, vector<int> &data) const {
    if (entryPoint == nullptr) {
        return;
    }
    postOrderToVec(entryPoint->left, data);
    postOrderToVec(entryPoint->right, data);
    data.push_back(entryPoint->value);
}

void binarySearchTree::preOrderToVec(const Node *entryPoint, vector<int> &data) const {
    if (entryPoint == nullptr) {
        return;
    }
    data.push_back(entryPoint->value);
    preOrderToVec(entryPoint->left, data);
    preOrderToVec(entryPoint->right, data);
}


bool binarySearchTree::hasValue(const Node *entryPoint, const int value) const {
    if (entryPoint == nullptr) {
        return false;
    }
    if (entryPoint->value == value) {
        return true;
    }
    if (value < entryPoint->value) {
        return hasValue(entryPoint->left, value);
    }
    if (value > entryPoint->value) {
        return hasValue(entryPoint->right, value);
    }else{
        throw std::invalid_argument("input has unusual comparison results, not suitable for BST");
    }
}


binarySearchTree::binarySearchTree(bool param) : root(nullptr), is_avl(param) {};

void binarySearchTree::insert(const int value) {
    root = insert(root, value);
}

[[nodiscard]] vector<int> binarySearchTree::inOrderToVec() const {
    vector<int> data;
    inOrderToVec(root, data);
    return data;
}

[[nodiscard]] vector<int> binarySearchTree::postOrderToVec() const {
    vector<int> data;
    postOrderToVec(root, data);
    return data;
}

[[nodiscard]] vector<int> binarySearchTree::preOrderToVec() const {
    vector<int> data;
    preOrderToVec(root, data);
    return data;
}

bool binarySearchTree::hasValue(const int value) const {
    return hasValue(root, value);
}

[[nodiscard]] int binarySearchTree::height() const {
    return height(root);
}

binarySearchTree::~binarySearchTree() {
    deleteTree(root);
}

void binarySearchTree::levelOrderTraversal() const {
    if (root == nullptr) {
        std::cout << "X" << std::endl;
        return;
    }

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();  // Number of nodes at the current level

        for (int i = 0; i < levelSize; ++i) {
            Node* current = q.front();
            q.pop();

            if (current == nullptr) {
                std::cout << "X ";
            } else {
                std::cout << current->value << " ";
                q.push(current->left);
                q.push(current->right);
            }
        }
        std::cout << "\n";  // End of current level
    }
}
