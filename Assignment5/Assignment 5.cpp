#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX_ITEMS = 100;

struct Item {
    string name;
    string category;
    int quantity;
    double price;
};

struct Transaction {
    string type; // "BUY" or "RETURN"
    string itemName;
    int quantity;
    double unitPrice;
};

int loadInventory(Item items[]) {
    ifstream fin("inventory.txt");
    int count = 0;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        items[count].name = line.substr(0, pos1);
        items[count].category = line.substr(pos1 + 1, pos2 - pos1 - 1);
        items[count].quantity = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        items[count].price = stod(line.substr(pos3 + 1));

        count++;
    }
    fin.close();
    return count;
}

void saveInventory(Item items[], int count) {
    ofstream fout("inventory.txt");
    for (int i = 0; i < count; i++) {
        fout << items[i].name << "," << items[i].category << ","
            << items[i].quantity << "," << items[i].price << endl;
    }
    fout.close();
}

void displayInventory(Item items[], int count) {
    cout << "\n============== INVENTORY ==============\n";
    cout << left << setw(15) << "Item Name"
        << setw(15) << "Category"
        << setw(5) << "Qty"
        << setw(10) << "Price" << endl;
    cout << "---------------------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << left << setw(15) << items[i].name
            << setw(15) << items[i].category
            << setw(5) << items[i].quantity
            << "$" << fixed << setprecision(2) << items[i].price << endl;
    }
}

void showProfitReport() {
    ifstream fin("transactions.txt");
    string line;
    double revenue = 0, refunds = 0;

    while (getline(fin, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        string type = line.substr(0, pos1);
        int qty = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        double price = stod(line.substr(pos3 + 1));
        if (type == "BUY") revenue += qty * price;
        else if (type == "RETURN") refunds += qty * price;
    }
    fin.close();

    cout << "========== PROFIT REPORT ==========\n";
    cout << "Total Revenue: $" << fixed << setprecision(2) << revenue << endl;
    cout << "Total Refunds: $" << refunds << endl;
    cout << "Net Profit: $" << revenue - refunds << endl;
    cout << "==================================\n";
}

void buyItem(Item items[], int count) {
    string name;
    int qty;
    cout << "Enter item to buy: ";
    cin.ignore();
    getline(cin, name);
    cout << "Quantity: ";
    cin >> qty;

    for (int i = 0; i < count; i++) {
        if (items[i].name == name && items[i].quantity >= qty) {
            items[i].quantity -= qty;
            double total = qty * items[i].price;
            cout << "Successfully purchased " << qty << " x " << name << " for $" << fixed << setprecision(2) << total << endl;
            saveTransaction({ "BUY", name, qty, items[i].price });
            return;
        }
    }
    cout << "Item not found or insufficient quantity.\n";
}

void returnItem(Item items[], int count) {
    string name;
    int qty;
    cout << "Enter item to return: ";
    cin.ignore();
    getline(cin, name);
    cout << "Quantity: ";
    cin >> qty;

    for (int i = 0; i < count; i++) {
        if (items[i].name == name) {
            items[i].quantity += qty;
            cout << qty << " x " << name << " returned and added back to inventory.\n";
            saveTransaction({ "RETURN", name, qty, items[i].price });
            return;
        }
    }
    cout << "Item not found.\n";
}

void saveTransaction(Transaction t) {
    ofstream fout("transactions.txt", ios::app);
    fout << t.type << "," << t.itemName << "," << t.quantity << "," << t.unitPrice << endl;
    fout.close();
}

void showProfitReport() {
    ifstream fin("transactions.txt");
    string line;
    double revenue = 0, refunds = 0;

    while (getline(fin, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        string type = line.substr(0, pos1);
        int qty = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        double price = stod(line.substr(pos3 + 1));
        if (type == "BUY") revenue += qty * price;
        else if (type == "RETURN") refunds += qty * price;
    }
    fin.close();

    cout << "========== PROFIT REPORT ==========\n";
    cout << "Total Revenue: $" << fixed << setprecision(2) << revenue << endl;
    cout << "Total Refunds: $" << refunds << endl;
    cout << "Net Profit: $" << revenue - refunds << endl;
    cout << "==================================\n";
}

int main() {
    Item items[MAX_ITEMS];
    int itemCount = loadInventory(items);
    int choice;

    cout << "******** Welcome to Store Inventory & Profit Tracker ********\n";

    do {
        cout << "\nPlease choose one of the following operations:\n"
            << "1. Add New Item to Inventory\n"
            << "2. Display Inventory\n"
            << "3. Buy Item\n"
            << "4. Return Item\n"
            << "5. View Profit Report\n"
            << "6. Exit\n→ ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            if (itemCount >= MAX_ITEMS) {
                cout << "Inventory full!\n";
                break;
            }
            cout << "Enter item name: ";
            getline(cin, items[itemCount].name);
            cout << "Enter category: ";
            getline(cin, items[itemCount].category);
            cout << "Enter quantity: ";
            cin >> items[itemCount].quantity;
            cout << "Enter price: ";
            cin >> items[itemCount].price;
            itemCount++;
            cout << "Item added!\n";
            break;
        }
        case 2:
            displayInventory(items, itemCount);
            break;
        case 3:
            buyItem(items, itemCount);
            break;
        case 4:
            returnItem(items, itemCount);
            break;
        case 5:
            showProfitReport();
            break;
        case 6:
            saveInventory(items, itemCount);
            cout << "Inventory and transactions saved. Goodbye!\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
