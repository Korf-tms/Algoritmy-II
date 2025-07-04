#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


// reads integers from the first line in the file
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

void test(std::string filename){
    std::vector<int> numbers;

    numbers = readIntegersFromFile(filename);

    for(const auto& item : numbers){
        std::cout << item << " ";
    }
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    std::string filename = argv[1];
    test(filename);
    return 0;
}
