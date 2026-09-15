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

void testReadNumbers(std::string filename){
    std::vector<int> numbers;

    numbers = readIntegersFromFile(filename);

    for(const auto& item : numbers){
        std::cout << item << " ";
    }
    std::cout << "\n";
}

std::vector<std::string> readWords(const std::string& path) {
    std::ifstream in(path);

    std::vector<std::string> words;
    std::string token;
    while (in >> token) {  // read by >> stops on whitespace
        std::string w;
        for (unsigned char c : token)
            if (std::isalpha(c))  // TODO: unsigned char: isalpha UB otherwise, also islocal depends on locale what is better than isalpha?
                w += static_cast<char>(std::tolower(c)); // NOTE: tolower is potential source of pain, depends on locale as well
        if (!w.empty())
            words.push_back(std::move(w));
    }
    return words;
}

void testReadWords(const std::string filename){
    const auto words = readWords(filename);

    for(const std::string word : words){
        std::cout << word << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string filename = argv[1];
    // testReadNumbers(filename);
    testReadWords(filename);
    return 0;
}
