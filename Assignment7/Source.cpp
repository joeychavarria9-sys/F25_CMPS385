#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct Order {
    string id;
    string name;
    string item;
    int quantity;
    float price;
    string status; // "Pending" or "Served"
};

// Function declarations
void loadOrders(vector<Order>& orders);
void saveOrders(const vector<Order>& orders);
void placeOrder(vector<Order>& orders);
void displayOrders(const vector<Order>& orders);
void markServed(vector<Order>& orders);
void salesSummary(const vector<Order>& orders);

int main() {
    vector<Order> orders;
    loadOrders(orders);
    int choice;
    do {
        cout << "\n****** Welcome to Restaurant Order Manager ******\n";
        cout << "1. Place New Order\n2. Display All Orders\n3. Mark Order as Served\n4. View Sales Summary\n5. Exit\n→ ";
        cin >> choice;
        switch (choice) {
        case 1: placeOrder(orders); break;
        case 2: displayOrders(orders); break;
        case 3: markServed(orders); break;
        case 4: salesSummary(orders); break;
        case 5:
            saveOrders(orders);
            cout << "Orders saved to orders.txt. Have a great day!" << endl;
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}

// Load orders from file
#include <sstream> // Add this to your includes

void loadOrders(vector<Order>& orders) {
    ifstream infile("orders.txt");
    if (!infile) return;

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        Order ord;
        string quantityStr, priceStr, totalStr;

        getline(ss, ord.id, ',');
        getline(ss, ord.name, ',');
        getline(ss, ord.item, ',');
        getline(ss, quantityStr, ',');
        getline(ss, priceStr, ',');
        getline(ss, totalStr, ',');
        getline(ss, ord.status);

        ord.quantity = stoi(quantityStr);
        ord.price = stof(priceStr);
        // totalStr is ignored since we recalculate total dynamically

        orders.push_back(ord);
    }
    infile.close();
}


// Save orders to file
void saveOrders(const vector<Order>& orders) {
    ofstream outfile("orders.txt");
    for (const auto& ord : orders) {
        float total = ord.quantity * ord.price;
        outfile << ord.id << "," << ord.name << "," << ord.item << ","
            << ord.quantity << "," << fixed << setprecision(2) << ord.price << ","
            << total << "," << ord.status << endl;
    }
    outfile.close();
}

// Place a new order
void placeOrder(vector<Order>& orders) {
    Order ord;
    cout << "Enter Order ID: ";
    cin >> ord.id;
    cin.ignore();
    cout << "Enter customer name: ";
    getline(cin, ord.name);
    cout << "Enter menu item: ";
    getline(cin, ord.item);
    cout << "Enter quantity: ";
    cin >> ord.quantity;
    cout << "Enter price per item: ";
    cin >> ord.price;
    ord.status = "Pending";
    orders.push_back(ord);
    cout << "Order added!" << endl;
}

// Display all orders
void displayOrders(const vector<Order>& orders) {
    cout << "\n=========== CURRENT ORDERS ===========" << endl;
    cout << left << setw(6) << "ID" << setw(10) << "Name" << setw(10) << "Item"
        << setw(5) << "Qty" << setw(8) << "Price" << setw(8) << "Total" << "Status" << endl;
    cout << "---------------------------------------------------" << endl;
    for (const auto& ord : orders) {
        float total = ord.quantity * ord.price;
        cout << left << setw(6) << ord.id << setw(10) << ord.name << setw(10) << ord.item
            << setw(5) << ord.quantity << "$" << setw(7) << fixed << setprecision(2) << ord.price
            << "$" << setw(7) << total << ord.status << endl;
    }
}

// Mark an order as served
void markServed(vector<Order>& orders) {
    string id;
    cout << "Enter Order ID to mark as served: ";
    cin >> id;
    bool found = false;
    for (auto& ord : orders) {
        if (ord.id == id && ord.status == "Pending") {
            ord.status = "Served";
            cout << "Order " << id << " marked as Served." << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "Order not found or already served." << endl;
}

// View sales summary
void salesSummary(const vector<Order>& orders) {
    int totalOrders = orders.size();
    int servedCount = 0;
    float totalSales = 0;
    for (const auto& ord : orders) {
        if (ord.status == "Served") {
            servedCount++;
            totalSales += ord.quantity * ord.price;
        }
    }
    cout << "\n========== SALES SUMMARY ==========" << endl;
    cout << "Total Orders: " << totalOrders << endl;
    cout << "Served: " << servedCount << endl;
    cout << "Pending: " << totalOrders - servedCount << endl;
    cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
    cout << "===================================" << endl;
}

