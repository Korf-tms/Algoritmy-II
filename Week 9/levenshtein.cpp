#include<string>
#include<vector>
#include<iostream>
#include<algorithm>

using std::vector, std::string;

// compute Levenshtein distance between source and target
int levenshteinDistanceDP(const string& source, const string& target){
    const size_t lenSource = source.size();
    const size_t lenTarget = target.size();

    // dp[i][j] - vzdalenost mezi source[:i] a target[:j]
    vector<vector<int>> dp(lenSource + 1, vector<int>(lenTarget + 1));

    for(size_t row = 0; row < lenSource + 1; row++){
        dp[row][0] = static_cast<int>(row);
    }

    for(size_t col = 0; col < lenTarget + 1; col++){
        dp[0][col] = static_cast<int>(col);
    }

    // naive fill by rows
    for(size_t row = 1; row < lenSource + 1; row++){
        for(size_t col = 1; col < lenTarget + 1; col++){
            int cost = 0;
            if(target[col - 1] != source[row - 1]){
                cost += 1;
            }

            dp[row][col] = std::min({
                dp[row - 1][col] + 1, // zacnu z kratsiho slova, pridam co potrebuju a podivam se do tabulky
                dp[row][col - 1] + 1, // zacnu z delsiho solva, uberu a pak se podivam do tabulky
                dp[row - 1][col - 1] + cost // zacnu z kratsiho, ciluju kratsi, pridavam na konec
            });
        }
    }
    return dp.back().back();
}

enum class Operation {
    NOOP, INSERTION, DELETION, SUBSTITUTION, MATCH
};

struct Step
{
    Operation op;
    char changedFrom = '\0'; // NOTE: string " ", char ' ', \0 is empty char
    char changedTo = '\0';

    Step(Operation iOp, char iFrom, char iTo) : op(iOp), changedFrom(iFrom), changedTo(iTo) {}
};

struct Node{
    int cost;
    Operation op;
};

// for fun, function that also returns (only) one of the optimal paths
std::vector<Step> levenshteinWithPath(const string& source, const string& target){
    const size_t lenSource = source.size();
    const size_t lenTarget = target.size();

    vector<vector<Node>> dp(lenSource + 1, vector<Node>((lenTarget + 1)));

    for(size_t row = 1; row < lenSource + 1; row++){
        dp[row][0] = {static_cast<int>(row), Operation::DELETION};
    }

    for(size_t col = 1; col < lenTarget + 1; col++){
        dp[0][col] = {static_cast<int>(col), Operation::INSERTION};
    }

    dp[0][0] = {0, Operation::NOOP};

    vector<int> costs;

    for(size_t row = 1; row < lenSource + 1; row++){
        for(size_t col = 1; col < lenTarget + 1; col++){
            int diagonalMoveCost = target[col - 1] == source[row - 1] ? 0 : 1;

            costs = {
                dp[row - 1][col].cost + 1, // zacnu z kratsiho slova, pridam co potrebuju a podivam se do tabulky
                dp[row][col - 1].cost + 1, // zacnu z delsiho solva, uberu a pak se podivam do tabulky
                dp[row - 1][col - 1].cost + diagonalMoveCost // zacnu z kratsiho, ciluju kratsi, pridavam na konec
            };

            const auto minCost = std::min_element(costs.begin(), costs.end());
            int location = minCost - costs.begin();
            dp[row][col].cost = *minCost;
            switch (location)
            {
            case 0:
                dp[row][col].op = Operation::DELETION;
                break;
            case 1:
                dp[row][col].op = Operation::INSERTION;
                break;
            case 2:
                if(diagonalMoveCost == 0){
                    dp[row][col].op = Operation::MATCH;
                }else{
                    dp[row][col].op = Operation::SUBSTITUTION;
                }
                break;
            default:
                std::cout << "This should never happen" << std::endl;
                break;
            }

        }
    }
    std::cout << "Distance is: " << dp.back().back().cost << "\n";
    
    // table is ready, go back to get the path to victory
    int row = lenSource;
    int col = lenTarget;
    vector<Step> path;
    while(row > 0 or col > 0){
        const Operation op = dp[row][col].op;

        char sourceChar = (row > 0) ? source[row - 1] : '\0'; // first row and col correspond to empty char
        char targetChar = (col > 0) ? target[col - 1] : '\0';

        switch (op)
        {
        case Operation::MATCH:
        // note there is no break and we continue to the case of SUBSTITUTION
        case Operation::SUBSTITUTION:
            path.emplace_back(op, sourceChar, targetChar);
            row -= 1; col -= 1;
            break;
        case Operation::INSERTION:
            path.emplace_back(op, ' ', targetChar);
            col -= 1;
            break;
        case Operation::DELETION:
            path.emplace_back(op, sourceChar, ' ');
            row -= 1;
            break;
        case Operation::NOOP:
            row = 0; col = 0;
            break;    
        default:
            std::cerr << "This should never happen" << std::endl;
            break;
        }
    }
    std::reverse(path.begin(), path.end());
    return path;
}


// for fun, print the path
void printPath(const std::string& source, const std::string& target, const std::vector<Step>& path) {
    std::cout << "Optimal path to transform '" << target << "' into '" << source << "':\n";
    int cost = 0;
    
    for (const auto& step : path) {
        // Since we want the path from TARGET to SOURCE, we invert the operations' meaning.
        switch (step.op) {
            case Operation::MATCH:
                std::cout << "Match '" << step.changedTo << "'.\n";
                break;
            case Operation::SUBSTITUTION:
                std::cout << "Substitute '" << step.changedTo << "' with '" << step.changedFrom << "'.\n";
                cost++;
                break;
            case Operation::INSERTION: // Insertion into SOURCE means Deletion from TARGET
                std::cout << "Delete '" << step.changedTo << "'.\n";
                cost++;
                break;
            case Operation::DELETION: // Deletion from SOURCE means Insertion into TARGET
                std::cout << "Insert '" << step.changedFrom << "'.\n";
                cost++;
                break;
            case Operation::NOOP: // Position [0, 0], should not appear in the final path
                break;
        }
    }
    std::cout << "Final string: '" << source << "'\n";
    std::cout << "Total Levenshtein Distance: " << cost << std::endl;
}

// test for the simple function computing only distance
void test() {
    string start = "doga";
    string target = "dofga";
    int res = levenshteinDistanceDP(start, target);
    std::cout << "Distance from " << start << " to " << target << " is: " << res << "\n";
}

// test for the path-finding implementation
void test2() {
    string start = "droga";
    string target = "dofg";
    std::cout << "Computing path from " << start << " to " << target << "\n";
    auto path = levenshteinWithPath(start, target);
    printPath(start, target, path);
}

int main(){
    test();
    test2();
    return 0;
}