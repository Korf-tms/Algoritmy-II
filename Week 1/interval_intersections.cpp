/* Levitin exercise 8 in section 6.1 Presorting
 * You have a list of n open intervals (a1, b1), (a2, b2 ), . . . , (an , bn) on the real line.
 * (An open interval (a, b) comprises all the points strictly between its endpoints
 * a and b, i.e., (a, b) = {x| a < x < b}.) Find the maximum number of these
 * intervals that have a common point. For example, for the intervals (1, 4),
 * (0, 3), (−1.5, 2), (3.6, 5), this maximum number is 3. Design an algorithm
 * for this problem with a better than quadratic time efﬁciency.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using Interval = std::pair<int, int>;


class NumberWithAnnotation{
public:
    int number;
    bool isStart;

    NumberWithAnnotation(int number, bool isStart) : number(number), isStart(isStart) {}

    bool operator<(const NumberWithAnnotation& other) const {
        // sort by number value first
        if(number < other.number) return true;
        if(number > other.number) return false;
        // break ties by putting starts first
        return isStart && !other.isStart;
    }
};


vector<NumberWithAnnotation> transformIntervalsToAnnotatedNumbers(const vector<Interval>& intervals){
    vector<NumberWithAnnotation> annotatedNumbers;

    for(const auto& interval : intervals){
        annotatedNumbers.emplace_back(interval.first, true);
        annotatedNumbers.emplace_back(interval.second, false);
    }

    return annotatedNumbers;
}

unsigned int solve(const vector<Interval>& intervals){
    // transform1: intervals to annotated numbers
    auto annotatedNumbers = transformIntervalsToAnnotatedNumbers(intervals);
    // transform2: sort annotated numbers
    std::sort(annotatedNumbers.begin(), annotatedNumbers.end());

    // auxiliary variables for the maximum finding conquer step
    unsigned int maxOverlap = 0;
    unsigned int currentOverlap = 0;

    for(const auto& annotatedNumber : annotatedNumbers){
        if(annotatedNumber.isStart){
            currentOverlap += 1;
            // if current is better than max, update max
            if(currentOverlap > maxOverlap){
                maxOverlap = currentOverlap;
            }
        } else {
            currentOverlap -= 1;
        }
    }

    return maxOverlap;
}

int main(){
    std::cout << "Maximum overlap: ";
    vector<Interval> intervals = {{1, 4}, {0, 3}, {-1.5, 2}, {3.6, 5}};
    std::cout << solve(intervals) << "\n";
    return 0;
}