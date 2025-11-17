#include <iostream>
using namespace std;

const int MAX_SIZE = 100;
int grades[MAX_SIZE];
int gradeCount = 0;

// Recursive function to display grades
void displayGrades(int grades[], int size, int index = 0) {
    if (index == size) {
        cout << endl;
        return;
    }
    cout << grades[index] << " ";
    displayGrades(grades, size, index + 1);
}

// Recursive function to sum grades
int sumGrades(int grades[], int size) {
    if (size == 0) return 0;
    return grades[size - 1] + sumGrades(grades, size - 1);
}

// Recursive function to count failing grades (< 60)
int countFailing(int grades[], int size) {
    if (size == 0) return 0;
    int count = (grades[size - 1] < 60) ? 1 : 0;
    return count + countFailing(grades, size - 1);
}

// Recursive function to find the highest grade
int findMax(int grades[], int size, int index = 0, int currentMax = -1) {
    if (index == size) return currentMax;
    if (grades[index] > currentMax) currentMax = grades[index];
    return findMax(grades, size, index + 1, currentMax);
}

int main() {
    int choice;

    cout << "****** Welcome to Recursive Grade Analyzer ******" << endl;

    do {
        cout << "\nPlease choose one of the following operations:\n";
        cout << "1. Enter Grades\n";
        cout << "2. Display Grades\n";
        cout << "3. Calculate Average Grade (using Recursion)\n";
        cout << "4. Count Failing Grades (using Recursion)\n";
        cout << "5. Find Highest Grade (using Recursion)\n";
        cout << "6. Exit\n";
        cout << "→ ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "How many grades do you want to enter?\n→ ";
            cin >> gradeCount;
            if (gradeCount > MAX_SIZE) {
                cout << "Maximum allowed is " << MAX_SIZE << ". Try again.\n";
                break;
            }
            for (int i = 0; i < gradeCount; ++i) {
                cout << "Enter grade #" << i + 1 << ": ";
                cin >> grades[i];
            }
            cout << "Grades successfully saved!" << endl;
            break;
        }
        case 2:
            cout << "Grades entered:\n→ ";
            displayGrades(grades, gradeCount);
            break;
        case 3: {
            if (gradeCount == 0) {
                cout << "No grades entered yet.\n";
                break;
            }
            double avg = static_cast<double>(sumGrades(grades, gradeCount)) / gradeCount;
            cout << "The average grade is: " << avg << endl;
            break;
        }
        case 4:
            cout << "Number of failing grades: " << countFailing(grades, gradeCount) << endl;
            break;
        case 5:
            cout << "Highest grade: " << findMax(grades, gradeCount) << endl;
            break;
        case 6:
            cout << "Thank you for using Recursive Grade Analyzer!\n";
            cout << "CMPS 385: Data Structures Fall 2025\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}
