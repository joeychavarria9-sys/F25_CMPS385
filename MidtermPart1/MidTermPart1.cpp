#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 100;

struct Box {
	int id;
	string item;
	float weight;
	string destination;
};

class Stack {
private:
	Box boxes[MAX];
	int top;

public:
	Stack() { top = -1; }
	bool isFull() { return top == MAX - 1; }
	bool isEmpty() { return top == -1; }

	void push(Box b) {
		if (isFull()) {
			cout << "Stack is full. Cannot add more boxes.\n";
			return;
		}
		boxes[++top] = b;
		cout << "Box added to stack!\n";
	}

	void pop() {
		if (isEmpty()) {
			cout << "Stack is empty. No boxes to dispatch.\n";
			return;
		}
		Box b = boxes[top--];
		cout << "Top box dispatched: \n";
		cout << "ID: " << b.id << ", Item: " << b.item << ", Destination: " << b.destination << endl;
	}

	void peek() {
		if (!isEmpty()) {
			Box b = boxes[top];
			cout << "Top Box: ID " << b.id << " - " << b.item << endl;
		}
	}

	void viewStack() {
		if (isEmpty()) {
			cout << "Stack is empty.\n";
			return;
		}
		cout << "========= STACKED BOXES (Top to Bottom) =========\n";
		cout << left << setw(8) << "Box ID" << setw(12) << "Item" << setw(12) << "Weight(kg)" << "Destination\n";
		cout << "-----------------------------------------------\n";
		for (int i = top; i >= 0; i--) {
			cout << left << setw(8) << boxes[i].id << setw(12) << boxes[i].item
				<< setw(12) << boxes[i].weight << boxes[i].destination << endl;
		}
	}

	void inventorySummary() {
		float totalWeight = 0;
		for (int i = 0; i <= top; i++) {
			totalWeight += boxes[i].weight;
		}
		cout << "========== INVENTORY SUMMARY ==========\n";
		cout << "Total Boxes in Stack: " << top + 1 << endl;
		cout << "Total Weight: " << fixed << setprecision(1) << totalWeight << " kg\n";
		peek();
		cout << "=======================================\n";
	}
	void saveToFile() {
		ofstream out("inventory.txt");
		for (int i = top; i >= 0; i--) {
			out << boxes[i].id << "," << boxes[i].item << "," << boxes[i].weight << "," << boxes[i].destination << endl;
		}
		out.close();
		cout << "Inventory saved to inventory.txt. Have a productive day!\n";
	}
};

int main() {
	Stack warehouse;
	int choice;

	cout << "****** Welcome to Warehouse Inventory Stack Tracker ******\n";
	do {
		cout << "\nPlease choose one of the following operations:\n";
		cout << "1. Add Box to Stack\n";
		cout << "2. View Current Stack\n";
		cout << "3. Remove Top Box (Dispatch)\n";
		cout << "4. View Inventory Summary\n";
		cout << "5. Exit\n";
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			Box b;
			cout << "Enter Box ID: ";
			cin >> b.id;
			cin.ignore();
			cout << "Enter Item Name: ";
			getline(cin, b.item);
			cout << "Enter Weight (kg): ";
			cin >> b.weight;
			cin.ignore();
			cout << "Enter Destination: ";
			getline(cin, b.destination);
			warehouse.push(b);
		}
		else if (choice == 2) {
			warehouse.viewStack();
		}
		else if (choice == 3) {
			warehouse.pop();
		}
		else if (choice == 4) {
			warehouse.inventorySummary();
		}
		else if (choice == 5) {
			warehouse.saveToFile();
		}
		else {
			cout << "Invalid choice. Try again.\n";
		}
	} while (choice != 5);

		return 0;
	}