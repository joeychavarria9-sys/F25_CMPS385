#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

struct MenuItem {
    int id;
    string name;
    float price;
};

struct CartItem {
    MenuItem item;
    int quantity;
};

void displayMenu(const vector<MenuItem>& menu) {
    cout << "\n=========== PIZZA MENU ===========" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Item" << "Price" << endl;
    cout << "----------------------------------" << endl;
    for (const auto& m : menu) {
        cout << left << setw(5) << m.id << setw(20) << m.name << "$" << fixed << setprecision(2) << m.price << endl;
    }
}

void addToCart(vector<CartItem>& cart, const vector<MenuItem>& menu) {
    int id, qty;
    cout << "Enter item ID to add: ";
    cin >> id;
    cout << "Enter quantity: ";
    cin >> qty;

    for (const auto& m : menu) {
        if (m.id == id) {
            cart.push_back({ m, qty });
            cout << qty << " x " << m.name << " added to cart.\n";
            return;
        }
    }
    cout << "Invalid item ID.\n";
}

void viewCart(const vector<CartItem>& cart) {
    cout << "\n=========== YOUR CART ===========" << endl;
    float total = 0;
    for (const auto& c : cart) {
        float itemTotal = c.item.price * c.quantity;
        cout << c.quantity << " x " << c.item.name << " @ $" << fixed << setprecision(2)
            << c.item.price << " = $" << itemTotal << endl;
        total += itemTotal;
    }
    cout << "----------------------------------" << endl;
    cout << "Total: $" << fixed << setprecision(2) << total << endl;
}

void checkout(const vector<CartItem>& cart) {
    ofstream receipt("receipt.txt");
    float total = 0;
    receipt << "=========== RECEIPT ===========" << endl;
    for (const auto& c : cart) {
        float itemTotal = c.item.price * c.quantity;
        receipt << c.quantity << " x " << c.item.name << " @ $" << fixed << setprecision(2)
            << c.item.price << " = $" << itemTotal << endl;
        total += itemTotal;
    }
    receipt << "----------------------------------" << endl;
    receipt << "Total: $" << fixed << setprecision(2) << total << endl;
    receipt.close();
    cout << "Order placed! Receipt saved to receipt.txt\n";
}

int main() {
    vector<MenuItem> menu = {
        {1, "Pepperoni Pizza", 12.99},
        {2, "Cheese Pizza", 10.99},
        {3, "Veggie Pizza", 11.49},
        {4, "Wings (6 pcs)", 6.99},
        {5, "Garlic Bread", 4.99},
        {6, "Soda", 1.99}
    };

    vector<CartItem> cart;
    int choice;

    do {
        cout << "\n****** Welcome to Pizza Order System ******\n";
        cout << "1. Browse Menu\n2. Add to Cart\n3. View Cart\n4. Checkout\n5. Exit\n→ ";
        cin >> choice;

        switch (choice) {
        case 1: displayMenu(menu); break;
        case 2: addToCart(cart, menu); break;
        case 3: viewCart(cart); break;
        case 4: checkout(cart); break;
        case 5: cout << "Thanks for using the Pizza Order System!\n"; break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
