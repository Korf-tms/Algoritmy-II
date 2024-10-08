#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


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

void test(){
    std::string filename = "test_file.txt";
    std::vector<int> numbers;

    numbers = readIntegersFromFile(filename);

    for(const auto& item : numbers){
        std::cout << item << " ";
    }
    std::cout << "\n";
}

int main() {
    test();
}
