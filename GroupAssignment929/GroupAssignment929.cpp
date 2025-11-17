#include <iostream>
using namespace std;

// Recursive function to calculate summation of (4n + 5)
int recursiveSum(int start, int end) {
    if (start > end)
        return 0;
    return (4 * start + 5) + recursiveSum(start + 1, end);
}

int main() {
    int startIndex, endIndex;

    // Input from user
    cout << "Enter the start index: ";
    cin >> startIndex;

    cout << "Enter the end index: ";
    cin >> endIndex;

    cout << "Now calculating (4n + 5) from " << startIndex << " to " << endIndex << "...\n";

    // Calculate and display result
    int result = recursiveSum(startIndex, endIndex);
    cout << "Final answer is: " << result << endl;

    return 0;
}
