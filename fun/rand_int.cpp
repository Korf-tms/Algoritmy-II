#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

std::vector<int> generateRandomVector(int length, int minVal, int maxVal) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(minVal, maxVal);

    std::vector<int> result;
    result.reserve(length);

    for (int i = 0; i < length; ++i) {
        result.push_back(dis(gen));
    }

    return result;
}

int main() {
    int length = 1000000; // Change the length as needed
    int minVal = 0;  // Minimum value of generated integers
    int maxVal = 1000000; // Maximum value of generated integers

    // Generate random vector
    std::vector<int> randomVector = generateRandomVector(length, minVal, maxVal);

    // Measure sorting time
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(randomVector.begin(), randomVector.end());
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Output sorting time
    std::cout << "Time taken to sort the vector: " << duration.count() << " seconds" << std::endl;

    return 0;
}
