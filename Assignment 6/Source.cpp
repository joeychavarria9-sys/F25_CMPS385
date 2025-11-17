// Load packages from file
template <typename T1, typename T2>
int loadPackages(Package<T1, T2> packages[]) {
    ifstream inFile("packages.txt");
    int count = 0;
    if (!inFile) return 0;

    string line;
    getline(inFile, line); // Skip header
    while (getline(inFile, line) && count < 100) {
        stringstream ss(line);
        string idStr, name, type, etaStr, feeStr, status;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, type, ',');
        getline(ss, etaStr, ',');
        getline(ss, feeStr, ',');
        getline(ss, status, ',');

        stringstream idStream(idStr);
        T1 id;
        idStream >> id;

        stringstream feeStream(feeStr);
        T2 fee;
        feeStream >> fee;

        packages[count].id = id;
        packages[count].name = name;
        packages[count].type = type;
        packages[count].etaHours = stoi(etaStr);
        packages[count].fee = fee;
        packages[count].status = status;
        count++;
    }
    inFile.close();
    return count;
}

// Save packages to file
template <typename T1, typename T2>
void savePackages(Package<T1, T2> packages[], int count) {
    ofstream outFile("packages.txt");
    outFile << "ID,Name,Type,ETA,Fee,Status\n";
    for (int i = 0; i < count; ++i) {
        outFile << packages[i].id << "," << packages[i].name << "," << packages[i].type << ","
            << packages[i].etaHours << "," << fixed << setprecision(2) << packages[i].fee << ","
            << packages[i].status << "\n";
    }
    outFile.close();
}

// Add new package
template <typename T1, typename T2>
void addPackage(Package<T1, T2> packages[], int& count) {
    if (count >= 100) {
        cout << "Package limit reached.\n";
        return;
    }
    Package<T1, T2> p;
    cout << "Enter package ID: ";
    cin >> p.id;
    cin.ignore();
    cout << "Enter recipient name: ";
    getline(cin, p.name);
    cout << "Enter delivery type (Standard/Express/Fragile): ";
    getline(cin, p.type);
    cout << "Enter delivery ETA (in hours): ";
    cin >> p.etaHours;
    cout << "Enter delivery fee: ";
    cin >> p.fee;
    p.status = "Pending";
    packages[count++] = p;
    cout << "Package added!\n";
}

// Display all packages
template <typename T1, typename T2>
void displayPackages(Package<T1, T2> packages[], int count) {
    cout << "\n============ PACKAGES ============\n";
    cout << "ID\tName\tType\tETA\tFee\tStatus\n";
    cout << "-----------------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        cout << packages[i].id << "\t" << packages[i].name << "\t" << packages[i].type << "\t"
            << packages[i].etaHours << "\t$" << fixed << setprecision(2) << packages[i].fee << "\t"
            << packages[i].status << "\n";
    }
}

// Mark package as delivered
template <typename T1, typename T2>
void markDelivered(Package<T1, T2> packages[], int count) {
    T1 id;
    cout << "Enter package ID to mark as delivered: ";
    cin >> id;
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (packages[i].id == id) {
            packages[i].status = "Delivered";
            cout << "Package " << id << " marked as Delivered.\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Package ID not found.\n";
}

// Delivery report
template <typename T1, typename T2>
void deliveryReport(Package<T1, T2> packages[], int count) {
    int delivered = 0;
    T2 revenue = 0;
    for (int i = 0; i < count; ++i) {
        if (packages[i].status == "Delivered") {
            delivered++;
            revenue += packages[i].fee;
        }
    }
    cout << "\n========== DELIVERY REPORT ==========\n";
    cout << "Total Packages: " << count << "\n";
    cout << "Delivered: " << delivered << "\n";
    cout << "Pending: " << count - delivered << "\n";
    cout << "Total Revenue: $" << fixed << setprecision(2) << revenue << "\n";
    cout << "=====================================\n";
};