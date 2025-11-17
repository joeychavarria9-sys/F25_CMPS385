#include <iostream>
#include <string>
using namespace std;

class Item {
private:
	string name;
	double price;

protected:
	Item(string n, double p) : name(n), price(p) {}

public:
	virtual void display() const = 0;
	virtual double getPrice() const = 0;
	virtual void receiptEntry() const = 0;
	string getName() const { return name; }
};
class FoodItem : public Item {
private:
	string expirationDate;
public:
	FoodItem(string n, double p, string exp) : Item(n, p), expirationDate(exp) {}

	void display() const override {
		cout << "Name: " << getName() << " | Price: $" << getPrice() << " | Exp: " << expirationDate << endl;
	}
	double getPrice() const override {
		return Item::getPrice();
	}
	void receiptEntry() const override {
		cout << getName() << " - $" << getPrice() << endl;
	}
};

class ElectronicItem : public Item {
private:
	int warrantyMonths;
public:
	ElectronicItem(string n, double p, int wm) : Item(n, p), warrantyMonths(wm) {}

	void display() const override {
		cout << "Name: " << getName() << " | Price: $" << getPrice() << " | Warranty: " << warrantyMonths << " months" << endl;
	}
	double getPrice() const override {
		return Item::getPrice();
	}
	void receiptEntry() const override {
		cout << getName() << " - $" << getPrice() << endl;
	}
};

int main() {
    const int MAX_ITEMS = 50;
    const int MAX_PURCHASES = 20;
    Item* store[MAX_ITEMS];
    Item* purchases[MAX_PURCHASES];
    int storeCount = 0, purchaseCount = 0;
    int choice;

    cout << "************* Welcome to QuickMart POS *************\n";
    do {
        cout << "1 - Add item to store\n";
        cout << "2 - Display available items\n";
        cout << "3 - Buy item by name\n";
        cout << "4 - View receipt\n";
        cout << "0 - Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string type, name;
            double price;

            cout << "Enter item type (Food/Electronic): ";
            getline(cin, type);
            cout << "Enter item name: ";
            getline(cin, name);
            cout << "Enter price: ";
            cin >> price;
            cin.ignore();

            if (type == "Food") {
                string exp;
                cout << "Enter expiration date: ";
                getline(cin, exp);
                store[storeCount++] = new FoodItem(name, price, exp);
            }
            else if (type == "Electronic") {
                int warranty;
                cout << "Enter warranty in months: ";
                cin >> warranty;
                cin.ignore();
                store[storeCount++] = new ElectronicItem(name, price, warranty);
            }
            cout << "Item added successfully!\n";

        }
        else if (choice == 2) {
            for (int i = 0; i < storeCount; ++i) {
                store[i]->display();
            }

        }
        else if (choice == 3) {
            string searchName;
            cout << "Enter item name: ";
            getline(cin, searchName);
            bool found = false;

            for (int i = 0; i < storeCount; ++i) {
                if (store[i]->getName() == searchName) {
                    purchases[purchaseCount++] = store[i];
                    cout << "Purchased " << store[i]->getName()
                        << " for $" << store[i]->getPrice() << endl;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Item not found.\n";
            }

        }
        else if (choice == 4) {
            double total = 0;
            cout << "---- Receipt ----\n";
            for (int i = 0; i < purchaseCount; ++i) {
                cout << i + 1 << ". ";
                purchases[i]->receiptEntry();
                total += purchases[i]->getPrice();
            }
            cout << "Total: $" << total << endl;
        }

    } while (choice != 0);

    cout << "Thank you for shopping at QuickMart!\n";

    for (int i = 0; i < storeCount; ++i) {
        delete store[i];
    }
    return 0;
}