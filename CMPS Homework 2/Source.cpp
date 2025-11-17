#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Book {
private:
	int id;
	string title;
	string author;
	int year;
	bool available;
public:
	//Default constructor
	Book() {
		id = 0;
		title = "";
		author = "";
		year = 0;
		available = true;
	}

	//Parameterized constructor
	Book(int _id, string _title, string _author, int _year) {
		id = _id;
		title = _title;
		author = _author;
		year = _year;
		available = true;
	}

	//Getters
	int getID() const { return id; }
	string getTitle() const { return title; }
	bool isAvailable() const { return available; }

	//Setters
    void setID(int _id) { id = _id; }
    void setTitle(string _title) { title = _title; }
    void setAuthor(string _author) { author = _author; }
	void setYear(int _year) { year = _year; }
	void setAvailable(bool _available) { available = _available; }

	//Borrow book
	void borrowBook() {
		if (available) {
			available = false;
			cout << "You have successfully borrowed '" << title << "'.\n";
		}
		else {
			cout << "Sorry, '" << title << "' is currently not available.\n";
		}
	}
	void returnBook() {
		if (!available) {
			available = true;
			cout << "You have successfully returned '" << title << "'.\n";
		}
		else {
			cout << "Book was not borrowed.\n";
		}
	}
	void display() const {
        cout << "ID: " << id << "\n";
        cout << "Title: " << title << "\n";
        cout << "Author: " << author << "\n";
        cout << "Year: " << year << "\n";
		cout << "Status: " << (available ? "Available" : "Borrowed") << "\n";
	}
};

Book library[100];
int bookCount = 0;

int generateBookID() {
	return rand() % 90000 + 10000;
}
int findBookByID(int id) {
	for (int i = 0; i < bookCount; ++i) {
		if (library[i].getID() == id) {
			return i;
		}
	}
	return -1;
}
// Main menu
void showMenu() {
    cout << "\n*************** Welcome to the Library ***************\n";
    cout << "Please choose one of the following operations:\n";
    cout << "1 - Add a new book\n";
    cout << "2 - Borrow a book\n";
    cout << "3 - Return a book\n";
    cout << "4 - Display details of a book (by ID)\n";
    cout << "5 - List all available books\n";
    cout << "0 - Exit\n";
    cout << "Choice: ";
}
int main() {
    srand(time(0)); // Seed for random ID generation
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(); // Clear newline from input buffer

        if (choice == 1) {
            string title, author;
            int year;
            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter year: ";
            cin >> year;

            int id = generateBookID();
            library[bookCount++] = Book(id, title, author, year);

            cout << "Book added successfully!\n";
            cout << "Title: " << title << "\n";
            cout << "ID: " << id << "\n";

        }
        else if (choice == 2) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            int index = findBookByID(id);
            if (index != -1) {
                library[index].borrowBook();
            }
            else {
                cout << "Book not found.\n";
            }

        }
        else if (choice == 3) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            int index = findBookByID(id);
            if (index != -1) {
                library[index].returnBook();
            }
            else {
                cout << "Book not found.\n";
            }

        }
        else if (choice == 4) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            int index = findBookByID(id);
            if (index != -1) {
                library[index].display();
            }
            else {
                cout << "Book not found.\n";
            }

        }
        else if (choice == 5) {
            bool found = false;
            cout << "Available Books:\n";
            for (int i = 0; i < bookCount; ++i) {
                if (library[i].isAvailable()) {
                    cout << "ID: " << library[i].getID()
                        << " | Title: " << library[i].getTitle()
                        << " | Author: " << library[i].getTitle()
                        << " | Year: " << library[i].getTitle() << "\n";
                    found = true;
                }
            }
            if (!found) {
                cout << "No books available at the moment.\n";
            }

        }
        else if (choice == 0) {
            cout << "Thank you for using the Library System! Goodbye!\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

        if (choice != 0) {
            cout << "Press Enter to return to Main Menu...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}