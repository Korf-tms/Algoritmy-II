#include <iostream>
#include <vector>
#include <unordered_map>

using std::unordered_map, std::vector;

class SetPartitions {
private:
    unordered_map<int, int> parent; // element -> parent/representant of element
    unordered_map<int, int> sizes; // element -> size of its partition

public:
    // Add a new element as its own partition
    void makeSet(int x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x; // note that this is defining property of representant
            sizes[x] = 1;
        }
    }

    // Find the representative of the set containing x
    int findRootRepresentative(int x) {
        if (parent[x] != x) {
            parent[x] = findRootRepresentative(parent[x]); // Path compression
        }
        return parent[x];
    }

    // Union two partitions based on their representatives
    void unionSubsets(int x, int y) {
        int rootX = findRootRepresentative(x);
        int rootY = findRootRepresentative(y);

        if (rootX == rootY) {
            return; // Already in the same partition
        }

        // Ensure rootX represents the smaller set
        if (sizes[rootX] > sizes[rootY]) {
            std::swap(rootX, rootY);
        }

        // Merge smaller set into the larger one
        parent[rootX] = rootY;
        sizes[rootY] += sizes[rootX]; // note that only the representatnt of the subset holds corrent subset size
    }

    // Check if two elements belong to the same partition
    bool belongsToTheSamePartition(int x, int y) {
        return findRootRepresentative(x) == findRootRepresentative(y);
    }

    // Print all partitions
    void printPartitions() {
        unordered_map<int, vector<int>> sets;
        for (const auto& [element, parent_element] : parent) {
            sets[findRootRepresentative(element)].push_back(element);
        }

        std::cout << "Printing partitions:\n";
        for (const auto& [representative, partition] : sets) {
            std::cout << "Partition led by " << representative << ": ";
            for (const auto& item : partition) {
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    SetPartitions myPartitions;
    auto elements = {1, 2, 3, 4, 5};

    for (int item : elements) {
        myPartitions.makeSet(item);
    }

    myPartitions.unionSubsets(1, 2);
    myPartitions.printPartitions();
    myPartitions.unionSubsets(1, 3);
    myPartitions.printPartitions();
    myPartitions.unionSubsets(4, 5);
    myPartitions.printPartitions();

    std::cout << "Are 1 and 2 in the same partition? "
              << (myPartitions.belongsToTheSamePartition(1, 2) ? "Yes" : "No") << "\n";
    std::cout << "Are 3 and 4 in the same partition? "
              << (myPartitions.belongsToTheSamePartition(3, 4) ? "Yes" : "No") << "\n";

    return 0;
}
