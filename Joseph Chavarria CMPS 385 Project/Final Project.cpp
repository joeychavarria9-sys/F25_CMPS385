#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>

using namespace std;

// ------------------------------
// Constants
// ------------------------------
const double DELIVERY_CHARGE = 5.99;
const double PRICE_DIPPING_CUP = 1.99;
const double PRICE_BREADBITES_16 = 6.99;
const double PRICE_LAVA_CAKES = 9.99;
const double PRICE_SALAD = 9.99;
const double PRICE_DRINK_20OZ = 2.99;
const double PRICE_DRINK_2L = 4.99;
const double PRICE_WINGS_8PC = 12.99;
const double PRICE_WINGS_16PC = 24.99;
const double PRICE_LOADED_TOTS = 9.99;
const double PRICE_BITES_32PC = 12.99;  // "32-piece bites"

// ------------------------------
// Utilities - Safe input helpers
// ------------------------------
int readInt(int minVal, int maxVal) {
    while (true) {
        int x;
        if (cin >> x) {
            if (x >= minVal && x <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            cout << "Please enter a number between " << minVal << " and " << maxVal << ".\n";
        }
        else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string readLineTrim() {
    string s;
    std::getline(cin, s);
    // trim leading/trailing whitespace
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

bool readYesNo() {
    while (true) {
        string s;
        std::getline(cin, s);
        if (s.empty()) continue;
        char c = s[0];
        if (c == 'y' || c == 'Y') return true;
        if (c == 'n' || c == 'N') return false;
        cout << "Please enter 'y' or 'n': ";
    }
}

string currentTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    // Format as YYYY-MM-DD HH:MM:SS
    std::tm tm;
#if defined(_MSC_VER)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return string(buf);
}

// ------------------------------
// Data Models
// ------------------------------
struct CartItem {
    string name;
    string details;
    double price;
    int quantity;
};

struct Order {
    int id = 0;
    string summary;
    string placedAt;
    string serviceMethod;
    vector<CartItem> items;
    string day;
    string time;
    double total = 0.0;
};

struct CustomerAccount {
    string name;
    int points = 0;
    vector<string> activeCoupons;
    vector<Order> orderHistory;
};

struct ServiceInfo {
    string method;   // Carryout, Delivery, Carside
    string address;  // For Delivery
    string carModel; // For Carside
    string carColor; // For Carside
};

struct TimedOrder {
    string day;   // e.g., "ASAP" or date
    string time;  // e.g., "6:30 PM" or empty
};

// ------------------------------
// Cart
// ------------------------------
class Cart {
private:
    vector<CartItem> items;
public:
    void addItem(const string& name, const string& details, double price, int quantity = 1) {
        if (quantity <= 0) quantity = 1;
        items.push_back({ name, details, price, quantity });
    }
    void showCart() const {
        cout << "\n=== Current Cart ===\n";
        if (items.empty()) {
            cout << "(empty)\n";
        }
        else {
            for (size_t i = 0; i < items.size(); ++i) {
                cout << (i + 1) << ") " << items[i].name;
                if (!items[i].details.empty()) cout << " (" << items[i].details << ")";
                cout << " x" << items[i].quantity << " : $"
                    << fixed << setprecision(2) << items[i].price * items[i].quantity << "\n";
            }
            cout << "--------------------\n";
            cout << "Subtotal: $" << fixed << setprecision(2) << calculateSubtotal() << "\n";
        }
    }
    void removeItem(int index) {
        if (index >= 1 && index <= (int)items.size()) {
            items.erase(items.begin() + (index - 1));
            cout << "Item removed.\n";
        }
        else {
            cout << "Invalid index.\n";
        }
    }
    void updateQuantity(int index, int newQty) {
        if (index >= 1 && index <= (int)items.size() && newQty > 0) {
            items[index - 1].quantity = newQty;
            cout << "Quantity updated.\n";
        }
        else {
            cout << "Invalid input.\n";
        }
    }
    void editDetails(int index, const string& newDetails) {
        if (index >= 1 && index <= (int)items.size()) {
            items[index - 1].details = newDetails;
            cout << "Details updated.\n";
        }
        else {
            cout << "Invalid index.\n";
        }
    }
    double calculateSubtotal() const {
        double subtotal = 0.0;
        for (const auto& it : items) subtotal += it.price * it.quantity;
        return subtotal;
    }
    double calculateTotal(const ServiceInfo& service) const {
        double subtotal = calculateSubtotal();
        double total = subtotal;
        if (service.method == "Delivery") {
            total += DELIVERY_CHARGE;
        }
        return total;
    }
    const vector<CartItem>& getItems() const { return items; }
    bool empty() const { return items.empty(); }
};

// ------------------------------
// Persistence: OrderMemory
// ------------------------------
class OrderMemory {
private:
    string filename;
public:
    OrderMemory(const string& file = "orders.txt") : filename(file) {}

    void saveOrder(const Order& order) {
        ofstream file(filename, ios::app);
        if (!file) {
            cerr << "Error: Could not open orders file for writing.\n";
            return;
        }
        file << "OrderID," << order.id << "\n";
        file << "PlacedAt," << order.placedAt << "\n";
        file << "ServiceMethod," << order.serviceMethod << "\n";
        file << "Day," << order.day << "\n";
        file << "Time," << order.time << "\n";
        file << "Total," << fixed << setprecision(2) << order.total << "\n";
        file << "Items:\n";
        for (const auto& item : order.items) {
            file << item.name << "," << item.details << "," << item.price << "," << item.quantity << "\n";
        }
        file << "----------------------\n";
    }

    void viewPastOrders() const {
        ifstream file(filename);
        if (!file) {
            cout << "No past orders found.\n";
            return;
        }
        cout << "\n=== Past Orders (file) ===\n";
        string line;
        while (getline(file, line)) {
            cout << line << "\n";
        }
        cout << "==========================\n";
    }
};

// ------------------------------
// Menus & Catalogs (cleaned & small)
// ------------------------------
class Menus {
public:
    static void showPizzaSizesAndCrusts() {
        cout << "\n--- Pizza Sizes and Crust Options ---\n";
        cout << "Small: Hand Tossed, Gluten-Free\n";
        cout << "Medium: Hand Tossed, Pan, Stuffed Crust, New York Style, Thin\n";
        cout << "Large: Hand Tossed, New York Style, Thin\n";
        cout << "Extra-Large: New York Style\n";
    }
    static void showSideItemsMenu() {
        cout << "\n=== Side Items ===\n";
        cout << "1) Bread Bites (16-piece) - $" << PRICE_BREADBITES_16 << "\n";
        cout << "2) Lava Cakes - $" << PRICE_LAVA_CAKES << "\n";
        cout << "3) Salad - $" << PRICE_SALAD << "\n";
        cout << "4) Wings (8-piece) - $" << PRICE_WINGS_8PC << "\n";
        cout << "5) Wings (16-piece) - $" << PRICE_WINGS_16PC << "\n";
        cout << "6) Loaded Tater Tots - $" << PRICE_LOADED_TOTS << "\n";
        cout << "7) Bread Bites (32-piece) - $" << PRICE_BITES_32PC << "\n";
        cout << "8) Back to previous\n";
    }
    static void showDrinksMenu() {
        cout << "\n--- Drinks Menu ---\n";
        cout << "Sizes:\n";
        cout << "  1) 20oz - $" << PRICE_DRINK_20OZ << "\n";
        cout << "  2) 2 Liter - $" << PRICE_DRINK_2L << "\n";
        cout << "Flavors: 1) Coke  2) Diet Coke  3) Sprite  4) Water\n";
    }
    static void showExtrasMenu() {
        cout << "\n--- Extras ---\n";
        cout << "1) Dipping Cup - $" << PRICE_DIPPING_CUP << "\n";
        cout << "2) Jalapenos - $1.10\n";
        cout << "3) Grated Parmesan - $2.99\n";
        cout << "4) Forks - Free\n";
        cout << "5) Back\n";
    }
};

// ------------------------------
// Pricing helper for pizzas (simple)
// ------------------------------
double getPizzaPrice(const string& size, const string& crust, bool specialty, int toppingCount) {
    double price = 0.0;
    if (!specialty) {
        if (size == "Small") price = 10.99;
        else if (size == "Medium") price = 13.99;
        else if (size == "Large") price = 16.99;
        else if (size == "Extra-Large") price = 19.99;
        else price = 13.99; // default
    }
    else {
        if (size == "Small") price = 17.99;
        else if (size == "Medium") price = 20.99;
        else if (size == "Large") price = 23.99;
        else if (size == "Extra-Large") price = 26.99;
        else price = 20.99;
    }
    // crust price overrides for certain crusts
    if (crust == "Pan") price = 22.99;
    else if (crust == "Stuffed Crust") price = 24.99;
    else if (crust == "Gluten-Free") price = 20.99;

    // add toppings
    price += toppingCount * 3.0;
    return price;
}

// ------------------------------
// Builders: pizza, side, drink, extras
// ------------------------------
string chooseCookingInstructions(const string& crust) {
    cout << "\n--- Cooking Instructions ---\n";

    // Always available
    vector<string> general = {
        "Lightly Done",
        "Well Done",
        "Pie Cut",
        "Square Cut",
        "Cut in 6 Slices",
        "Cut in 10 Slices",
        "Cut in 12 Slices",
        "Cut in 14 Slices",
        "Uncut",
        "Clean Cut"
    };

    // Crust-specific
    bool allowNoGarlic = (crust == "Hand Tossed" || crust == "Stuffed Crust");
    bool allowNoParmesan = (crust == "Stuffed Crust");
    bool allowNoOregano = (crust == "Thin");

    vector<string> selected;

    cout << "\nGENERAL OPTIONS:\n";
    for (size_t i = 0; i < general.size(); ++i)
        cout << "  " << (i + 1) << ") " << general[i] << "\n";

    cout << "Select general options (enter 0 when done):\n";
    while (true) {
        cout << "Choice (1-" << general.size() << ", or 0 to finish): ";
        int c = readInt(0, general.size());
        if (c == 0) break;
        selected.push_back(general[c - 1]);
    }

    // Crust-specific extras
    cout << "\nCRUST-SPECIFIC OPTIONS:\n";
    if (allowNoGarlic) cout << "  G) No Garlic Oil\n";
    if (allowNoParmesan) cout << "  P) No Parmesan\n";
    if (allowNoOregano) cout << "  O) No Oregano\n";

    cout << "Select crust-specific options (letters, ENTER to skip): ";
    string choices;
    getline(cin, choices);

    for (char c : choices) {
        c = tolower(c);
        if (c == 'g' && allowNoGarlic) selected.push_back("No Garlic Oil");
        if (c == 'p' && allowNoParmesan) selected.push_back("No Parmesan");
        if (c == 'o' && allowNoOregano) selected.push_back("No Oregano");
    }

    // Build final instruction string
    if (selected.empty()) return "No special instructions";

    string result;
    for (size_t i = 0; i < selected.size(); ++i) {
        if (i > 0) result += ", ";
        result += selected[i];
    }
    return result;
}
void buildPizza(Cart& cart) {
    cout << "\n--- Build Pizza ---\n";
    cout << "Available sizes: Small, Medium, Large, Extra-Large\n";
    cout << "Enter size: ";
    string size;
    size = readLineTrim();
    if (size.empty()) size = "Medium";

    cout << "Enter crust (Hand Tossed, Pan, Stuffed Crust, New York Style, Thin, Gluten-Free): ";
    string crust;
    crust = readLineTrim();
    if (crust.empty()) crust = "Hand Tossed";

    cout << "Is this a specialty pizza? (y/n): ";
    bool specialty = readYesNo();

    cout << "How many toppings (0-10): ";
    int toppingCount = readInt(0, 10);

    cout << "Enter a short topping description (or press ENTER): ";
    string toppingDesc = readLineTrim();

    // Ask user for cooking instructions
    string cookingInstructions = chooseCookingInstructions(crust);

    double price = getPizzaPrice(size, crust, specialty, toppingCount);
    ostringstream details;
    if (!toppingDesc.empty()) details << toppingDesc;
    if (specialty) {
        if (!details.str().empty()) details << " | ";
        details << "Specialty";
    }
    if (!details.str().empty()) details << " | ";
    details << cookingInstructions;
    cart.addItem(size + " Pizza", details.str(), price, 1);
    cout << "Pizza added to cart: " << size << " (" << crust << ") - $" << fixed << setprecision(2) << price << "\n";
}

// ------------------------------
// Side Items Builder (updated with wings, tots, bites)
// ------------------------------
void buildSideItem(Cart& cart) {
    while (true) {
        Menus::showSideItemsMenu();
        cout << "Select a side item: ";
        int choice = readInt(1, 8);

        if (choice == 8) return; // Back

        cout << "Enter quantity (1-20): ";
        int qty = readInt(1, 20);

        switch (choice) {
        case 1:
            cart.addItem("Bread Bites (16-piece)", "", PRICE_BREADBITES_16, qty);
            break;
        case 2:
            cart.addItem("Lava Cakes", "", PRICE_LAVA_CAKES, qty);
            break;
        case 3:
            cart.addItem("Salad", "", PRICE_SALAD, qty);
            break;
        case 4:
            cart.addItem("Wings (8-piece)", "", PRICE_WINGS_8PC, qty);
            break;
        case 5:
            cart.addItem("Wings (16-piece)", "", PRICE_WINGS_16PC, qty);
            break;
        case 6:
            cart.addItem("Loaded Tater Tots", "", PRICE_LOADED_TOTS, qty);
            break;
        case 7:
            cart.addItem("Bread Bites (32-piece)", "", PRICE_BITES_32PC, qty);
            break;
        }

        cout << "Item added!\n";
    }
}
// ------------------------------
// Drinks Builder
// ------------------------------
void buildDrink(Cart& cart) {
    while (true) {
        Menus::showDrinksMenu();
        cout << "Choose size (1 = 20oz, 2 = 2L, 3 = back): ";
        int sizeChoice = readInt(1, 3);

        if (sizeChoice == 3) return;

        double basePrice = (sizeChoice == 1) ? PRICE_DRINK_20OZ : PRICE_DRINK_2L;

        cout << "Choose flavor:\n1) Coke  2) Diet Coke  3) Sprite  4) Water  (5 = Back)\n";
        int flavorChoice = readInt(1, 5);

        if (flavorChoice == 5) continue;

        string flavor;
        if (flavorChoice == 1) flavor = "Coke";
        else if (flavorChoice == 2) flavor = "Diet Coke";
        else if (flavorChoice == 3) flavor = "Sprite";
        else flavor = "Water";

        cout << "Enter quantity: ";
        int qty = readInt(1, 20);

        string name = (sizeChoice == 1 ? "20oz " : "2L ") + flavor;
        cart.addItem(name, "", basePrice, qty);
        cout << "Drink added!\n";
    }
}

// ------------------------------
// Extras Builder
// ------------------------------
void buildExtras(Cart& cart) {
    while (true) {
        Menus::showExtrasMenu();
        cout << "Choose an extra: ";
        int c = readInt(1, 5);

        if (c == 5) return;

        cout << "Enter quantity: ";
        int qty = readInt(1, 20);

        switch (c) {
        case 1:
            cart.addItem("Dipping Cup", "", PRICE_DIPPING_CUP, qty);
            break;
        case 2:
            cart.addItem("Jalapenos", "", 1.10, qty);
            break;
        case 3:
            cart.addItem("Grated Parmesan", "", 2.99, qty);
            break;
        case 4:
            cart.addItem("Fork", "", 0.00, qty);
            break;
        }
        cout << "Added!\n";
    }
}

// ------------------------------
// Cart Editing
// ------------------------------
void editCart(Cart& cart) {
    while (true) {
        cart.showCart();
        if (cart.empty()) {
            cout << "Nothing to edit.\n";
            return;
        }

        cout << "Edit Menu:\n";
        cout << "1) Remove item\n";
        cout << "2) Update quantity\n";
        cout << "3) Update details\n";
        cout << "4) Back\n";
        cout << "Choice: ";

        int c = readInt(1, 4);
        if (c == 4) return;

        cout << "Enter item number: ";
        int index = readInt(1, (int)cart.getItems().size());

        if (c == 1) {
            cart.removeItem(index);
        }
        else if (c == 2) {
            cout << "New quantity: ";
            int q = readInt(1, 20);
            cart.updateQuantity(index, q);
        }
        else if (c == 3) {
            cout << "Enter new details: ";
            string d = readLineTrim();
            cart.editDetails(index, d);
        }
    }
}

// ------------------------------
// Service Method Selection
// ------------------------------
ServiceInfo chooseServiceMethod() {
    ServiceInfo info;
    cout << "\nChoose service method:\n";
    cout << "1) Carryout\n";
    cout << "2) Delivery\n";
    cout << "3) Carside\n";
    int c = readInt(1, 3);

    if (c == 1) {
        info.method = "Carryout";
    }
    else if (c == 2) {
        info.method = "Delivery";
        cout << "Enter delivery address: ";
        info.address = readLineTrim();
    }
    else {
        info.method = "Carside";
        cout << "Enter car model: ";
        info.carModel = readLineTrim();
        cout << "Enter car color: ";
        info.carColor = readLineTrim();
    }
    return info;
}

// ------------------------------
// Timed Order Selection
// ------------------------------
TimedOrder chooseTimedOrder() {
    TimedOrder t;
    cout << "\nChoose pickup/delivery time:\n";
    cout << "1) ASAP\n";
    cout << "2) Schedule for later\n";
    int c = readInt(1, 2);

    if (c == 1) {
        t.day = "ASAP";
        t.time = "";
    }
    else {
        cout << "Enter day (e.g., Friday): ";
        t.day = readLineTrim();
        cout << "Enter time (e.g., 6:30 PM): ";
        t.time = readLineTrim();
    }
    return t;
}

// ------------------------------
// Place Order
// ------------------------------
void placeOrder(Cart& cart, CustomerAccount& acct, OrderMemory& memory, const ServiceInfo& service, const TimedOrder& timed) {
    if (cart.empty()) {
        cout << "Cart is empty.\n";
        return;
    }

    double total = cart.calculateTotal(service);

    cout << "\n=== Order Summary ===\n";
    cart.showCart();
    if (service.method == "Delivery") {
        cout << "Delivery Charge: $" << DELIVERY_CHARGE << "\n";
    }
    cout << "TOTAL: $" << fixed << setprecision(2) << total << "\n";

    cout << "Place order? (y/n): ";
    if (!readYesNo()) return;

    Order o;
    o.id = rand() % 1000000;
    o.items = cart.getItems();
    o.placedAt = currentTimestamp();
    o.serviceMethod = service.method;
    o.total = total;
    o.day = timed.day;
    o.time = timed.time;

    o.summary = "Order " + to_string(o.id) + " placed via " + service.method;

    acct.orderHistory.push_back(o);
    memory.saveOrder(o);

    acct.points += 10; // reward system placeholder

    cout << "Order placed! Order ID: " << o.id << "\n";
    cout << "You earned 10 points!\n";

    cart = Cart(); // reset cart
}
void showActiveCoupons() {
    cout << "\n=== Active Coupons ===\n";
    cout << "Disclaimer: Only one coupon can be used per order\n";
    cout << "1) 2 or more 3-Topping Pizzas — $10.99 each\n";
    cout << "2) 2 or more Large 2-Toppings — $13.99 each\n";
    cout << "3) 1 Medium 3 Topping Pizza for $11.99\n";
    cout << "4) 1 Large 3 Topping Pizza for $14.99\n";
    cout << "5) Mix and Match Deal\n";
    cout << "6) Weeklong Carryout\n";
    cout << "7) 20% Off Entire Order\n";
    cout << "======================\n";
}
// ------------------------------
// Main Menu Flow
// ------------------------------
void showMainMenu() {
    cout << "\n========== Domino's Console ==========\n";
    cout << "1) Build Pizza\n";
    cout << "2) Sides\n";
    cout << "3) Drinks\n";
    cout << "4) Extras\n";
    cout << "5) View Cart\n";
    cout << "6) Edit Cart\n";
    cout << "7) Choose Service Method\n";
    cout << "8) Choose Pickup/Delivery Time\n";
    cout << "9) Place Order\n";
    cout << "10) View Past Orders\n";
    cout << "11) View Active Coupons\n";   
    cout << "12) Exit\n";                 
    cout << "Choose: ";
}

// ------------------------------
// MAIN
// ------------------------------
int main() {
    srand((unsigned)time(nullptr));

    Cart cart;
    CustomerAccount account;
    account.name = "Guest";

    OrderMemory memory;

    ServiceInfo service;
    service.method = "Carryout";

    TimedOrder timed;
    timed.day = "ASAP";

    while (true) {
        showMainMenu();
        int c = readInt(1, 12);

        switch (c) {
        case 1:
            buildPizza(cart);
            break;
        case 2:
            buildSideItem(cart);
            break;
        case 3:
            buildDrink(cart);
            break;
        case 4:
            buildExtras(cart);
            break;
        case 5:
            cart.showCart();
            break;
        case 6:
            editCart(cart);
            break;
        case 7:
            service = chooseServiceMethod();
            break;
        case 8:
            timed = chooseTimedOrder();
            break;
        case 9:
            placeOrder(cart, account, memory, service, timed);
            break;
        case 10:
            memory.viewPastOrders();
            break;
        case 11:
            showActiveCoupons();
            break;
        case 12:
            cout << "Goodbye!\n";
            return 0;
        }
    }
}