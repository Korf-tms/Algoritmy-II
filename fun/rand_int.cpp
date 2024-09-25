#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

std::vector<int> generateRandomVector(int length, int minVal, int maxVal, int seed) {
    std::mt19937 generator;  // 32-bit random int generator based on Mersenne Twister
    std::uniform_int_distribution<int> dis(minVal, maxVal);  // the actual distribution

    std::vector<int> result;
    result.reserve(length);

    generator.seed(seed);  // naive seed setup, do not use for anything important
    for (int i = 0; i < length; ++i) {
        result.push_back(dis(generator));
    }

    return result;
}

int main() {
    int length = 10; // Change the length as needed
    int minVal = -100;  // Minimum value of generated integers
    int maxVal = 300; // Maximum value of generated integers

    // generator is seeded, we can either take a random seed a prescribe one
    // pick one, comment the other
    // std::random_device rd;
    // int seed = rd();

    int seed = 6;

    // Generate random vector
    std::vector<int> randomVector = generateRandomVector(length, minVal, maxVal, seed);

    // check the generated vector
    for(int item : randomVector){
        std::cout << item << " ";
    }
    std::cout << "\n";

    // Measure sorting time
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(randomVector.begin(), randomVector.end());
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Output sorting time
    std::cout << "Time taken to sort the vector: " << duration.count() << " seconds" << std::endl;

    return 0;
}
