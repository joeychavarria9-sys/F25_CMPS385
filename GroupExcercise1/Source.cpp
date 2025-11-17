#include <iostream>
#include <limits>
#include <vector>
#include <string>
using namespace std;

struct ClassInfo {
	string className;
	int numStudents;
	vector<double> grades;
	int average;
	int maxGrade;
	int minGrade;
};

void calculateStats(ClassInfo& cls) {
	double sum = 0.0;
	cls.maxGrade = numeric_limits<double>::lowest();
	cls.minGrade = numeric_limits<double>::max();

	for (int i = 0; i < cls.numStudents; ++i) {
		double grade;
		cout << "Enter grade for student " << i + 1 << ": ";
		cin >> grade;
		cls.grades.push_back(grade);
		sum += grade;
		if (grade > cls.maxGrade) cls.maxGrade = grade;
		if (grade < cls.minGrade) cls.minGrade = grade;
		}
		cls.average = sum / cls.numStudents;
}

void displayClassInfo(const ClassInfo& cls) {
	cout << "\nClass Name: " << cls.className << endl;
	cout << "Number of Students: " << cls.numStudents << endl;
	cout << "Average Grade: " << cls.average << endl;
	cout << "Maximum Grade: " << cls.maxGrade << endl;
	cout << "Minimum Grade: " << cls.minGrade << endl;
	cout << "-----------------------------\n";
}

int main() {
	int numClasses;
	cout << "Enter number of classes: ";
	cin >> numClasses;

	vector<ClassInfo> allClasses;

	for (int i = 0; i < numClasses; ++i) {
		ClassInfo cls;
		cout << "\nEnter class name: ";
		cin >> ws;
		getline(cin, cls.className);

		cout << "Enter number of students in class: ";
		cin >> cls.numStudents;

		calculateStats(cls);
		allClasses.push_back(cls);
	}

	for (const auto& cls : allClasses) {
		displayClassInfo(cls);
	}
	return 0;
}